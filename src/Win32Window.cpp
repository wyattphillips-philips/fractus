#include "Win32Window.h"

#include <stdexcept>

Win32Window::Win32Window()
    : hInstance_(GetModuleHandleW(nullptr)),
      className_(L"FractusMainWindowClass") {
}

Win32Window::~Win32Window() {
    if (hwnd_ != nullptr) {
        DestroyWindow(hwnd_);
        hwnd_ = nullptr;
    }
}

bool Win32Window::create(const wchar_t* title, uint32_t width, uint32_t height) {
    WNDCLASSEXW windowClass{};
    windowClass.cbSize = sizeof(windowClass);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = StaticWindowProc;
    windowClass.hInstance = hInstance_;
    windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    windowClass.lpszClassName = className_.c_str();

    if (RegisterClassExW(&windowClass) == 0) {
        if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
            return false;
        }
    }

    RECT desiredRect{0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
    constexpr DWORD windowStyle = WS_OVERLAPPEDWINDOW;
    AdjustWindowRect(&desiredRect, windowStyle, FALSE);

    hwnd_ = CreateWindowExW(
        0,
        className_.c_str(),
        title,
        windowStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        desiredRect.right - desiredRect.left,
        desiredRect.bottom - desiredRect.top,
        nullptr,
        nullptr,
        hInstance_,
        this
    );

    if (hwnd_ == nullptr) {
        return false;
    }

    ShowWindow(hwnd_, SW_SHOW);
    UpdateWindow(hwnd_);
    updateClientSize();
    resizePending_ = true;
    shouldClose_ = false;
    return true;
}

void Win32Window::pollEvents() {
    MSG message{};
    while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
}

bool Win32Window::shouldClose() const {
    return shouldClose_;
}

bool Win32Window::isMinimized() const {
    return width_ == 0 || height_ == 0;
}

bool Win32Window::consumeResizeEvent(uint32_t& width, uint32_t& height) {
    if (!resizePending_) {
        return false;
    }

    resizePending_ = false;
    width = width_;
    height = height_;
    return true;
}

HWND Win32Window::hwnd() const {
    return hwnd_;
}

uint32_t Win32Window::width() const {
    return width_;
}

uint32_t Win32Window::height() const {
    return height_;
}

LRESULT CALLBACK Win32Window::StaticWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_NCCREATE) {
        const auto* createStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
        auto* window = static_cast<Win32Window*>(createStruct->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        window->hwnd_ = hwnd;
    }

    auto* window = reinterpret_cast<Win32Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (window != nullptr) {
        return window->windowProc(hwnd, message, wParam, lParam);
    }

    return DefWindowProcW(hwnd, message, wParam, lParam);
}

LRESULT Win32Window::windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CLOSE:
            shouldClose_ = true;
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            shouldClose_ = true;
            hwnd_ = nullptr;
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:
            width_ = static_cast<uint32_t>(LOWORD(lParam));
            height_ = static_cast<uint32_t>(HIWORD(lParam));
            resizePending_ = true;
            return 0;
        default:
            break;
    }

    return DefWindowProcW(hwnd, message, wParam, lParam);
}

void Win32Window::updateClientSize() {
    RECT clientRect{};
    if (GetClientRect(hwnd_, &clientRect) == 0) {
        throw std::runtime_error("Failed to query window client size.");
    }

    width_ = static_cast<uint32_t>(clientRect.right - clientRect.left);
    height_ = static_cast<uint32_t>(clientRect.bottom - clientRect.top);
}
