#include "CompiledPairingSource.h"

#include "Network/Secrets.h"

const char* CompiledPairingSource::pairedChatId() const {
    return TELEGRAM_PAIRED_CHAT_ID;
}

const char* CompiledPairingSource::pairingCode() const {
    /*
     * A bench build is already paired via a compiled chat identifier;
     * pairing by code never runs, so this value is never shown or checked.
     */
    return "";
}

void CompiledPairingSource::bind(const char*) {
    // A bench build is already paired; binding never applies.
}
