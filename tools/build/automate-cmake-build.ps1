<#!powershell
<#
Automate CMake configure + build using Visual Studio developer environment.

Features:
 - Non-interactive: runs vcvars then cmake configure and optional build in the same cmd session.
 - Interactive: opens a new Developer Command Prompt window (vcvars invoked) and cd's to repo root for manual debugging.
 - Prefill: -PrefillCMake or -PrefillCommand to present a prefilled command in the interactive window and optionally run it when Enter is pressed.
 - Environment logging: -LogEnv will dump the initial environment into a timestamped file (LIB/INCLUDE/PATH/WindowsSdkDir/where cl/where link).
 - Examples and safe defaults for common use-cases.

Usage (run from repo root in PowerShell):
  # Interactive with prefilled configure command and environment logging
  .\automate-cmake-build.ps1 -Interactive -PrefillCMake -LogEnv

  # Non-interactive: clean, configure and build (Debug x64)
  .\automate-cmake-build.ps1 -Clean

  # Configure only (no build)
  .\automate-cmake-build.ps1 -NoBuild -Config Release

Notes:
 - The script finds Visual Studio via vswhere if available, and uses the appropriate vcvars batch.
 - The script runs vcvars and subsequent cmake commands in a single cmd session so environment changes persist.
 - For interactive debugging, the script opens a new cmd window and optionally pre-fills a command. Press Enter in that window to run the prefilled command.
 - The environment log contains the output of 'set', 'where cl', and 'where link' for quick troubleshooting.

Parameters:
  -Arch           : "x64" (default) or "x86"
  -Generator      : CMake generator string (default "Visual Studio 17 2022")
  -BuildDir       : build output directory (default "build")
  -Clean          : remove build dir before configure
  -NoBuild        : do not perform the cmake --build step (only configure)
  -Config         : configuration to build (Debug default)
  -VsInstallPath  : optional Visual Studio install path (will try vswhere if not provided)
  -Interactive    : open a new Developer Command Prompt window (vcvars invoked)
  -PrefillCommand : a custom command string to prefill inside the interactive window
  -PrefillCMake   : prefill a default cmake configure command
  -LogEnv         : dump the initial environment in the interactive or non-interactive cmd session to a file
  -EnvLogPath     : custom path for the environment dump file

#>

[CmdletBinding()]
param(
    [ValidateSet("x64","x86")]
    [string]$Arch = "x64",

    [string]$Generator = "Visual Studio 17 2022",

    [string]$BuildDir = "build",

    [switch]$Clean,

    [switch]$NoBuild,

    [string]$Config = "Debug",

    [string]$VsInstallPath,

    [switch]$Interactive,

    [string]$PrefillCommand,

    [switch]$PrefillCMake,

    [switch]$LogEnv,

    [string]$EnvLogPath
)

function Find-VSWhere {
    $programFilesX86 = [System.Environment]::GetFolderPath([System.Environment+SpecialFolder]::ProgramFilesX86)
    $p = Join-Path -Path $programFilesX86 -ChildPath "Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $p) { return $p }
    $wg = Get-Command vswhere -ErrorAction SilentlyContinue
    if ($wg) { return $wg.Source }
    return $null
}

function Get-VsInstallPath {
    param($vswhere)
    try {
        $out = & $vswhere -latest -products * -property installationPath 2>$null
        if ($out) { return $out.Trim() }
    } catch { }
    return $null
}

$programFilesX86 = [System.Environment]::GetFolderPath([System.Environment+SpecialFolder]::ProgramFilesX86)

if (-not $VsInstallPath) {
    $vswhere = Find-VSWhere
    if (-not $vswhere) {
        Write-Error "vswhere.exe not found. Ensure Visual Studio Installer is installed or pass -VsInstallPath."
        exit 2
    }
    $VsInstallPath = Get-VsInstallPath -vswhere $vswhere
    if (-not $VsInstallPath) {
        Write-Error "No Visual Studio installation found by vswhere. Open Visual Studio Installer and ensure you have an installed instance."
        exit 3
    }
}

