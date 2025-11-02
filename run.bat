@echo off
echo ================================
echo 3D Game Engine - Running...
echo ================================
echo.

REM Check if executable exists
if not exist build\bin\Release\GameEngine.exe (
    if not exist build\bin\Debug\GameEngine.exe (
        echo ERROR: GameEngine.exe not found!
        echo Please run build.bat first to compile the project.
        pause
        exit /b 1
    )
    set EXEC_PATH=build\bin\Debug\GameEngine.exe
) else (
    set EXEC_PATH=build\bin\Release\GameEngine.exe
)

REM Run the game engine
echo Starting Game Engine...
echo.
%EXEC_PATH%

echo.
echo Game Engine closed.
pause
