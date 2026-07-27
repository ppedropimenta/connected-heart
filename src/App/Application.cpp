#include "Application.h"

#include "Core/Log.h"

void Application::begin() {
    LOG_INFO("Initializing application modules");

    for (Module* module : modules_) {
        if (module != nullptr) {
            module->begin();
        }
    }
}

void Application::update() {
    for (Module* module : modules_) {
        if (module != nullptr) {
            module->update();
        }
    }
}
