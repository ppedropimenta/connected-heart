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
        "<title>Connected Heart</title></head>"
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

}

constexpr std::uint16_t ProvisioningPortal::DnsPort;
constexpr std::uint16_t ProvisioningPortal::HttpPort;

ProvisioningPortal::ProvisioningPortal(NvsCredentialSource& credentialSource, PairingSource& pairingSource)
    : credentialSource_(credentialSource)
    , pairingSource_(pairingSource)
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
    wifiConfirmed_ = false;
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
    if (!wifiConfirmed_ || !pairingSource_.isPaired()) {
        return false;
    }

    wifiConfirmed_ = false;
    return true;
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

        /*
         * Per ticket 12, Setup no longer ends the moment WiFi connects: it
         * also waits on pairing, which can take a while if the Sender is
         * slow to send the code. WifiManager isn't handed this connection
         * until that full handoff (see justProvisioned()), so this is
         * armed here to keep the link alive on its own for however long
         * that wait turns out to be.
         */
        WiFi.setAutoReconnect(true);

        connectAttempt_ = ConnectAttempt::Succeeded;
        wifiConfirmed_ = true;
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
            server_.send(200, "text/html", buildSuccessPage());
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
            "<title>Connected Heart</title>"
            "<style>"
            "body{font-family:sans-serif;max-width:420px;margin:32px auto;padding:0 16px;}"
            "select,input,button{width:100%;padding:10px;margin:8px 0;font-size:16px;box-sizing:border-box;}"
            "button{background:#d1477a;color:#fff;border:none;border-radius:6px;}"
            ".error{color:#b00020;}"
            "</style></head><body>";
    html += "<h1>Configurar o Connected Heart</h1>";

    /*
     * Only shown while unpaired: a heart reopening this page during
     * ADR-0004's automatic recovery is already paired, and showing a
     * pairing code there would wrongly suggest re-pairing is needed.
     */
    if (!pairingSource_.isPaired()) {
        html += "<p>Código de pareamento: <strong>";
        html += pairingSource_.pairingCode();
        html += "</strong></p>";
        html += "<p>Peça para a pessoa que vai te mandar mensagens enviar esse "
                "código para o bot no Telegram.</p>";
    }

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

String ProvisioningPortal::buildSuccessPage() const {
    String html;
    html.reserve(768);

    html += "<!DOCTYPE html><html><head><meta charset=\"utf-8\">"
            "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
            "<title>Connected Heart</title></head>"
            "<body style=\"font-family:sans-serif;max-width:420px;margin:32px auto;padding:0 16px;\">";
    html += "<h1>Conectado!</h1>";

    if (pairingSource_.isPaired()) {
        /*
         * Recovery per ADR-0004: WiFi just came back, pairing already
         * happened long ago, so there is nothing left to wait on here.
         */
        html += "<p>O aparelho esta se conectando à sua rede. Pode fechar esta pagina.</p>";
    } else {
        html += "<p>Agora peça para a pessoa que vai te mandar mensagens enviar este "
                "código para o bot dela no Telegram:</p>";
        html += "<p style=\"font-size:28px;font-weight:bold;text-align:center;\">";
        html += pairingSource_.pairingCode();
        html += "</p>";
        html += "<p>Pode fechar esta pagina depois disso.</p>";
    }

    html += "</body></html>";

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
