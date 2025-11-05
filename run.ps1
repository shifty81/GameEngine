# Run script for Game Engine Editor

param(
    [string]$Configuration = "Release"
)

Write-Host "================================" -ForegroundColor Cyan
Write-Host "  Launching Game Engine Editor" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan
Write-Host ""

# Check if built
$editorPath = "src\GameEngine.Editor\bin\$Configuration\net9.0-windows\GameEngine.Editor.exe"
if (-not (Test-Path $editorPath)) {
    Write-Host "ERROR: Editor not built!" -ForegroundColor Red
    Write-Host "Please run .\build.ps1 first" -ForegroundColor Yellow
    exit 1
}

# Check if Core DLL exists
$coreDllPath = "bin\$Configuration\x64\GameEngine.Core.dll"
if (-not (Test-Path $coreDllPath)) {
    Write-Host "WARNING: GameEngine.Core.dll not found at expected location" -ForegroundColor Yellow
    Write-Host "Path: $coreDllPath" -ForegroundColor Yellow
    Write-Host "Attempting to continue..." -ForegroundColor Yellow
    Write-Host ""
}

Write-Host "Starting editor..." -ForegroundColor Green
& dotnet run --project src\GameEngine.Editor\GameEngine.Editor.csproj -c $Configuration --no-build
