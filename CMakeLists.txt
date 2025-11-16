cmake_minimum_required(VERSION 3.10)
project(civeeEngine LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OpenGL_GL_PREFERENCE GLVND)

# Find Dependencies
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(Glad STATIC vendor/glad/src/glad.c)
target_include_directories(Glad PRIVATE vendor/glad/include)

# Setup ImGui (Source files)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/vendor/imgui)
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
    
    src/Renderer/Shader.cpp
    src/Renderer/Buffer.cpp
    src/Renderer/RendererAPI.cpp
    src/Renderer/RenderCommand.cpp
    src/Renderer/VertexArray.cpp

    src/Platform/OpenGL/OpenGLShader.cpp
    src/Platform/OpenGL/OpenGLBuffer.cpp
    src/Platform/OpenGL/OpenGLRendererAPI.cpp
    src/Platform/OpenGL/OpenGLVertexArray.cpp

    ${IMGUI_SOURCES}
)

# Include Paths
target_include_directories(${PROJECT_NAME} PRIVATE 
    include
    src
    src/Platform/OpenGL

    vendor/glad/include
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${SDL2_INCLUDE_DIRS} 
)

# Link Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Glad
    SDL2::SDL2
    OpenGL::GL
    dl
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
    COMMENT "Linking assets folder..."
)
