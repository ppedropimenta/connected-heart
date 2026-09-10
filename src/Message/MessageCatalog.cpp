#include "MessageCatalog.h"

const char* MessageCatalog::textFor(const Feeling feeling) {
    switch (feeling) {
        case Feeling::Love:
            return "Te amo!";

        case Feeling::MissYou:
            return "Morrendo de saudade!";

        case Feeling::ThinkingOfYou:
            return "Pensando em ti";

        case Feeling::GoodMorning:
            return "Bom dia, meu amor!";

        case Feeling::GoodNight:
            return "Boa noite, meu amor!";
    }

    return "";
}
