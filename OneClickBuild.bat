@echo off
REM ============================================================================
REM One-Click Build System for GameEngine
REM ============================================================================
REM This script provides a complete one-click solution to build the GameEngine
REM on Windows. It automatically:
REM   1. Checks for all prerequisites (Git, CMake, Visual Studio)
REM   2. Offers to install missing prerequisites
REM   3. Initializes git submodules
REM   4. Configures the build with CMake
REM   5. Builds the project
REM   6. Reports success and provides next steps
REM
REM Usage: Simply double-click this file or run "OneClickBuild.bat"
REM ============================================================================

setlocal enabledelayedexpansion

REM Set colors for output (if supported)
color 0A

echo.
echo ============================================================================
echo       GameEngine - One-Click Build System for Windows
echo ============================================================================
echo.
echo This script will automatically set up and build the GameEngine project.
echo Please wait while we check your system...
echo.
echo ============================================================================
echo.

REM ============================================================================
REM Step 1: Check for Administrator Rights (Optional, for auto-install)
REM ============================================================================
net session >nul 2>&1
if %errorLevel% == 0 (
    set ADMIN_RIGHTS=1
    echo [✓] Running with administrator privileges
) else (
    set ADMIN_RIGHTS=0
    echo [i] Running without administrator privileges
    echo     ^(Some auto-install features may require administrator rights^)
)
echo.

REM ============================================================================
REM Step 2: Check for Git
REM ============================================================================
echo [Step 1/6] Checking for Git...
where git >nul 2>&1
if %errorLevel% neq 0 (
    echo [✗] Git is not installed or not in PATH
    echo.
    echo Git is required to download project dependencies.
    echo.
    echo Please download and install Git for Windows from:
    echo https://git-scm.com/download/win
    echo.
    echo After installation, restart this script.
    echo.
    pause
    exit /b 1
) else (
    for /f "tokens=*" %%i in ('git --version') do set GIT_VERSION=%%i
    echo [✓] Found: !GIT_VERSION!
)
echo.

REM ============================================================================
REM Step 3: Check for CMake
REM ============================================================================
echo [Step 2/6] Checking for CMake...
where cmake >nul 2>&1
if %errorLevel% neq 0 (
    echo [✗] CMake is not installed or not in PATH
    echo.
    echo CMake is required to generate build files.
    echo.
    echo Would you like to download a portable version of CMake? ^(Y/N^)
    set /p DOWNLOAD_CMAKE=
    
    if /i "!DOWNLOAD_CMAKE!"=="Y" (
        echo.
        echo Downloading portable CMake...
        echo This will download CMake to the current directory.
        echo.
        echo Please download CMake from: https://cmake.org/download/
        echo Choose "Windows x64 ZIP" for a portable version.
        echo Extract it and add the bin folder to your PATH, or
        echo place cmake.exe in this directory.
        echo.
        pause
    )
    
    echo.
    echo Please install CMake and restart this script.
    echo Download from: https://cmake.org/download/
    echo.
    pause
    exit /b 1
) else (
    for /f "tokens=*" %%i in ('cmake --version') do set CMAKE_VERSION=%%i
    echo [✓] Found: !CMAKE_VERSION!
)
echo.

REM ============================================================================
REM Step 4: Check for Visual Studio and C++ Compiler
REM ============================================================================
echo [Step 3/6] Checking for Visual Studio C++ Compiler...

REM Use our existing compiler detection tool if available
if exist "tools\check-vs2022-compiler.bat" (
    call tools\check-vs2022-compiler.bat -quiet
    if !errorLevel! neq 0 (
        echo.
        echo ========================================================================
        echo [✗] Visual Studio C++ compiler issues detected!
        echo ========================================================================
        echo.
        echo Would you like to run the diagnostic tool to fix this? ^(Y/N^)
        set /p RUN_DIAGNOSTIC=
        
        if /i "!RUN_DIAGNOSTIC!"=="Y" (
            echo.
            call tools\check-vs2022-compiler.bat -autofix
            if !errorLevel! neq 0 (
                echo.
                echo Please install Visual Studio with "Desktop development with C++" workload
                echo and restart this script.
                echo.
                pause
                exit /b 1
            )
        ) else (
            echo.
            echo Please install Visual Studio 2022 or 2019 with the
            echo "Desktop development with C++" workload.
            echo.
            echo Download from: https://visualstudio.microsoft.com/downloads/
            echo.
            pause
            exit /b 1
        )
    ) else (
        echo [✓] Visual Studio C++ compiler found
    )
) else (
    REM Fallback: Simple check for common VS installations
    set VS_FOUND=0
    
    if exist "C:\Program Files\Microsoft Visual Studio\2022" set VS_FOUND=1
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\2022" set VS_FOUND=1
    if exist "C:\Program Files\Microsoft Visual Studio\2019" set VS_FOUND=1
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019" set VS_FOUND=1
    
    if !VS_FOUND! == 0 (
        echo [✗] Visual Studio not found
        echo.
        echo Please install Visual Studio 2022 or 2019 Community Edition with
        echo the "Desktop development with C++" workload.
        echo.
        echo Download from: https://visualstudio.microsoft.com/downloads/
        echo.
        pause
        exit /b 1
    ) else (
        echo [✓] Visual Studio installation detected
    )
)
echo.

REM ============================================================================
REM Step 5: Initialize Git Submodules
REM ============================================================================
echo [Step 4/6] Initializing git submodules...
echo This will download GLFW, GLM, and Assimp libraries...
echo.

git submodule update --init --recursive
if %errorLevel% neq 0 (
    echo [✗] Failed to initialize git submodules
    echo.
    echo This could be a network issue. Please check your internet connection
    echo and try again.
    echo.
    pause
    exit /b 1
) else (
    echo [✓] Git submodules initialized successfully
)
echo.

REM ============================================================================
REM Step 6: Create Build Directory and Run CMake
REM ============================================================================
echo [Step 5/6] Configuring build with CMake...
echo.

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

cd build

REM Run CMake to generate Visual Studio project files
REM Note: For Visual Studio generators, CMAKE_BUILD_TYPE is not needed at configure time
REM The configuration (Debug/Release) is selected at build time
cmake ..
if %errorLevel% neq 0 (
    echo.
    echo ========================================================================
    echo [✗] CMake configuration failed
    echo ========================================================================
    echo.
    echo Common causes:
    echo   - Visual Studio C++ compiler not properly installed
    echo   - Missing dependencies
    echo   - CMake version too old (requires 3.20+)
    echo.
    echo Please review the error messages above and try again.
    echo.
    cd ..
    pause
    exit /b 1
) else (
    echo [✓] CMake configuration successful
)
echo.

REM ============================================================================
REM Step 7: Build the Project
REM ============================================================================
echo [Step 6/6] Building the project...
echo.
echo This may take 10-15 minutes on the first build as it compiles
echo all dependencies (Assimp, GLFW, etc.)
echo.
echo Please be patient...
echo.

cmake --build . --config Release
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
) else (
    echo [✓] Build completed successfully!
)

cd ..

echo.
echo ============================================================================
echo                     BUILD COMPLETED SUCCESSFULLY!
echo ============================================================================
echo.
echo The GameEngine has been built successfully!
echo.
echo Executable location: build\bin\Release\GameEngine.exe
echo.
echo To run the engine:
echo   1. Double-click: build\bin\Release\GameEngine.exe
echo   2. Or run: run.bat
echo.
echo To rebuild in the future:
echo   - Run: build.bat (faster, incremental builds)
echo   - Or run this script again
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
echo Press any key to exit...
pause >nul