$vcvarsName = "vcvars$($Arch).bat"
$vcvarsPath = Join-Path -Path $VsInstallPath -ChildPath "VC\Auxiliary\Build\$vcvarsName"
$vcvarsArg = $null
if (-not (Test-Path $vcvarsPath)) {
    $vcvarsall = Join-Path -Path $VsInstallPath -ChildPath "VC\Auxiliary\Build\vcvarsall.bat"
    if (Test-Path $vcvarsall) {
        $vcvarsPath = $vcvarsall
        $vcvarsArg = if ($Arch -eq "x64") { "amd64" } else { "x86" }
    } else {
        Write-Error "vcvars batch not found at expected path: $vcvarsPath"
        Write-Error "Expected Visual Studio path: $VsInstallPath"
        exit 4
    }
}

$repoRoot = (Get-Location).Path

# Determine prefill command if requested
$prefillCmdFinal = $null
if ($PrefillCommand) {
    $prefillCmdFinal = $PrefillCommand.Trim()
} elseif ($PrefillCMake) {
    $escapedGenerator = $Generator -replace '"','\\"'
    $prefillCmdFinal = "cmake -S . -B `"$BuildDir`" -G `"$escapedGenerator`" -A $Arch"
}

# Prepare environment log path (timestamped default if not provided)
if ($LogEnv) {
    if (-not $EnvLogPath -or $EnvLogPath.Trim() -eq "") {
        $timestamp = (Get-Date).ToString('yyyyMMdd-HHmmss')
        $logDir = Join-Path -Path $repoRoot -ChildPath "build_env_logs"
        if (-not (Test-Path $logDir)) {
            New-Item -Path $logDir -ItemType Directory -Force | Out-Null
        }
        $EnvLogPath = Join-Path -Path $logDir -ChildPath ("env_$timestamp.txt")
    } else {
        $envLogDir = Split-Path -Path $EnvLogPath -Parent
        if ($envLogDir -and -not (Test-Path $envLogDir)) {
            New-Item -Path $envLogDir -ItemType Directory -Force | Out-Null
        }
    }
}

function Escape-For-Echo($s) {
    if (-not $s) { return "" }
    return $s -replace '"', '\\"'
}

