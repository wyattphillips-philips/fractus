#pragma once

#include <cstdint>
#include <string>

#include <windows.h>

class Win32Window {
public:
    Win32Window();
    ~Win32Window();

    Win32Window(const Win32Window&) = delete;
    Win32Window& operator=(const Win32Window&) = delete;

    bool create(const wchar_t* title, uint32_t width, uint32_t height);
    void pollEvents();

    [[nodiscard]] bool shouldClose() const;
    [[nodiscard]] bool isMinimized() const;
    bool consumeResizeEvent(uint32_t& width, uint32_t& height);

    [[nodiscard]] HWND hwnd() const;
    [[nodiscard]] uint32_t width() const;
    [[nodiscard]] uint32_t height() const;

private:
    static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    void updateClientSize();

    HINSTANCE hInstance_{nullptr};
    HWND hwnd_{nullptr};
    uint32_t width_{0};
    uint32_t height_{0};
    bool shouldClose_{false};
    bool resizePending_{false};
    std::wstring className_;
};
