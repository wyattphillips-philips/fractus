#include "App.h"

#include <chrono>
#include <stdexcept>
#include <thread>

int App::run() {
    if (!window_.create(L"fractus - Vulkan Triangle", 1280, 720)) {
        throw std::runtime_error("Failed to create Win32 window.");
    }

    renderer_.initialize(window_.hwnd(), window_.width(), window_.height());

    while (!window_.shouldClose()) {
        window_.pollEvents();
        if (window_.shouldClose()) {
            break;
        }

        uint32_t resizedWidth = 0;
        uint32_t resizedHeight = 0;
        if (window_.consumeResizeEvent(resizedWidth, resizedHeight)) {
            renderer_.onResize(resizedWidth, resizedHeight);
        }

        if (window_.isMinimized()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            continue;
        }

        renderer_.drawFrame();
    }

    renderer_.shutdown();
    return 0;
}
