#pragma once

#include <Arduino.h>

#include <GxEPD2_BW.h>

#include <Core/Module.h>

class EpaperDisplay final : public Module {
public:
    EpaperDisplay();

    void begin() override;
    void update() override;

    void showStartupScreen();
    void showMessage(const char* title, const char* message);
    void clear();

    [[nodiscard]]
    bool isInitialized() const;

private:
    GxEPD2_BW<
        GxEPD2_213_BN,
        GxEPD2_213_BN::HEIGHT
    > display_;

    void prepareDrawing();
    void drawCenteredText(
        const char* text,
        int16_t centerX,
        int16_t baselineY
    );

    bool initialized_ = false;
};


