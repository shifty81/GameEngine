@echo off
REM ============================================================================
REM GameEngine Setup Script - Automated Dependency Installation
REM ============================================================================
REM This script automatically sets up your development environment including:
REM - CMake (if not found)
REM - Git submodules
REM - Directory structure
REM
REM If you encounter issues, see TROUBLESHOOTING.md or WINDOWS_TROUBLESHOOTING.md
REM ============================================================================

echo ========================================
echo GameEngine - Automated Setup
echo ========================================
echo.
echo WARNING: Having issues? See TROUBLESHOOTING.md for help
echo.

REM Check if running as administrator (needed for some installations)
net session >nul 2>&1
if %errorLevel% == 0 (
    echo Running with administrator privileges...
) else (
    echo Note: Not running as administrator. Some installations may require elevation.
)
echo.

REM ============================================================================
REM Step 1: Check for Git
REM ============================================================================
echo [1/4] Checking for Git...
where git >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Git is not installed or not in PATH
    echo.
    echo Please install Git from: https://git-scm.com/download/win
    echo After installation, restart this script.
    echo.
    pause
    exit /b 1
)
echo    Git found: 
git --version
echo.

REM ============================================================================
REM Step 2: Check for CMake and offer to download if missing
REM ============================================================================
echo [2/4] Checking for CMake...
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo CMake not found in system PATH.
    echo.
    echo Checking common installation locations...
    
    REM Check common CMake installation paths
    set CMAKE_FOUND=0
    set CMAKE_PATHS=^
        "C:\Program Files\CMake\bin\cmake.exe" ^
        "C:\Program Files (x86)\CMake\bin\cmake.exe" ^
        "C:\make\bin\cmake.exe" ^
        "C:\cmake\bin\cmake.exe" ^
        "%LOCALAPPDATA%\Programs\CMake\bin\cmake.exe"
    
    for %%p in (%CMAKE_PATHS%) do (
        if exist %%p (
            echo    Found CMake at: %%~p
            set CMAKE_EXE=%%~p
            set CMAKE_FOUND=1
            goto :cmake_found
        )
    )
    
    :cmake_not_found
    if %CMAKE_FOUND%==0 (
        echo CMake not found in common locations.
        echo.
        echo ----------------------------------------
        echo CMake Installation Required
        echo ----------------------------------------
        echo.
        echo Option 1: Download Portable CMake (Recommended for this project)
        echo    - This script can download a portable CMake version
        echo    - No installation or PATH modification needed
        echo    - Stored in GameEngine\tools\cmake\
        echo.
        echo Option 2: Install CMake System-Wide
        echo    - Download from: https://cmake.org/download/
        echo    - During installation, select "Add CMake to system PATH"
        echo    - Restart this script after installation
        echo.
        choice /C 12 /N /M "Choose option (1=Portable, 2=Manual Install): "
        
        if errorlevel 2 goto :manual_cmake_install
        if errorlevel 1 goto :download_portable_cmake
    )
    
    :download_portable_cmake
    echo.
    echo Downloading portable CMake...
    echo.
    
    REM Create tools directory
    if not exist "tools" mkdir "tools"
    if not exist "tools\cmake" mkdir "tools\cmake"
    
    REM Download CMake portable (using PowerShell)
    echo Downloading CMake 3.28.1 (Windows x64 ZIP)...
    echo This may take a few minutes...
    echo.
    
    REM CMake 3.28.1 SHA256: 2c255d7c748ecbf14649f1e45f8a6c08b39a7fc19e046bcfc3a3bac5d12bba14
    set CMAKE_URL=https://github.com/Kitware/CMake/releases/download/v3.28.1/cmake-3.28.1-windows-x86_64.zip
    set CMAKE_SHA256=2c255d7c748ecbf14649f1e45f8a6c08b39a7fc19e046bcfc3a3bac5d12bba14
    
    powershell -Command "& {[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri '%CMAKE_URL%' -OutFile 'tools\cmake.zip' -UseBasicParsing}"
    
    if %ERRORLEVEL% NEQ 0 (
        echo ERROR: Failed to download CMake
        echo.
        echo Please manually download CMake from: https://cmake.org/download/
        echo Choose "Windows x64 ZIP" for portable installation
        echo Extract to: %CD%\tools\cmake\
        echo.
        pause
        exit /b 1
    )
    
    echo Verifying download integrity...
    powershell -Command "& {$hash = (Get-FileHash 'tools\cmake.zip' -Algorithm SHA256).Hash; if ($hash -ne '%CMAKE_SHA256%') { Write-Host 'ERROR: Checksum verification failed!' -ForegroundColor Red; exit 1 } else { Write-Host 'Checksum verified successfully' -ForegroundColor Green }}"
    
    if %ERRORLEVEL% NEQ 0 (
        echo ERROR: CMake download corrupted or tampered
        del "tools\cmake.zip"
        pause
        exit /b 1
    )
    
    echo Extracting CMake...
    powershell -Command "Expand-Archive -Path 'tools\cmake.zip' -DestinationPath 'tools\cmake' -Force"
    
    REM Find the extracted cmake.exe
    for /d %%d in ("tools\cmake\cmake-*") do set CMAKE_DIR=%%d
    set CMAKE_EXE=%CMAKE_DIR%\bin\cmake.exe
    
    if exist "%CMAKE_EXE%" (
        echo CMake extracted successfully!
        del "tools\cmake.zip"
        set CMAKE_FOUND=1
        goto :cmake_found
    ) else (
        echo ERROR: CMake extraction failed
        pause
        exit /b 1
    )
    
    :manual_cmake_install
    echo.
    echo Please install CMake manually:
    echo 1. Visit: https://cmake.org/download/
    echo 2. Download "Windows x64 Installer"
    echo 3. During installation, select "Add CMake to system PATH"
    echo 4. Restart this script after installation
    echo.
    pause
    exit /b 1
    
    :cmake_found
    if defined CMAKE_EXE (
        echo    Using CMake: %CMAKE_EXE%
        "%CMAKE_EXE%" --version
    ) else (
        echo    CMake found in PATH
        cmake --version
        set CMAKE_EXE=cmake
    )
) else (
    echo    CMake found in PATH
    cmake --version
    set CMAKE_EXE=cmake
)
echo.

