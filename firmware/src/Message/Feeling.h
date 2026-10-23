#pragma once

/**
 * @brief The closed set of sentiments a Predefined Message can carry.
 *
 * Wording for each Feeling is resolved by MessageCatalog. Feeling carries
 * no notion of custom text; a Custom Message is expressed separately by
 * HeartMessage's kind.
 */
enum class Feeling {
    Love,
    MissYou,
    ThinkingOfYou,
    GoodMorning,
    GoodNight
};
