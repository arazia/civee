cmake_minimum_required(VERSION 3.10)
project(civeeEngine LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OpenGL_GL_PREFERENCE GLVND)


add_compile_definitions(CIVEE_BUILD_DLL)
add_compile_definitions(ENGINE_ASSET_DIR="${CMAKE_SOURCE_DIR}/assets")

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(Glad STATIC
    vendor/glad/src/glad.c
)
set_target_properties(Glad PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(Glad PRIVATE
    vendor/glad/include
)

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
add_library(civeeCore SHARED
    src/Core/Application.cpp
    # src/Core/GraphicsContext.cpp

    src/Renderer/Renderer.cpp
    src/Renderer/RendererAPI.cpp
    src/Renderer/RenderCommand.cpp
    src/Renderer/Data/Shader.cpp
    src/Renderer/Data/Buffer.cpp
    src/Renderer/Data/VertexArray.cpp
    src/Renderer/Data/Texture.cpp
    src/Renderer/Scene/Mesh.cpp
    src/Renderer/Scene/Scene.cpp
    src/Renderer/Camera/OrthographicCamera.cpp
    src/Renderer/Camera/OrthographicCameraController.cpp

    src/Platform/OpenGL/OpenGLShader.cpp
    src/Platform/OpenGL/OpenGLBuffer.cpp
    src/Platform/OpenGL/OpenGLTexture.cpp
    src/Platform/OpenGL/OpenGLRendererAPI.cpp
    src/Platform/OpenGL/OpenGLVertexArray.cpp
    src/Platform/OpenGL/OpenGLContext.cpp

    ${IMGUI_SOURCES}
)

target_include_directories(civeeCore PUBLIC
    include
    src
    vendor/glad/include
    vendor/stb_image
    ${SDL2_INCLUDE_DIRS}
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Include Paths
# target_include_directories(${PROJECT_NAME} PRIVATE 
#     include
#     src
#     src/Platform/OpenGL

#     vendor
#     vendor/stb_image
#     vendor/glad/include
#     ${IMGUI_DIR}
#     ${IMGUI_DIR}/backends
#     ${SDL2_INCLUDE_DIRS} 
# )

target_link_libraries(civeeCore PUBLIC
    Glad
    SDL2::SDL2
    OpenGL::GL
    dl
)

# Link Libraries
# target_link_libraries(${PROJECT_NAME} PRIVATE
#     Glad
#     SDL2::SDL2
#     OpenGL::GL
#     dl
# )

add_executable(${PROJECT_NAME}
  src/main.cpp
  ${IMGUI_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  civeeCore
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
    COMMENT "Linking assets folder..."
)
