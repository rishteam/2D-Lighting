cmake_minimum_required(VERSION 3.16)
project(2D-Lighting)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(deps/glad)

add_executable(2D-Lighting

        main.cpp
        src/Game.cpp
        src/Light.cpp
        src/Renderer.cpp
        src/Block.cpp
        src/Shader.cpp
        src/VertexArray.cpp
        src/buffer.cpp
        src/Framebuffer.cpp
        src/OrthographicCamera.cpp
        src/OrthographicCameraController.cpp)
include_directories(./include)
target_precompile_headers(2D-Lighting PUBLIC include/pch.h)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(2D-Lighting
        sfml-system sfml-window sfml-graphics sfml-network sfml-audio
        )
find_package(ImGui-SFML REQUIRED)
if(NOT ImGui-SFML_FOUND)
    message(FATAL_ERROR "ImGui-SFML not found")
endif()
target_link_libraries(2D-Lighting
        ImGui-SFML::ImGui-SFML
        )

find_package(OpenGL REQUIRED)
if(NOT OpenGL_FOUND)
    message(FATAL_ERROR "OpenGL not found")
endif()
target_include_directories(2D-Lighting
        PUBLIC ${OPENGL_INCLUDE_DIRS}
        )

target_link_libraries(2D-Lighting glad::glad)