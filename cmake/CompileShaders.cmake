function(fractus_configure_shader_target target_name)
    if(DEFINED ENV{VULKAN_SDK})
        set(_vulkan_glslc "$ENV{VULKAN_SDK}/Bin/glslc.exe")
        if(EXISTS "${_vulkan_glslc}")
            set(GLSLC_EXECUTABLE "${_vulkan_glslc}")
        endif()
    endif()

    if(NOT GLSLC_EXECUTABLE)
        find_program(GLSLC_EXECUTABLE NAMES glslc REQUIRED)
    endif()

    set(SHADER_OUTPUT_DIR "${CMAKE_BINARY_DIR}/shaders")
    file(MAKE_DIRECTORY "${SHADER_OUTPUT_DIR}")

    set(shader_outputs)
    foreach(shader IN LISTS ARGN)
        get_filename_component(shader_filename "${shader}" NAME)
        set(shader_output "${SHADER_OUTPUT_DIR}/${shader_filename}.spv")
        add_custom_command(
            OUTPUT "${shader_output}"
            COMMAND "${GLSLC_EXECUTABLE}" "${shader}" -o "${shader_output}"
            DEPENDS "${shader}"
            COMMENT "Compiling ${shader_filename} -> ${shader_output}"
            VERBATIM
        )
        list(APPEND shader_outputs "${shader_output}")
    endforeach()

    add_custom_target(${target_name} DEPENDS ${shader_outputs})
endfunction()
