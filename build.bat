@echo off
echo ================================
echo 3D Game Engine - Build Script
echo ================================
echo.

REM Check if CMake is installed
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: CMake is not installed or not in PATH
    echo Please install CMake from https://cmake.org/download/
    pause
    exit /b 1
)

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
