# Build script for Game Engine
# Builds both C++ Core and C# Editor

param(
    [string]$Configuration = "Release",
    [string]$Platform = "x64"
)

Write-Host "================================" -ForegroundColor Cyan
Write-Host "  Game Engine Build Script" -ForegroundColor Cyan
Write-Host "  Configuration: $Configuration" -ForegroundColor Cyan
Write-Host "  Platform: $Platform" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan
Write-Host ""

# Check for .NET 9
Write-Host "Checking .NET SDK version..." -ForegroundColor Yellow
$dotnetVersion = & dotnet --version 2>$null
if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: .NET SDK not found!" -ForegroundColor Red
    Write-Host "Please install .NET 9 SDK from: https://dotnet.microsoft.com/download/dotnet/9.0" -ForegroundColor Red
    exit 1
}
Write-Host "Found .NET SDK: $dotnetVersion" -ForegroundColor Green
Write-Host ""

# Check for MSBuild (Visual Studio)
Write-Host "Checking for MSBuild..." -ForegroundColor Yellow
$msbuildPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" `
    -latest -requires Microsoft.Component.MSBuild `
    -find MSBuild\**\Bin\MSBuild.exe 2>$null | Select-Object -First 1

if (-not $msbuildPath) {
    Write-Host "WARNING: MSBuild not found. Attempting to use dotnet build..." -ForegroundColor Yellow
} else {
    Write-Host "Found MSBuild: $msbuildPath" -ForegroundColor Green
}
Write-Host ""

# Build C++ Core
Write-Host "Building C++ Core Engine..." -ForegroundColor Yellow
if ($msbuildPath) {
    & $msbuildPath src\GameEngine.Core\GameEngine.Core.vcxproj `
        /p:Configuration=$Configuration `
        /p:Platform=$Platform `
        /nologo /v:minimal
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "ERROR: C++ Core build failed!" -ForegroundColor Red
        exit 1
    }
} else {
    Write-Host "Skipping C++ build (MSBuild not available)" -ForegroundColor Yellow
    Write-Host "Please build GameEngine.Core manually in Visual Studio" -ForegroundColor Yellow
}
Write-Host "C++ Core build completed" -ForegroundColor Green
Write-Host ""

# Build C# Editor
Write-Host "Building C# Editor..." -ForegroundColor Yellow
& dotnet build src\GameEngine.Editor\GameEngine.Editor.csproj `
    -c $Configuration `
    --nologo -v minimal

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: C# Editor build failed!" -ForegroundColor Red
    exit 1
}
Write-Host "C# Editor build completed" -ForegroundColor Green
Write-Host ""

Write-Host "================================" -ForegroundColor Cyan
Write-Host "  Build Successful!" -ForegroundColor Green
Write-Host "  Run with: .\run.ps1" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan
