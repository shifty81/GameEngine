@echo off
REM ============================================================================
REM GameEngine - MinGW Build Script (No Visual Studio Required!)
REM ============================================================================
REM This script builds the GameEngine using MinGW-w64 compiler instead of
REM Visual Studio. It provides a complete Visual Studio-free build solution.
REM
REM MinGW-w64 is a free, open-source C++ compiler for Windows that provides
REM full C++20 support. This script will automatically download and setup
REM MinGW if it's not already installed.
REM ============================================================================

setlocal enabledelayedexpansion

echo.
echo ============================================================================
echo   GameEngine - MinGW Build (Visual Studio Free!)
echo ============================================================================
echo.
echo This script builds the GameEngine using MinGW-w64 (GCC) compiler.
echo No Visual Studio installation required!
echo.
echo ============================================================================
echo.

REM Set MinGW paths
set MINGW_DIR=%~dp0external\mingw64
set MINGW_BIN=%MINGW_DIR%\bin

REM ============================================================================
REM Step 1: Check for MinGW installation
REM ============================================================================
echo [Step 1/6] Checking for MinGW-w64 compiler...
echo.

if exist "%MINGW_BIN%\g++.exe" (
    echo [✓] MinGW found at: %MINGW_DIR%
    
    REM Add to PATH for this session
    set PATH=%MINGW_BIN%;%PATH%
    
    REM Verify it works
    "%MINGW_BIN%\g++.exe" --version >nul 2>&1
    if !errorLevel! neq 0 (
        echo [✗] MinGW installation appears corrupted
        echo     Will attempt to reinstall...
        goto :install_mingw
    )
) else (
    echo [!] MinGW not found. Installing MinGW-w64...
    echo.
    goto :install_mingw
)

goto :check_cmake

:install_mingw
if exist "tools\setup-mingw.bat" (
    echo Running MinGW setup script...
    echo.
    call tools\setup-mingw.bat
    if !errorLevel! neq 0 (
        echo.
        echo [✗] MinGW setup failed
        echo.
        pause
        exit /b 1
    )
    
    REM Refresh PATH
    set PATH=%MINGW_BIN%;%PATH%
) else (
    echo.
    echo [✗] MinGW setup script not found: tools\setup-mingw.bat
    echo.
    echo Please download MinGW-w64 manually from:
    echo https://winlibs.com/ or https://www.mingw-w64.org/
    echo.
    echo Extract to: %MINGW_DIR%
    echo.
    pause
    exit /b 1
)

echo.

REM ============================================================================
REM Step 2: Check for CMake
REM ============================================================================
:check_cmake
echo [Step 2/6] Checking for CMake...
echo.

set CMAKE_EXE=cmake
set CMAKE_FOUND=0

REM Check if CMake is in PATH
where cmake >nul 2>&1
if %errorLevel% equ 0 (
    set CMAKE_FOUND=1
    echo [✓] CMake found in PATH
    goto :check_git
)

REM Check for portable CMake in tools directory
echo CMake not in PATH, checking for portable installation...
for /d %%d in ("tools\cmake\cmake-*") do (
    if exist "%%d\bin\cmake.exe" (
        set CMAKE_EXE=%%d\bin\cmake.exe
        set CMAKE_FOUND=1
        echo [✓] Found portable CMake: %%d\bin\cmake.exe
        goto :check_git
    )
)

REM Check common CMake installation paths
echo Checking common installation locations...
set CMAKE_PATHS=^
    "C:\Program Files\CMake\bin\cmake.exe" ^
    "C:\Program Files (x86)\CMake\bin\cmake.exe" ^
    "C:\make\bin\cmake.exe" ^
    "C:\cmake\bin\cmake.exe" ^
    "%LOCALAPPDATA%\Programs\CMake\bin\cmake.exe"

for %%p in (%CMAKE_PATHS%) do (
    if exist %%p (
        set CMAKE_EXE=%%~p
        set CMAKE_FOUND=1
        echo [✓] Found CMake at: %%~p
        goto :check_git
    )
)

:cmake_not_found
if %CMAKE_FOUND%==0 (
    echo [✗] CMake not found!
    echo.
    echo CMake is required to build this project.
    echo.
    echo Please download and install CMake from:
    echo https://cmake.org/download/
    echo.
    echo During installation, select "Add CMake to system PATH"
    echo.
    pause
    exit /b 1
)

REM ============================================================================
REM Step 3: Check for Git
REM ============================================================================
:check_git
echo.
echo [Step 3/6] Checking for Git...
echo.

where git >nul 2>&1
if %errorLevel% neq 0 (
    echo [✗] Git not found
    echo.
    echo Git is required to manage project dependencies.
    echo.
    echo Please download and install Git from:
    echo https://git-scm.com/download/win
    echo.
    pause
    exit /b 1
)

echo [✓] Git found
echo.

REM ============================================================================
REM Step 4: Update Git Submodules
REM ============================================================================
echo [Step 4/6] Updating git submodules...
echo This will download GLFW, GLM, and Assimp libraries...
echo.

git submodule update --init --recursive
if %errorLevel% neq 0 (
    echo [✗] Failed to update git submodules
    echo.
    pause
    exit /b 1
)

echo [✓] Git submodules updated
echo.

REM ============================================================================
REM Step 5: Configure CMake with MinGW
REM ============================================================================
echo [Step 5/6] Configuring CMake with MinGW Makefiles...
echo.

REM Create build directory
if not exist "build-mingw" mkdir build-mingw
cd build-mingw

REM Configure with MinGW Makefiles generator
echo Running CMake configuration...
echo Generator: MinGW Makefiles
echo Build type: Release
echo Compiler: MinGW-w64 GCC
echo.

"%CMAKE_EXE%" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..

if %errorLevel% neq 0 (
    echo.
    echo ========================================================================
    echo [✗] CMake configuration failed
    echo ========================================================================
    echo.
    echo This could be due to:
    echo   - Missing dependencies
    echo   - MinGW compiler not properly detected
    echo   - CMake version too old (requires 3.20+)
    echo.
    echo Please review the error messages above.
    echo.
    cd ..
    pause
    exit /b 1
)

echo.
echo [✓] CMake configuration successful
echo.

REM ============================================================================
REM Step 6: Build the Project
REM ============================================================================
echo [Step 6/6] Building the project with MinGW...
echo.
echo This may take 10-15 minutes on the first build as it compiles
echo all dependencies (Assimp, GLFW, etc.)
echo.
echo Please be patient...
echo.

REM Build using MinGW make
"%CMAKE_EXE%" --build . --config Release -j 8

if %errorLevel% neq 0 (
    echo.
    echo ========================================================================
    echo [✗] Build failed
    echo ========================================================================
    echo.
    echo Please review the error messages above.
    echo.
    echo For troubleshooting help, see TROUBLESHOOTING.md
    echo.
    cd ..
    pause
    exit /b 1
)

cd ..

echo.
echo ============================================================================
echo                    BUILD COMPLETED SUCCESSFULLY!
echo ============================================================================
echo.
echo The GameEngine has been built successfully with MinGW!
echo No Visual Studio required!
echo.
echo Executable location: build-mingw\bin\GameEngine.exe
echo.
echo To run the engine:
echo   build-mingw\bin\GameEngine.exe
echo.
echo To rebuild in the future:
echo   build-mingw.bat
echo.
echo Controls:
echo   - WASD: Move camera
echo   - Mouse: Look around
echo   - Space: Move up
echo   - Left Shift: Move down
echo   - F: Toggle wireframe
echo   - C: Toggle cell shading
echo   - ` (grave): Toggle debug console
echo   - ESC: Exit
echo.
echo ============================================================================
echo.
pause
