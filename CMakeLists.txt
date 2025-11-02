cmake_minimum_required(VERSION 3.20)

# =============================================================================
# IMPORTANT: If you get "No CMAKE_CXX_COMPILER could be found" errors:
#
# Windows Users:
#   You need to install Visual Studio with the "Desktop development with C++" 
#   workload. This installs the MSVC compiler (cl.exe).
#   
#   Supported Versions:
#   - Visual Studio 2022 (v17.8 through v17.14 and newer) - Recommended
#   - Visual Studio 2019 (v16.11 and newer) - Alternative
#   
#   Steps to fix:
#   1. Download Visual Studio Community (free):
#      https://visualstudio.microsoft.com/downloads/
#   2. Run the installer
#   3. Select "Desktop development with C++" workload
#   4. Click Install/Modify
#   5. After installation, delete the 'build' folder
#   6. Run 'build.bat' again
#
# Linux Users:
#   sudo apt-get install build-essential cmake libgl1-mesa-dev
#
# macOS Users:
#   xcode-select --install
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
# Fix for CMake 3.31+ which no longer supports compatibility with CMake < 3.5
# GLM submodule requires CMake 3.2, so we tell CMake to allow it by setting the minimum policy version
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "Minimum CMake policy version" FORCE)
add_subdirectory(external/glm)

# Assimp for 3D model loading
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)
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

# Windows-specific settings
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE OFF
    )
endif()

# Set startup project for Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
