#include "AcknowledgementCatalog.h"

const char* AcknowledgementCatalog::delivered() {
    return "Chegou! O coração recebeu sua mensagem.";
}

const char* AcknowledgementCatalog::deliveredReplacingUnrevealed() {
    return "Chegou! Mas a mensagem anterior ainda não tinha sido vista, "
           "e foi substituída por essa.";
}

const char* AcknowledgementCatalog::rejectedTooLong() {
    return "Essa mensagem é longa demais para o coração. "
           "Tente escrever algo mais curto.";
}

const char* AcknowledgementCatalog::rejectedNotText() {
    return "O coração só recebe mensagens de texto. "
           "Essa chegou vazia ou em um formato que ele não entende.";
}

const char* AcknowledgementCatalog::paired() {
    return "Pareado! Agora as mensagens que você enviar vão aparecer no coração.";
}
