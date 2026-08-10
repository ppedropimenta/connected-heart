#pragma once

#include <Arduino.h>
#include <GxEPD2_BW.h>

#include <Core/Module.h>

/**
 * @brief Low-level interface for the Connected Heart e-paper display.
 *
 * EpaperDisplay encapsulates the physical display, GxEPD2 and generic
 * drawing operations.
 *
 * It does not know anything about device states, messages, Telegram,
 * Wi-Fi or application flow.
 */
class EpaperDisplay final : public Module {
public:
    EpaperDisplay();

    /**
     * @brief Initializes the physical e-paper display.
     */
    void begin() override;

    /**
     * @brief Periodic update hook.
     *
     * The e-paper currently requires no continuous processing.
     */
    void update() override;

    /**
     * @brief Executes a complete display render.
     *
     * The provided drawing function may be executed multiple times because
     * GxEPD2 uses paged rendering. Therefore, the drawing function must not
     * modify application state.
     *
     * @tparam DrawFunction Callable containing drawing operations.
     *
     * @return true if rendering was started.
     * @return false if the display has not been initialized.
     */
    template<typename DrawFunction>
    bool render(DrawFunction drawFunction) {
        if (!initialized_) {
            return false;
        }

        prepareDrawing();

        display_.firstPage();

        do {
            display_.fillScreen(GxEPD_WHITE);

            drawFunction();
        }
        while (display_.nextPage());

        return true;
    }

    /**
     * @brief Selects the regular application font.
     */
    void setRegularFont();

    /**
     * @brief Selects the bold application font.
     */
    void setBoldFont();

    /**
     * @brief Draws text centered around the provided coordinates.
     */
    void drawCenteredText(
        const String& text,
        int16_t centerX,
        int16_t centerY
    );

    /**
     * @brief Draws word-wrapped text centered horizontally and vertically.
     *
     * Text that cannot fit within maxLines is truncated and receives "...".
     */
    void drawCenteredWrappedText(
        const String& text,
        int16_t centerX,
        int16_t centerY,
        int16_t maxWidth,
        uint8_t maxLines,
        int16_t lineHeight
    );

    /**
     * @brief Draws the Connected Heart bitmap centered around a coordinate.
     */
    void drawHeart(
        int16_t centerX,
        int16_t centerY
    );

    /**
     * @brief Draws a small crossed connection symbol in the upper-right area.
     */
    void drawConnectionErrorIndicator();

    /**
     * @brief Returns the logical display width for the current rotation.
     */
    [[nodiscard]]
    int16_t width() const;

    /**
     * @brief Returns the logical display height for the current rotation.
     */
    [[nodiscard]]
    int16_t height() const;

    /**
     * @brief Returns whether the physical display was initialized.
     */
    [[nodiscard]]
    bool isInitialized() const;

private:
    static constexpr uint8_t MAX_WRAPPED_LINES = 6;

    GxEPD2_BW<
        GxEPD2_213_BN,
        GxEPD2_213_BN::HEIGHT
    > display_;

    bool initialized_ = false;

    void prepareDrawing();

    [[nodiscard]]
    uint16_t measureTextWidth(const String& text);

    [[nodiscard]]
    String normalizeText(const String& text) const;

    [[nodiscard]]
    String truncateToWidth(
        const String& text,
        int16_t maxWidth
    );
};