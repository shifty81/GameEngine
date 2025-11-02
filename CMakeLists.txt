cmake_minimum_required(VERSION 3.20)

# =============================================================================
# Ubuntu 24.04 LTS Game Engine Build Configuration
#
# Prerequisites:
#   sudo apt-get install build-essential cmake libgl1-mesa-dev libglu1-mesa-dev \
#       libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
#
# Quick Start:
#   ./setup.sh
#   ./build.sh
#   ./run.sh
# =============================================================================

project(GameEngine VERSION 1.0.0 LANGUAGES CXX)

# Use modern C++ standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find OpenGL
find_package(OpenGL REQUIRED)

# GLFW for window management
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

# GLAD for OpenGL function loading
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)
# Compile glad.c as C++ code (C++ compiler can handle C code)
set_source_files_properties(external/glad/src/glad.c PROPERTIES LANGUAGE CXX)

# GLM for mathematics
# Fix for CMake 3.27+ which no longer supports compatibility with CMake < 3.5
# GLM submodule requires CMake 3.2, so we tell CMake to allow it by setting the minimum policy version
if(NOT DEFINED CMAKE_POLICY_VERSION_MINIMUM)
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
endif()
add_subdirectory(external/glm)

# Assimp for 3D model loading
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
add_subdirectory(external/assimp)

# stb_image for texture loading (header-only)
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE external/stb)

# Engine source files
file(GLOB_RECURSE ENGINE_SOURCES 
    src/*.cpp
    src/*.h
)

# Create executable
add_executable(${PROJECT_NAME} ${ENGINE_SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/include
)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    OpenGL::GL
    glfw
    glad
    glm::glm
    assimp
    stb_image
)

# Linux-specific settings for Ubuntu 24.04
# Note: This applies to all Unix-like systems (including other Linux distributions)
# but the project is optimized for Ubuntu 24.04 LTS
if(UNIX AND NOT APPLE)
    # Enable threading support
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} Threads::Threads)
endif()
