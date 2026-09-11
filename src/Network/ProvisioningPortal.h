/**
* @file ProvisioningPortal.h
 * @brief Captive portal that lets Setup be configured from a phone with no
 * app: an access point, a configuration page listing visible networks, a
 * password field, and the pairing code (ticket 12) so the recipient can
 * relay it to her Sender while she is still setting up.
 */

#pragma once

#include <cstdint>

#include <DNSServer.h>
#include <WebServer.h>

#include "Communication/PairingSource.h"
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
    ProvisioningPortal(NvsCredentialSource& credentialSource, PairingSource& pairingSource);

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
     * @brief True once Setup is fully done: a submitted network has been
     * confirmed to connect (its credentials persisted) and pairing (see
     * PairingSource) has completed.
     *
     * These can complete on different update()s: WiFi confirms as soon as
     * a candidate network is submitted, but pairing only completes once
     * the Sender actually sends the code, which may take a while longer.
     * The WiFi half is latched internally so it isn't lost while waiting
     * on the second; once both are true, this returns true exactly once.
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
    PairingSource& pairingSource_;

    DNSServer dnsServer_;
    WebServer server_{HttpPort};

    bool active_ = false;

    /**
     * @brief Latches a confirmed WiFi connection for the current Setup
     * visit, so it survives however many update()s pairing still takes
     * to complete. See justProvisioned().
     */
    bool wifiConfirmed_ = false;

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
    String buildSuccessPage() const;

    [[nodiscard]]
    String scanNetworksHtml() const;
};