REM ============================================================================
REM Step 3: Check for Visual Studio / C++ Compiler
REM ============================================================================
echo [3/4] Checking for C++ compiler...
echo.

REM Use the new comprehensive compiler detection script
if exist "tools\check-vs2022-compiler.bat" (
    call tools\check-vs2022-compiler.bat
    if %ERRORLEVEL% NEQ 0 (
        echo.
        echo The compiler detection script found issues with your Visual Studio installation.
        echo.
        choice /C YN /M "Would you like to automatically open the Visual Studio Installer to fix these issues?"
        if errorlevel 2 (
            choice /C YN /M "Continue setup anyway"
            if errorlevel 2 exit /b 1
        ) else (
            echo.
            echo Launching Visual Studio Installer with AutoFix...
            call tools\check-vs2022-compiler.bat -autofix
            echo.
            echo After modifying Visual Studio, please run this setup script again.
            pause
            exit /b 1
        )
    )
) else (
    REM Fallback to simple vswhere check if detection script is missing
    set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
    if exist "%VSWHERE%" (
        for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
            set VS_PATH=%%i
        )
    )
    
    if defined VS_PATH (
        echo    Visual Studio found: %VS_PATH%
        echo    C++ compiler: OK
    ) else (
        echo    WARNING: Visual Studio C++ compiler not detected
        echo.
        echo    To build this project, you need Visual Studio with C++ support.
        echo.
        echo    Download from: https://visualstudio.microsoft.com/downloads/
        echo    Select "Desktop development with C++" workload during installation
        echo.
        choice /C YN /M "Continue anyway"
        if errorlevel 2 exit /b 1
    )
)
echo.

REM ============================================================================
REM Step 4: Initialize Git Submodules
REM ============================================================================
echo [4/4] Initializing git submodules...
git submodule update --init --recursive
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to update git submodules
    pause
    exit /b 1
)
echo    Submodules initialized successfully
echo.

REM ============================================================================
REM Setup Complete
REM ============================================================================
echo ========================================
echo Setup Complete!
echo ========================================
echo.
echo Your development environment is ready.
echo.
echo Next steps:
echo   1. Run: build.bat (to compile the engine)
echo   2. Run: run.bat (to start the game engine)
echo.
echo If you encounter build issues, see:
echo   - TROUBLESHOOTING.md for cross-platform issues
echo   - WINDOWS_TROUBLESHOOTING.md for Windows-specific problems
echo.
pause
