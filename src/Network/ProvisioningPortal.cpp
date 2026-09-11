#include "ProvisioningPortal.h"

#include <Arduino.h>
#include <WiFi.h>

#include "Core/Config.h"
#include "Core/Log.h"

namespace {
    constexpr const char* ScanningPage =
        "<!DOCTYPE html><html><head><meta charset=\"utf-8\">"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        "<meta http-equiv=\"refresh\" content=\"2\">"
        "<title>Coracao Conectado</title></head>"
        "<body style=\"font-family:sans-serif;max-width:420px;margin:32px auto;padding:0 16px;\">"
        "<h1>Procurando redes...</h1>"
        "<p>Um instante.</p>"
        "</body></html>";

    constexpr const char* ConnectingPage =
        "<!DOCTYPE html><html><head><meta charset=\"utf-8\">"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        "<meta http-equiv=\"refresh\" content=\"2;url=/status\">"
        "<title>Coracao Conectado</title></head>"
        "<body style=\"font-family:sans-serif;max-width:420px;margin:32px auto;padding:0 16px;\">"
        "<h1>Conectando...</h1>"
        "<p>Aguarde um instante.</p>"
        "</body></html>";

    constexpr const char* SuccessPage =
        "<!DOCTYPE html><html><head><meta charset=\"utf-8\">"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        "<title>Coracao Conectado</title></head>"
        "<body style=\"font-family:sans-serif;max-width:420px;margin:32px auto;padding:0 16px;\">"
        "<h1>Conectado!</h1>"
        "<p>O coracao esta se conectando a sua rede. Pode fechar esta pagina.</p>"
        "</body></html>";
}

/*
 * Out-of-class definitions for the static constexpr members: the project
 * targets a pre-C++17 standard, where a reference-binding use (as DnsPort
 * gets passed into below) odr-uses the member and needs one of these.
 */
constexpr std::uint16_t ProvisioningPortal::DnsPort;
constexpr std::uint16_t ProvisioningPortal::HttpPort;

ProvisioningPortal::ProvisioningPortal(NvsCredentialSource& credentialSource)
    : credentialSource_(credentialSource)
{
}

void ProvisioningPortal::begin() {
    if (active_) {
        return;
    }

    LOG_INFO("Starting provisioning portal, AP '%s'", Config::ProvisioningApSsid);

    /*
     * The access point is intentionally open: asking for an AP password
     * before the configuration page can even be reached would add exactly
     * the kind of friction this ticket exists to remove. It only stays up
     * for the short Setup window.
     */
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(Config::ProvisioningApSsid);

    dnsServer_.start(DnsPort, "*", WiFi.softAPIP());

    server_.on("/", HTTP_GET, [this]() { handleRoot(); });
    server_.on("/connect", HTTP_POST, [this]() { handleConnect(); });
    server_.on("/status", HTTP_GET, [this]() { handleStatus(); });
    server_.onNotFound([this]() { handleNotFound(); });
    server_.begin();

    active_ = true;
    provisioned_ = false;
    connectAttempt_ = ConnectAttempt::None;
}

void ProvisioningPortal::end() {
    if (!active_) {
        return;
    }

    LOG_INFO("Stopping provisioning portal");

    server_.stop();
    dnsServer_.stop();
    WiFi.scanDelete();
    WiFi.softAPdisconnect(true);

    active_ = false;
}

void ProvisioningPortal::update() {
    if (!active_) {
        return;
    }

    dnsServer_.processNextRequest();
    server_.handleClient();
    pollConnectAttempt();
}

bool ProvisioningPortal::active() const {
    return active_;
}

bool ProvisioningPortal::justProvisioned() {
    const bool value = provisioned_;
    provisioned_ = false;
    return value;
}

void ProvisioningPortal::pollConnectAttempt() {
    if (connectAttempt_ != ConnectAttempt::InProgress) {
        return;
    }

    if (WiFi.status() == WL_CONNECTED) {
        LOG_INFO("Provisioning attempt succeeded: SSID '%s'", pendingSsid_.c_str());

        /*
         * Only a confirmed-working network is persisted, so a wrong
         * password never leaves storage in a state that would make the
         * next boot think the heart is provisioned.
         */
        credentialSource_.save({pendingSsid_.c_str(), pendingPassword_.c_str()});

        connectAttempt_ = ConnectAttempt::Succeeded;
        provisioned_ = true;
        return;
    }

    if (millis() - connectAttemptStartedAt_ >= Config::ProvisioningConnectTimeoutMs) {
        LOG_WARN("Provisioning attempt failed: SSID '%s'", pendingSsid_.c_str());

        WiFi.disconnect();
        connectAttempt_ = ConnectAttempt::Failed;
    }
}

