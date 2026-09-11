/**
* @file ProvisioningPortal.h
 * @brief Captive portal that lets Setup be configured from a phone with no
 * app: an access point, a configuration page listing visible networks, and
 * a password field.
 */

#pragma once

#include <cstdint>

#include <DNSServer.h>
#include <WebServer.h>

#include "NvsCredentialSource.h"

/**
 * @brief Raises the access point and web server Setup needs, and persists
 * a submitted network only once it has actually been confirmed to connect.
 *
 * Deliberately independent of WifiManager: WifiManager only ever runs once
 * the heart is provisioned, and this class owns the one WiFi STA connection
 * attempt used to validate a candidate network while its own access point
 * stays up. This keeps the two from having to coordinate a shared AP+STA
 * mode, at the cost of a brief STA reconnect handled by WifiManager once
 * the caller hands off to it after justProvisioned().
 *
 * Both the network scan and the candidate connection attempt are polled
 * from update() rather than blocked on, mirroring WifiManager's own
 * non-blocking design: a connection attempt can take up to
 * Config::ProvisioningConnectTimeoutMs, and blocking the single Arduino
 * loop for that long would freeze the Setup LED pattern, touch handling,
 * and the DNS/web server itself.
 *
 * begin()/end() may be called repeatedly over the device's life: Setup can
 * be re-entered long after boot, once the heart has been unable to connect
 * for a while (see StateMachine's ConnectionAttemptsExhausted handling).
 */
class ProvisioningPortal {
public:
    explicit ProvisioningPortal(NvsCredentialSource& credentialSource);

    /**
     * @brief Starts the access point, DNS, and web server. Does nothing if
     * already active().
     */
    void begin();

    /**
     * @brief Stops the access point, DNS, and web server. Does nothing if
     * not active().
     */
    void end();

    /**
     * @brief Services DNS and HTTP requests and advances any in-progress
     * connection attempt. Must be polled regularly while active(); does
     * nothing otherwise.
     */
    void update();

    [[nodiscard]]
    bool active() const;

    /**
     * @brief True once, on the update() where a submitted network was
     * confirmed to connect and its credentials were persisted.
     */
    bool justProvisioned();

private:
    /**
     * @brief Tracks the one candidate network being tested at a time,
     * polled from update() instead of blocking the request that submitted
     * it.
     */
    enum class ConnectAttempt {
        None,
        InProgress,
        Succeeded,
        Failed
    };

    static constexpr std::uint16_t DnsPort = 53;
    static constexpr std::uint16_t HttpPort = 80;

    NvsCredentialSource& credentialSource_;

    DNSServer dnsServer_;
    WebServer server_{HttpPort};

    bool active_ = false;
    bool provisioned_ = false;

    String pendingSsid_;
    String pendingPassword_;
    ConnectAttempt connectAttempt_ = ConnectAttempt::None;
    unsigned long connectAttemptStartedAt_ = 0;

    void handleRoot();
    void handleConnect();
    void handleStatus();
    void handleNotFound();

    void pollConnectAttempt();

    [[nodiscard]]
    String buildConfigurationPage(const char* errorMessage = nullptr) const;

    [[nodiscard]]
    String scanNetworksHtml() const;
};
