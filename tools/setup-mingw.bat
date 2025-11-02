@echo off
REM ============================================================================
REM MinGW Setup Script for GameEngine
REM ============================================================================
REM This script automatically downloads and sets up MinGW-w64 compiler
REM so users can build the GameEngine without Visual Studio.
REM
REM MinGW-w64 is a free, open-source C++ compiler for Windows that provides
REM full C++20 support without requiring Visual Studio installation.
REM ============================================================================

setlocal enabledelayedexpansion

echo.
echo ============================================================================
echo       MinGW-w64 Setup for GameEngine
echo ============================================================================
echo.
echo This script will download and setup MinGW-w64 compiler (GCC for Windows)
echo so you can build the GameEngine without Visual Studio.
echo.

REM Set MinGW installation directory
set MINGW_DIR=%~dp0..\external\mingw64
set MINGW_BIN=%MINGW_DIR%\bin
set MINGW_VERSION=13.2.0
set MINGW_RELEASE=13.2.0-16.0.6-11.0.0-ucrt-r1
set MINGW_ARCH=x86_64
set MINGW_THREADS=posix
set MINGW_EXCEPTION=seh

REM MinGW-w64 download URL (using winlibs.com builds - latest stable GCC)
set MINGW_URL=https://github.com/brechtsanders/winlibs_mingw/releases/download/%MINGW_RELEASE%/winlibs-%MINGW_ARCH%-%MINGW_THREADS%-%MINGW_EXCEPTION%-gcc-%MINGW_VERSION%-mingw-w64ucrt-11.0.0-r1.zip
set MINGW_ARCHIVE=mingw64.zip

REM ============================================================================
REM Step 1: Check if MinGW is already installed
REM ============================================================================
echo [Step 1/4] Checking for existing MinGW installation...
echo.

if exist "%MINGW_BIN%\g++.exe" (
    echo [✓] MinGW-w64 is already installed at: %MINGW_DIR%
    echo.
    
    REM Test the compiler
    "%MINGW_BIN%\g++.exe" --version >nul 2>&1
    if %errorLevel% equ 0 (
        for /f "tokens=*" %%i in ('"%MINGW_BIN%\g++.exe" --version ^| findstr /C:"g++"') do (
            echo Compiler version: %%i
        )
        echo.
        echo MinGW is ready to use!
        echo.
        goto :add_to_path
    ) else (
        echo [!] MinGW installation appears corrupted. Reinstalling...
        echo.
        rmdir /s /q "%MINGW_DIR%" 2>nul
    )
) else (
    echo [i] MinGW not found. Will download and install.
    echo.
)

REM ============================================================================
REM Step 2: Check for required tools
REM ============================================================================
echo [Step 2/4] Checking for required tools...
echo.

REM Check for PowerShell (for downloading)
where powershell >nul 2>&1
if %errorLevel% neq 0 (
    echo [✗] PowerShell is not available
    echo     PowerShell is required for downloading files
    echo.
    pause
    exit /b 1
)
echo [✓] PowerShell found

REM Check for tar (Windows 10+ includes tar.exe)
where tar >nul 2>&1
if %errorLevel% neq 0 (
    echo [!] tar command not found
    echo     Will attempt to use PowerShell for extraction
    set USE_POWERSHELL_EXTRACT=1
) else (
    echo [✓] tar found
    set USE_POWERSHELL_EXTRACT=0
)
echo.

REM ============================================================================
REM Step 3: Download MinGW-w64
REM ============================================================================
echo [Step 3/4] Downloading MinGW-w64 (GCC %MINGW_VERSION%)...
echo.
echo This will download approximately 150 MB and may take a few minutes.
echo URL: %MINGW_URL%
echo.

REM Create external directory if it doesn't exist
if not exist "%~dp0..\external" mkdir "%~dp0..\external"

REM Download using PowerShell
echo Downloading from trusted source (GitHub/winlibs)...
echo.
REM SECURITY NOTE: This script downloads from GitHub (winlibs.com builds).
REM For enhanced security in production environments, consider:
REM   1. Verifying SHA256 checksum after download
REM   2. Using a local mirror/cache of the installer
REM   3. Scanning with antivirus before extraction
REM The winlibs.com MinGW-w64 builds are widely used and community-trusted.
powershell -Command "& {[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; $ProgressPreference = 'SilentlyContinue'; Invoke-WebRequest -Uri '%MINGW_URL%' -OutFile '%~dp0..\external\%MINGW_ARCHIVE%' -UseBasicParsing}"

if %errorLevel% neq 0 (
    echo.
    echo [✗] Failed to download MinGW
    echo.
    echo Please check your internet connection and try again.
    echo.
    echo Alternatively, you can manually download from:
    echo %MINGW_URL%
    echo.
    echo Extract to: %MINGW_DIR%
    echo.
    pause
    exit /b 1
)

echo [✓] Download complete
echo.

REM ============================================================================
REM Step 4: Extract MinGW-w64
REM ============================================================================
echo [Step 4/4] Extracting MinGW-w64...
echo.

if !USE_POWERSHELL_EXTRACT! equ 1 (
    REM Use PowerShell for extraction
    echo Using PowerShell for extraction...
    powershell -Command "& {Expand-Archive -Path '%~dp0..\external\%MINGW_ARCHIVE%' -DestinationPath '%~dp0..\external' -Force}"
) else (
    REM Use tar for extraction (faster)
    echo Using tar for extraction...
    tar -xf "%~dp0..\external\%MINGW_ARCHIVE%" -C "%~dp0..\external"
)

if %errorLevel% neq 0 (
    echo.
    echo [✗] Failed to extract MinGW
    echo.
    pause
    exit /b 1
)

REM Clean up archive
del "%~dp0..\external\%MINGW_ARCHIVE%" >nul 2>&1

echo [✓] Extraction complete
echo.

REM Verify installation
if not exist "%MINGW_BIN%\g++.exe" (
    echo [✗] Installation verification failed
    echo     g++.exe not found at: %MINGW_BIN%
    echo.
    pause
    exit /b 1
)

echo [✓] MinGW-w64 installed successfully!
echo.

REM ============================================================================
REM Add to PATH (for current session)
REM ============================================================================
:add_to_path
echo ============================================================================
echo Configuration
echo ============================================================================
echo.
echo MinGW location: %MINGW_DIR%
echo.
echo To use MinGW in this session, the bin directory has been added to PATH.
echo.
echo For permanent PATH configuration (optional):
echo   1. Press Windows + R, type: sysdm.cpl
echo   2. Click "Environment Variables"
echo   3. Under "User variables", select "Path", click "Edit"
echo   4. Click "New" and add: %MINGW_BIN%
echo   5. Click OK on all dialogs
echo.
echo ============================================================================
echo.

REM Add to PATH for current session
set PATH=%MINGW_BIN%;%PATH%

REM Verify compiler works
echo Testing compiler installation...
echo.
"%MINGW_BIN%\g++.exe" --version
if %errorLevel% neq 0 (
    echo.
    echo [✗] Compiler test failed
    pause
    exit /b 1
)

echo.
echo ============================================================================
echo                    SETUP COMPLETED SUCCESSFULLY!
echo ============================================================================
echo.
echo MinGW-w64 is now installed and ready to use!
echo.
echo You can now build the GameEngine without Visual Studio using:
echo   build-mingw.bat
echo.
echo Or manually:
echo   mkdir build
echo   cd build
echo   cmake -G "MinGW Makefiles" ..
echo   cmake --build . --config Release
echo.
echo ============================================================================
echo.

endlocal
exit /b 0
