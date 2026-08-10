#include "EpaperDisplay.h"

#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include <Core/Log.h>
#include <Core/Pins.h>

namespace {
    constexpr int16_t HEART_WIDTH = 32;
    constexpr int16_t HEART_HEIGHT = 28;

    const uint8_t HEART_BITMAP[] PROGMEM = {
        0x00, 0x00, 0x00, 0x00,
        0x01, 0xFC, 0x3F, 0x80,
        0x07, 0xFE, 0x7F, 0xE0,
        0x0F, 0xFF, 0xFF, 0xF0,
        0x0F, 0xFF, 0xFF, 0xF0,
        0x1F, 0xFF, 0xFF, 0xF8,
        0x1F, 0xFF, 0xFF, 0xF8,
        0x1F, 0xFF, 0xFF, 0xF8,
        0x1F, 0xFF, 0xFF, 0xF8,
        0x1F, 0xFF, 0xFF, 0xF8,
        0x1F, 0xFF, 0xFF, 0xF8,
        0x1F, 0xFF, 0xFF, 0xF8,
        0x0F, 0xFF, 0xFF, 0xF0,
        0x0F, 0xFF, 0xFF, 0xF0,
        0x07, 0xFF, 0xFF, 0xE0,
        0x07, 0xFF, 0xFF, 0xE0,
        0x03, 0xFF, 0xFF, 0xC0,
        0x01, 0xFF, 0xFF, 0x80,
        0x00, 0xFF, 0xFF, 0x00,
        0x00, 0x7F, 0xFE, 0x00,
        0x00, 0x3F, 0xFC, 0x00,
        0x00, 0x0F, 0xF0, 0x00,
        0x00, 0x07, 0xE0, 0x00,
        0x00, 0x01, 0x80, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };
}

EpaperDisplay::EpaperDisplay()
    : display_(
        GxEPD2_213_BN(
            Pins::Display::CS,
            Pins::Display::DC,
            Pins::Display::RST,
            Pins::Display::BUSY
        )
    )
{}

void EpaperDisplay::begin() {
    LOG_INFO("Initializing e-paper display");

    display_.init(
        115200,
        true,
        50,
        false
    );

    display_.setRotation(1);
    display_.setTextColor(GxEPD_BLACK);

    initialized_ = true;

    LOG_INFO(
        "E-paper display initialized: %d x %d",
        display_.width(),
        display_.height()
    );
}

void EpaperDisplay::update() {
    // No periodic processing is currently required.
}

void EpaperDisplay::setRegularFont() {
    display_.setFont(&FreeMono9pt7b);
}

void EpaperDisplay::setBoldFont() {
    display_.setFont(&FreeMonoBold9pt7b);
}

void EpaperDisplay::drawCenteredText(
    const String& text,
    const int16_t centerX,
    const int16_t centerY
) {
    const String normalized = normalizeText(text);

    if (normalized.length() == 0) {
        return;
    }

    int16_t boundsX = 0;
    int16_t boundsY = 0;

    uint16_t boundsWidth = 0;
    uint16_t boundsHeight = 0;

    display_.getTextBounds(
        normalized,
        0,
        0,
        &boundsX,
        &boundsY,
        &boundsWidth,
        &boundsHeight
    );

    const int16_t cursorX =
        centerX
        - static_cast<int16_t>(boundsWidth / 2)
        - boundsX;

    const int16_t cursorY =
        centerY
        - static_cast<int16_t>(boundsHeight / 2)
        - boundsY;

    display_.setCursor(cursorX, cursorY);
    display_.print(normalized);
}

void EpaperDisplay::drawCenteredWrappedText(
    const String& text,
    const int16_t centerX,
    const int16_t centerY,
    const int16_t maxWidth,
    const uint8_t maxLines,
    const int16_t lineHeight
) {
    if (maxLines == 0) {
        return;
    }

    const uint8_t effectiveMaxLines =
        maxLines > MAX_WRAPPED_LINES
            ? MAX_WRAPPED_LINES
            : maxLines;

    const String normalized = normalizeText(text);

    if (normalized.length() == 0) {
        return;
    }

    String lines[MAX_WRAPPED_LINES];

    uint8_t lineCount = 0;
    String currentLine;

    int start = 0;

    while (start < normalized.length()) {
        while (
            start < normalized.length() &&
            normalized[start] == ' '
        ) {
            ++start;
        }

        if (start >= normalized.length()) {
            break;
        }

        int end = normalized.indexOf(' ', start);

        if (end < 0) {
            end = normalized.length();
        }

        String word = normalized.substring(start, end);

        /*
         * Protect against a single word that is wider than the entire
         * available area.
         */
        if (measureTextWidth(word) > maxWidth) {
            word = truncateToWidth(word, maxWidth);
        }

        String candidate;

        if (currentLine.length() == 0) {
            candidate = word;
        } else {
            candidate = currentLine + " " + word;
        }

        if (measureTextWidth(candidate) <= maxWidth) {
            currentLine = candidate;
        } else {
            if (lineCount >= effectiveMaxLines) {
                break;
            }

            lines[lineCount++] = currentLine;
            currentLine = word;

            if (lineCount == effectiveMaxLines) {
                break;
            }
        }

        start = end + 1;
    }

    bool truncated = start < normalized.length();

    if (currentLine.length() > 0 && lineCount < effectiveMaxLines) {
        lines[lineCount++] = currentLine;
    } else if (currentLine.length() > 0) {
        truncated = true;
    }

    if (lineCount == 0) {
        return;
    }

    if (truncated) {
        String lastLine = lines[lineCount - 1];

        while (lastLine.length() > 0 && measureTextWidth(lastLine + "...") > maxWidth) {
            lastLine.remove(lastLine.length() - 1);
        }

        lines[lineCount - 1] = lastLine + "...";
    }

    const int16_t totalHeight =
        static_cast<int16_t>(
            (lineCount - 1) * lineHeight
        );

    const int16_t firstLineCenterY =
        centerY - (totalHeight / 2);

    for (uint8_t i = 0; i < lineCount; ++i) {
        drawCenteredText(
            lines[i],
            centerX,
            firstLineCenterY +
                static_cast<int16_t>(i * lineHeight)
        );
    }
}

