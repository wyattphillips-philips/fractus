#pragma once

#include "VulkanRenderer.h"
#include "Win32Window.h"

class App {
public:
    int run();

private:
    Win32Window window_;
    VulkanRenderer renderer_;
};
