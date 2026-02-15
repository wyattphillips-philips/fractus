# fractus

Fractal explorer implemented in Vulkan.

## Current milestone

This project currently boots a Win32 window and renders a triangle with raw Vulkan API calls.

## Prerequisites

- Windows 10/11
- CMake 3.24+
- A C++20-capable compiler (MSVC recommended)
- Vulkan SDK installed (`VULKAN_SDK` set)

## Build

```powershell
cmake -S . -B build
cmake --build build
```

## Run

```powershell
.\build\fractus.exe
```

If you use a multi-config generator (Visual Studio), run the configuration-specific binary, for example:

```powershell
.\build\Debug\fractus.exe
```
