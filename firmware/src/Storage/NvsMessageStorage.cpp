#include "NvsMessageStorage.h"

#include <Preferences.h>

#include "Message/MessageKind.h"

namespace {
    constexpr const char* Namespace = "heart";
    constexpr const char* KindKey = "msg_kind";
    constexpr const char* FeelingKey = "msg_feeling";
    constexpr const char* TextKey = "msg_text";
}

const HeartMessage* NvsMessageStorage::loadCurrentMessage() {
    Preferences preferences;

    if (!preferences.begin(Namespace, true)) {
        return nullptr;
    }

    if (!preferences.isKey(KindKey)) {
        preferences.end();
        return nullptr;
    }

    const auto kind = static_cast<MessageKind>(preferences.getUChar(KindKey));

    if (kind == MessageKind::Predefined) {
        const auto feeling = static_cast<Feeling>(preferences.getUChar(FeelingKey));
        cached_.set(HeartMessage(feeling));
    } else {
        char text[HeartMessage::TextCapacity];
        preferences.getString(TextKey, text, sizeof(text));
        cached_.set(HeartMessage(text));
    }

    preferences.end();

    return cached_.get();
}

void NvsMessageStorage::saveCurrentMessage(const HeartMessage& message) {
    Preferences preferences;

    if (!preferences.begin(Namespace, false)) {
        return;
    }

    preferences.putUChar(KindKey, static_cast<uint8_t>(message.kind()));

    /*
     * Only one of these is meaningful for the message being saved, but a
     * kind switch (e.g. a Predefined message replacing a Custom one) must
     * not leave the other kind's stale value behind in this shared
     * namespace, which also holds the credentials.
     */
    if (message.kind() == MessageKind::Predefined) {
        preferences.putUChar(FeelingKey, static_cast<uint8_t>(message.feeling()));
        preferences.remove(TextKey);
    } else {
        preferences.putString(TextKey, message.text());
        preferences.remove(FeelingKey);
    }

    preferences.end();
}

void NvsMessageStorage::clear() {
    Preferences preferences;

    if (!preferences.begin(Namespace, false)) {
        return;
    }

    preferences.clear();
    preferences.end();

    cached_.reset();
}