if ($Interactive) {
    if ($vcvarsArg) {
        $vcvarsCall = "call `"$vcvarsPath`" $vcvarsArg"
    } else {
        $vcvarsCall = "call `"$vcvarsPath`""
    }

    $interactiveCmd = "$vcvarsCall & cd /d `"$repoRoot`" & echo. & echo Developer prompt initialized for repository: `"$repoRoot`" & echo."

    if ($LogEnv) {
        $interactiveCmd += " & echo Dumping initial environment to `"$EnvLogPath`" & if not exist `"$([IO.Path]::GetDirectoryName($EnvLogPath))`" mkdir `"$([IO.Path]::GetDirectoryName($EnvLogPath))`" & set > `"$EnvLogPath`" 2>&1 & echo. >> `"$EnvLogPath`" & echo ===== where cl ===== >> `"$EnvLogPath`" 2>&1 & where cl >> `"$EnvLogPath`" 2>&1 & echo. >> `"$EnvLogPath`" & echo ===== where link ===== >> `"$EnvLogPath`" 2>&1 & where link >> `"$EnvLogPath`" 2>&1 & echo Environment dumped to `"$EnvLogPath`" & echo."
    }

    if ($prefillCmdFinal) {
        $prefillForEcho = Escape-For-Echo $prefillCmdFinal
        $interactiveCmd += " & echo Prefilled command: & echo   $prefillForEcho & echo."
        $interactiveCmd += " & set /p _run=Press Enter to run the prefilled command, or type N then Enter to cancel: "
        $interactiveCmd += " & if "%_run%"=="" ( call $prefillCmdFinal ) else ( echo Cancelled )"
        $interactiveCmd += " & echo. & echo The environment remains initialized. Type exit to close this window."
    } else {
        $interactiveCmd += " & echo No prefilled command requested. You can now run your CMake commands interactively."
        if ($LogEnv) {
            $interactiveCmd += " & echo Env log saved to `"$EnvLogPath`"."
        }
    }

    try {
        Start-Process -FilePath "cmd.exe" -ArgumentList @("/k", $interactiveCmd) -WindowStyle Normal
        Write-Host "Opened new Developer Command Prompt window (vcvars loaded) and changed directory to $repoRoot." -ForegroundColor Green
        if ($LogEnv) {
            Write-Host "The initial environment will be dumped to: $EnvLogPath" -ForegroundColor Green
        }
        if ($prefillCmdFinal) {
            Write-Host "A prefilled command was shown in the new window; press Enter there to run it (or type N then Enter to cancel)." -ForegroundColor Green
        } else {
            Write-Host "Run your CMake commands in the new window." -ForegroundColor Green
        }
        exit 0
    } catch {
        Write-Error "Failed to open interactive cmd window: $_"
        exit 5
    }
}

# Non-interactive flow
$escapedVcvars = '"' + $vcvarsPath + '"'
$cmdParts = @()

if ($vcvarsArg) {
    $cmdParts += "call $escapedVcvars $vcvarsArg"
} else {
    $cmdParts += "call $escapedVcvars"
}

if ($LogEnv) {
    $cmdParts += "if not exist `"$([IO.Path]::GetDirectoryName($EnvLogPath))`" mkdir `"$([IO.Path]::GetDirectoryName($EnvLogPath))`""
    $cmdParts += "set > `"$EnvLogPath`" 2>&1"
    $cmdParts += "echo. >> `"$EnvLogPath`""
    $cmdParts += "echo ===== where cl ===== >> `"$EnvLogPath`" 2>&1"
    $cmdParts += "where cl >> `"$EnvLogPath`" 2>&1"
    $cmdParts += "echo. >> `"$EnvLogPath`""
    $cmdParts += "echo ===== where link ===== >> `"$EnvLogPath`" 2>&1"
    $cmdParts += "where link >> `"$EnvLogPath`" 2>&1"
    $cmdParts += "echo Environment dumped to `"$EnvLogPath`""
}

$cmdParts += "cd /d `"$repoRoot`""

if ($Clean) {
    $fullBuildDir = Join-Path -Path $repoRoot -ChildPath $BuildDir
    $cmdParts += "if exist `"$fullBuildDir`" rd /s /q `"$fullBuildDir`""
}

$escapedGenerator = $Generator -replace '"','\\"'
$configureCmd = "cmake -S . -B `"$BuildDir`" -G `"$escapedGenerator`" -A $Arch"
$cmdParts += $configureCmd

if (-not $NoBuild) {
    $buildCmd = "cmake --build `"$BuildDir`" --config $Config"
    $cmdParts += $buildCmd
}

$cmd = [string]::Join(" && ", $cmdParts)

Write-Host "Running (inside a single cmd session):`n$cmd`n" -ForegroundColor Cyan

$startInfo = @{
    FilePath = "cmd.exe"
    ArgumentList = @("/c", $cmd)
    NoNewWindow = $true
    Wait = $true
}

try {
    $proc = Start-Process @startInfo -PassThru
    if ($proc.ExitCode -ne 0) {
        Write-Error "cmd.exe exited with code $($proc.ExitCode). See output above for details."
        exit $proc.ExitCode
    }
} catch {
    Write-Error "Failed to start cmd.exe: $_"
    exit 10
}

if ($LogEnv) {
    Write-Host "Environment log written to: $EnvLogPath" -ForegroundColor Green
}

Write-Host "`nDone. If the build failed, re-run with -NoBuild to only configure and inspect CMake logs in $BuildDir\CMakeFiles." -ForegroundColor Green

#>