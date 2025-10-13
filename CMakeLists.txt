cmake_minimum_required(VERSION 3.14)
project(MyApp LANGUAGES CXX)

# Требуем C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определяем тип сборки по умолчанию (debug)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Проверка допустимых типов сборки
if(NOT CMAKE_BUILD_TYPE MATCHES "^(Debug|Release)$")
    message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}. Use 'Debug' or 'Release'.")
endif()

# Исходники
set(SOURCES
    tests/main.cpp
    engine/core/Engine.cpp
    engine/core/SpatialPartitioning.cpp
    engine/core/events/EventSystem.cpp
    engine/core/window/GameWindow.cpp
    engine/core/utils/Logger.cpp
    engine/core/Systems.cpp
    engine/core/ecs/components/ScriptComponent.cpp
    engine/core/utils/Time.cpp
    engine/core/ui/Settings.cpp
    engine/core/graphics/renderer/Renderer.cpp
    engine/core/graphics/shaders/Shader.cpp
    engine/core/graphics/shaders/ShaderManager.cpp
    engine/core/graphics/textures/TextureLoader.cpp
    engine/core/ui/ImGuiContext.cpp
    extern/imgui/imgui.cpp
    extern/imgui/imgui_draw.cpp
    extern/imgui/imgui_tables.cpp
    extern/imgui/imgui_widgets.cpp
    extern/imgui/backends/imgui_impl_glfw.cpp
    extern/imgui/backends/imgui_impl_opengl3.cpp
)

# Добавляем исполняемый файл
add_executable(myapp ${SOURCES})

add_custom_command(TARGET myapp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:myapp>/assets
)

# Указываем include-директории
target_include_directories(myapp PRIVATE
    .
    include
    extern/imgui
)

# Настройка флагов компиляции
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(myapp PRIVATE -g -Wall)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(myapp PRIVATE -O3 -DNDEBUG -Wall)
endif()

# Поиск и линковка зависимостей (только для Linux)
if(UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED IMPORTED_TARGET glfw3)

    target_link_libraries(myapp
        OpenGL::GL
        GLEW::GLEW
        PkgConfig::GLFW
    )
else()
    message(FATAL_ERROR "Only Linux is supported.")
endif()