void ProvisioningPortal::handleRoot() {
    /*
     * The scan itself is async (scanNetworks(true)); handleRoot() and
     * handleStatus() both just poll scanComplete() so neither the web
     * server nor the rest of the loop ever blocks on it.
     */
    const int16_t scanResult = WiFi.scanComplete();

    if (scanResult == WIFI_SCAN_FAILED) {
        WiFi.scanNetworks(true);
        server_.send(200, "text/html", ScanningPage);
        return;
    }

    if (scanResult == WIFI_SCAN_RUNNING) {
        server_.send(200, "text/html", ScanningPage);
        return;
    }

    server_.send(200, "text/html", buildConfigurationPage());
}

void ProvisioningPortal::handleStatus() {
    switch (connectAttempt_) {
        case ConnectAttempt::InProgress:
            server_.send(200, "text/html", ConnectingPage);
            return;

        case ConnectAttempt::Succeeded:
            server_.send(200, "text/html", SuccessPage);
            return;

        case ConnectAttempt::Failed:
            connectAttempt_ = ConnectAttempt::None;
            server_.send(
                200,
                "text/html",
                buildConfigurationPage("Nao foi possivel conectar. Verifique a senha e tente novamente.")
            );
            return;

        case ConnectAttempt::None:
            handleRoot();
            return;
    }
}

void ProvisioningPortal::handleNotFound() {
    /*
     * Captive portal detection: phones probe an arbitrary URL right after
     * joining and expect either a 204 (already has internet) or a redirect
     * to the portal. Redirecting everything unknown here is what makes the
     * configuration page pop up on its own instead of the recipient having
     * to find it.
     */
    server_.sendHeader(
        "Location",
        String("http://") + WiFi.softAPIP().toString(),
        true
    );
    server_.send(302, "text/plain", "");
}

void ProvisioningPortal::handleConnect() {
    const String ssid = server_.arg("ssid");
    const String password = server_.arg("password");

    if (ssid.length() == 0) {
        server_.send(200, "text/html", buildConfigurationPage("Escolha uma rede."));
        return;
    }

    LOG_INFO("Provisioning attempt: SSID '%s'", ssid.c_str());

    pendingSsid_ = ssid;
    pendingPassword_ = password;
    connectAttempt_ = ConnectAttempt::InProgress;
    connectAttemptStartedAt_ = millis();

    WiFi.begin(ssid.c_str(), password.c_str());

    server_.send(200, "text/html", ConnectingPage);
}

String ProvisioningPortal::buildConfigurationPage(const char* errorMessage) const {
    String html;
    html.reserve(2048);

    html += "<!DOCTYPE html><html><head><meta charset=\"utf-8\">"
            "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
            "<title>Coracao Conectado</title>"
            "<style>"
            "body{font-family:sans-serif;max-width:420px;margin:32px auto;padding:0 16px;}"
            "select,input,button{width:100%;padding:10px;margin:8px 0;font-size:16px;box-sizing:border-box;}"
            "button{background:#d1477a;color:#fff;border:none;border-radius:6px;}"
            ".error{color:#b00020;}"
            "</style></head><body>";
    html += "<h1>Configurar o Coracao Conectado</h1>";

    if (errorMessage != nullptr) {
        html += "<p class=\"error\">";
        html += errorMessage;
        html += "</p>";
    }

    html += "<form method=\"POST\" action=\"/connect\">";
    html += "<label for=\"ssid\">Rede</label>";
    html += "<select name=\"ssid\" id=\"ssid\">";
    html += scanNetworksHtml();
    html += "</select>";
    html += "<label for=\"password\">Senha</label>";
    html += "<input type=\"password\" name=\"password\" id=\"password\">";
    html += "<button type=\"submit\">Conectar</button>";
    html += "</form></body></html>";

    return html;
}

String ProvisioningPortal::scanNetworksHtml() const {
    const int16_t count = WiFi.scanComplete();

    String options;

    if (count <= 0) {
        options += "<option value=\"\">Nenhuma rede encontrada</option>";
        return options;
    }

    for (int16_t i = 0; i < count; ++i) {
        const String ssid = WiFi.SSID(i);

        if (ssid.length() == 0) {
            continue;
        }

        options += "<option value=\"";
        options += ssid;
        options += "\">";
        options += ssid;
        options += "</option>";
    }

    return options;
}
