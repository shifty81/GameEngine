@echo off
echo ================================
echo 3D Game Engine - Build Script
echo ================================
echo.

REM ============================================================================
REM Auto-detect CMake from multiple sources
REM ============================================================================
set CMAKE_EXE=cmake
set CMAKE_FOUND=0

REM First, check if CMake is in PATH
where cmake >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    set CMAKE_FOUND=1
    echo Using CMake from PATH
    goto :cmake_ready
)

REM Check for portable CMake in tools directory (from setup.bat)
echo CMake not in PATH, checking for portable installation...
for /d %%d in ("tools\cmake\cmake-*") do (
    if exist "%%d\bin\cmake.exe" (
        set CMAKE_EXE=%%d\bin\cmake.exe
        set CMAKE_FOUND=1
        echo Found portable CMake: %%d\bin\cmake.exe
        goto :cmake_ready
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
        echo Found CMake at: %%~p
        goto :cmake_ready
    )
)

:cmake_not_found
if %CMAKE_FOUND%==0 (
    echo.
    echo ========================================================================
    echo ERROR: CMake not found!
    echo ========================================================================
    echo.
    echo CMake is required to build this project.
    echo.
    echo Quick Fix Option 1: Run setup.bat (Automated Setup - Recommended)
    echo    - Automatically downloads portable CMake
    echo    - No PATH configuration needed
    echo    - Run: setup.bat
    echo.
    echo Quick Fix Option 2: Add CMake to PATH manually
    echo    If CMake is installed at a custom location (e.g., C:\make\):
    echo    1. Press Windows + R, type: sysdm.cpl
    echo    2. Click "Environment Variables"
    echo    3. Under "System variables", select "Path", click "Edit"
    echo    4. Click "New" and add: C:\make\bin (or your cmake bin directory)
    echo    5. Click OK on all dialogs
    echo    6. Restart Command Prompt and run build.bat again
    echo.
    echo Quick Fix Option 3: Install CMake system-wide
    echo    1. Download from: https://cmake.org/download/
    echo    2. During installation, select "Add CMake to system PATH"
    echo    3. Restart Command Prompt and run build.bat again
    echo.
    echo ========================================================================
    echo.
    pause
    exit /b 1
)

:cmake_ready
"%CMAKE_EXE%" --version
echo.

REM Update submodules
echo Updating git submodules...
git submodule update --init --recursive
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to update git submodules
    pause
    exit /b 1
)

REM Create build directory
if not exist build mkdir build
cd build

REM Generate Visual Studio project
echo.
echo Generating Visual Studio project...
cmake ..
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ======================================================================
    echo ERROR: CMake configuration failed!
    echo ======================================================================
    echo.
    echo Common causes:
    echo.
    echo 1. Visual Studio C++ compiler not installed
    echo    - Open Visual Studio Installer
    echo    - Click "Modify" on your Visual Studio installation
    echo    - Select "Desktop development with C++" workload
    echo    - Click "Modify" to install
    echo.
    echo 2. Visual Studio not installed
    echo    - Download Visual Studio Community (free):
    echo      https://visualstudio.microsoft.com/downloads/
    echo    - During installation, select "Desktop development with C++"
    echo.
    echo 3. Missing CMake or Git
    echo    - Ensure both are installed and in your system PATH
    echo.
    echo After fixing the issue:
    echo    - Delete the 'build' folder
    echo    - Run build.bat again
    echo.
    echo For detailed troubleshooting, see README.md section:
    echo "Building the Engine" ^-^> "Troubleshooting"
    echo.
    echo ======================================================================
    echo.
    cd ..
    pause
    exit /b 1
)

REM Build the project
echo.
echo Building project...
cmake --build . --config Release
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Build failed
    cd ..
    pause
    exit /b 1
)

cd ..

echo.
echo ================================
echo Build completed successfully!
echo ================================
echo.
echo Executable location: build\bin\Release\GameEngine.exe
echo.
echo Run the game engine using: run.bat
echo.
pause
