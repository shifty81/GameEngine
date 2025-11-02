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
    echo ERROR: CMake configuration failed
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
