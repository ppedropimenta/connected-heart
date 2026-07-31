#include "MessageCatalog.h"

String MessageCatalog::textFor(MessageType type) {
    switch (type) {
        case MessageType::Love:
            return "Eu te amo";

        case MessageType::MissYou:
            return "Morrendo de saudade";

        case MessageType::ThinkingOfYou:
            return "Pensando em você";

        case MessageType::GoodMorning:
            return "Bom dia! Te amo gatinha";

        case MessageType::GoodNight:
            return "Boa noite. Durma bem meu amor";

        case MessageType::Custom:
            return "";
    }

    return "";
}