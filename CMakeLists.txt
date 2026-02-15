cmake_minimum_required(VERSION 3.24)
project(fractus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Vulkan REQUIRED)

add_executable(fractus
    src/main.cpp
    src/App.cpp
    src/App.h
    src/Win32Window.cpp
    src/Win32Window.h
    src/VulkanRenderer.cpp
    src/VulkanRenderer.h
)

target_link_libraries(fractus PRIVATE Vulkan::Vulkan)
target_compile_definitions(fractus PRIVATE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    VK_USE_PLATFORM_WIN32_KHR
    UNICODE
    _UNICODE
)

if(MSVC)
    target_compile_options(fractus PRIVATE /W4 /permissive-)
else()
    target_compile_options(fractus PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(cmake/CompileShaders.cmake)
fractus_configure_shader_target(
    compile_shaders
    "${CMAKE_SOURCE_DIR}/shaders/triangle.vert"
    "${CMAKE_SOURCE_DIR}/shaders/triangle.frag"
)
add_dependencies(fractus compile_shaders)

set_property(TARGET fractus PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