void EpaperDisplay::drawHeart(
    const int16_t centerX,
    const int16_t centerY
) {
    const int16_t x =
        centerX - HEART_WIDTH / 2;

    const int16_t y =
        centerY - HEART_HEIGHT / 2;

    display_.drawBitmap(
        x,
        y,
        HEART_BITMAP,
        HEART_WIDTH,
        HEART_HEIGHT,
        GxEPD_BLACK
    );
}

void EpaperDisplay::drawConnectionErrorIndicator() {
    /*
     * Small crossed wireless-style indicator.
     *
     * It intentionally avoids text so it consumes very little screen area.
     */

    constexpr int16_t indicatorWidth = 18;

    const int16_t x =
        display_.width() - indicatorWidth - 4;

    constexpr int16_t y = 5;

    // Upper signal segment.
    display_.drawLine(
        x,
        y + 4,
        x + 8,
        y,
        GxEPD_BLACK
    );

    display_.drawLine(
        x + 8,
        y,
        x + 16,
        y + 4,
        GxEPD_BLACK
    );

    // Middle signal segment.
    display_.drawLine(
        x + 4,
        y + 8,
        x + 8,
        y + 6,
        GxEPD_BLACK
    );

    display_.drawLine(
        x + 8,
        y + 6,
        x + 12,
        y + 8,
        GxEPD_BLACK
    );

    // Signal point.
    display_.fillCircle(
        x + 8,
        y + 12,
        1,
        GxEPD_BLACK
    );

    // Slash = unavailable.
    display_.drawLine(
        x + 2,
        y,
        x + 15,
        y + 14,
        GxEPD_BLACK
    );
}

int16_t EpaperDisplay::width() const {
    return display_.width();
}

int16_t EpaperDisplay::height() const {
    return display_.height();
}

bool EpaperDisplay::isInitialized() const {
    return initialized_;
}

void EpaperDisplay::prepareDrawing() {
    display_.setRotation(1);
    display_.setTextColor(GxEPD_BLACK);
    display_.setFullWindow();
}

uint16_t EpaperDisplay::measureTextWidth(
    const String& text
) {
    int16_t boundsX = 0;
    int16_t boundsY = 0;

    uint16_t boundsWidth = 0;
    uint16_t boundsHeight = 0;

    display_.getTextBounds(
        text,
        0,
        0,
        &boundsX,
        &boundsY,
        &boundsWidth,
        &boundsHeight
    );

    return boundsWidth;
}

String EpaperDisplay::truncateToWidth(
    const String& text,
    const int16_t maxWidth
) {
    String result = text;

    while (result.length() > 0 && measureTextWidth(result + "...") > maxWidth) {
        result.remove(result.length() - 1);
    }

    return result + "...";
}

String EpaperDisplay::normalizeText(const String& text) const {
    String result;
    result.reserve(text.length());

    for (size_t i = 0; i < text.length(); ++i) {
        const uint8_t current =
            static_cast<uint8_t>(text[i]);

        /*
         * Portuguese UTF-8 characters beginning with 0xC3.
         *
         * FreeMono GFX fonts used here contain the printable ASCII range,
         * so the MVP transliterates accented Portuguese characters.
         *
         * A proper Unicode-capable font can replace this later.
         */
        if (current == 0xC3 && i + 1 < text.length()) {
            const uint8_t next =
                static_cast<uint8_t>(text[++i]);

            switch (next) {
                // a: á à â ã ä
                case 0xA1:
                case 0xA0:
                case 0xA2:
                case 0xA3:
                case 0xA4:
                    result += 'a';
                    break;

                // A
                case 0x81:
                case 0x80:
                case 0x82:
                case 0x83:
                case 0x84:
                    result += 'A';
                    break;

                // e: é è ê ë
                case 0xA9:
                case 0xA8:
                case 0xAA:
                case 0xAB:
                    result += 'e';
                    break;

                // E
                case 0x89:
                case 0x88:
                case 0x8A:
                case 0x8B:
                    result += 'E';
                    break;

                // i
                case 0xAD:
                case 0xAC:
                case 0xAE:
                case 0xAF:
                    result += 'i';
                    break;

                // I
                case 0x8D:
                case 0x8C:
                case 0x8E:
                case 0x8F:
                    result += 'I';
                    break;

                // o
                case 0xB3:
                case 0xB2:
                case 0xB4:
                case 0xB5:
                case 0xB6:
                    result += 'o';
                    break;

                // O
                case 0x93:
                case 0x92:
                case 0x94:
                case 0x95:
                case 0x96:
                    result += 'O';
                    break;

                // u
                case 0xBA:
                case 0xB9:
                case 0xBB:
                case 0xBC:
                    result += 'u';
                    break;

                // U
                case 0x9A:
                case 0x99:
                case 0x9B:
                case 0x9C:
                    result += 'U';
                    break;

                // ç / Ç
                case 0xA7:
                    result += 'c';
                    break;

                case 0x87:
                    result += 'C';
                    break;

                default:
                    result += '?';
                    break;
            }

            continue;
        }

        if (current < 0x80)
        {
            result += static_cast<char>(current);
        }
    }

    return result;
}