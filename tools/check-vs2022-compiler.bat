@echo off
REM ============================================================================
REM Visual Studio 2022 Compiler Check Wrapper
REM ============================================================================
REM This batch file wrapper makes it easy to call the PowerShell detection script
REM from other batch files in the project.
REM ============================================================================

setlocal

REM Parse command line arguments
set AUTO_FIX=
set QUIET_MODE=

:parse_args
if "%~1"=="" goto end_parse
if /i "%~1"=="-autofix" set AUTO_FIX=-AutoFix
if /i "%~1"=="--autofix" set AUTO_FIX=-AutoFix
if /i "%~1"=="-quiet" set QUIET_MODE=-Quiet
if /i "%~1"=="--quiet" set QUIET_MODE=-Quiet
shift
goto parse_args
:end_parse

REM Check if PowerShell script exists
if not exist "%~dp0check-vs2022-compiler.ps1" (
    echo ERROR: check-vs2022-compiler.ps1 not found in tools directory
    exit /b 1
)

REM Run the PowerShell script
echo Running Visual Studio 2022 compiler detection...
echo.

powershell -ExecutionPolicy Bypass -File "%~dp0check-vs2022-compiler.ps1" %AUTO_FIX% %QUIET_MODE%

REM Capture the exit code
set EXIT_CODE=%ERRORLEVEL%

REM Return the same exit code
exit /b %EXIT_CODE%
