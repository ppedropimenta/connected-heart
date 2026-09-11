#include "Application.h"

#include "Core/Config.h"
#include "Core/Log.h"
#include "Led/LedPatternSelector.h"
#include "Message/MessageCatalog.h"

Application::Application(
    DisplayRenderer& renderer,
    Led& led,
    TouchSensor& touch,
    MessageSource& messageSource,
    MessageStorage& storage,
    Clock& clock
)
    : renderer_(renderer)
    , led_(led)
    , touch_(touch)
    , messageSource_(messageSource)
    , storage_(storage)
    , clock_(clock)
{
}

void Application::begin(const bool provisioned) {
    LOG_INFO("Initializing application modules");

    const HeartMessage* restoredMessage = storage_.loadCurrentMessage();

    if (restoredMessage != nullptr) {
        messageManager_.restoreCurrent(*restoredMessage);
    }

    renderer_.showStartup();

    processEvent(DeviceEvent::InitializationFinished, provisioned);
}

void Application::update() {
    processInputs();
    processTimers();

    /*
     * Application only calls led_.show() on state entry (a new pattern);
     * a rhythm like a breathe or a blink needs continuous frames in
     * between to animate.
     */
    led_.update();
}

void Application::processInputs() {
    messageSource_.update();

    if (messageSource_.hasMessage()) {
        const HeartMessage message = messageSource_.takeMessage();

        if (messageManager_.receive(message)) {
            processEvent(DeviceEvent::MessageReceived);
        }
    }

    touch_.update();

    if (touch_.wasPressed()) {
        processEvent(DeviceEvent::TouchPressed);
    }
}

void Application::processTimers() {
    if (stateMachine_.state() != DeviceState::RevealingMessage) {
        return;
    }

    if (clock_.millis() - stateEnteredAt_ >= Config::RevealDurationMs) {
        processEvent(DeviceEvent::RevealFinished);
    }
}

void Application::notifyProvisioningFinished() {
    processEvent(DeviceEvent::ProvisioningFinished);
}

void Application::notifyConnectionEstablished() {
    consecutiveConnectionFailures_ = 0;
    setConnectionState(ConnectionState::Connected);

    processEvent(DeviceEvent::ConnectionEstablished);
}

void Application::notifyConnectionAttemptFailed() {
    setConnectionState(ConnectionState::Error);

    ++consecutiveConnectionFailures_;

    if (consecutiveConnectionFailures_ < Config::MaxConsecutiveConnectionFailures) {
        return;
    }

    if (processEvent(DeviceEvent::ConnectionAttemptsExhausted)) {
        consecutiveConnectionFailures_ = 0;
    }
}

bool Application::processEvent(const DeviceEvent event, const bool provisioned) {
    const DeviceState previousState = stateMachine_.state();

    if (!stateMachine_.process(event, provisioned)) {
        return false;
    }

    const DeviceState newState = stateMachine_.state();

    if (event == DeviceEvent::RevealFinished && newState == DeviceState::DisplayingMessage) {
        /*
         * The reveal completed without being interrupted by another
         * message, so the pending message becomes the current one. This is
         * the only place the Current Message changes, so it is also the
         * only place storage needs to be written.
         */
        messageManager_.revealPending();
        storage_.saveCurrentMessage(*messageManager_.current());
    }

    if (newState != previousState) {
        enterState(newState);
    }

    return true;
}

void Application::enterState(const DeviceState state) {
    stateEnteredAt_ = clock_.millis();

    renderState(state);
}

void Application::renderState(const DeviceState state) {
    switch (state) {
        case DeviceState::Boot:
            break;

        case DeviceState::Setup:
            renderer_.showSetup();
            break;

        case DeviceState::Waking:
            renderer_.showConnecting();
            break;

        case DeviceState::WaitingForMessage:
            /*
             * A Current Message restored from storage is displayed rather
             * than the waiting screen: the recipient should never see an
             * empty screen when a message is actually available.
             */
            if (messageManager_.hasCurrent()) {
                showCurrentMessage();
            } else {
                renderer_.showWaitingForMessage(connectionState_);
            }
            break;

        case DeviceState::Notification:
            renderer_.showNotification(connectionState_);
            break;

        case DeviceState::RevealingMessage:
            renderer_.showRevealHeart(connectionState_);
            break;

        case DeviceState::DisplayingMessage:
            showCurrentMessage();
            break;
    }

    if (state != DeviceState::Boot) {
        led_.show(LedPatternSelector::select(state, connectionState_));
    }
}

void Application::setConnectionState(const ConnectionState state) {
    if (connectionState_ == state) {
        return;
    }

    connectionState_ = state;

    /*
     * A connection change alone never changes DeviceState (that's
     * ConnectionAttemptsExhausted's job once failures accumulate), so
     * enterState() will not run on its own here. The screen already shown
     * and the LED pattern are refreshed directly instead, so the indicator
     * appears (or clears) without disturbing anything else on screen.
     */
    renderState(stateMachine_.state());
}

void Application::showCurrentMessage() {
    const HeartMessage* message = messageManager_.current();

    renderer_.showMessage(
        message != nullptr ? MessageCatalog::resolve(*message) : "",
        connectionState_
    );
}
