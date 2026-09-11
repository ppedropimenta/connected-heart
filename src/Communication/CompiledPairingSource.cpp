#include "CompiledPairingSource.h"

#include "Network/Secrets.h"

const char* CompiledPairingSource::pairedChatId() const {
    return TELEGRAM_PAIRED_CHAT_ID;
}
