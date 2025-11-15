cmake_minimum_required(VERSION 3.10)
project(civeeEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Dependencies
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

# Setup ImGui (Source files)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/vendor/imgui)

if(NOT EXISTS "${IMGUI_DIR}/imgui.h")
    message(FATAL_ERROR "imgui.h not found! Did you clone it into vendor/imgui?")
endif()

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Define Executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Engine.cpp
    src/Core/GraphicsContext.cpp
    ${IMGUI_SOURCES}
)

# Include Paths
target_include_directories(${PROJECT_NAME} PRIVATE 
    include 
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${SDL2_INCLUDE_DIRS} 
    ${GLEW_INCLUDE_DIRS}
)

# Link Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${SDL2_LIBRARIES} 
    ${GLEW_LIBRARIES} 
    OpenGL::GL
)
