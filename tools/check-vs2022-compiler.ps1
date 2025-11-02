# ============================================================================
# Visual Studio 2022 Compiler Detection and Fix Script
# ============================================================================
# This script detects Visual Studio 2022 installation and C++ compiler status
# and provides automated fixes when the compiler is not found.
# ============================================================================

param(
    [switch]$AutoFix = $false,
    [switch]$Quiet = $false
)

$ErrorActionPreference = "Stop"

# ANSI color codes for better output
$Red = "`e[31m"
$Green = "`e[32m"
$Yellow = "`e[33m"
$Blue = "`e[34m"
$Magenta = "`e[35m"
$Cyan = "`e[36m"
$Reset = "`e[0m"

function Write-ColorOutput {
    param(
        [string]$Message,
        [string]$Color = $Reset,
        [switch]$NoNewline
    )
    if (-not $Quiet) {
        if ($NoNewline) {
            Write-Host "${Color}${Message}${Reset}" -NoNewline
        } else {
            Write-Host "${Color}${Message}${Reset}"
        }
    }
}

function Write-Header {
    param([string]$Title)
    if (-not $Quiet) {
        Write-Host ""
        Write-ColorOutput "============================================================================" $Cyan
        Write-ColorOutput " $Title" $Cyan
        Write-ColorOutput "============================================================================" $Cyan
    }
}

function Write-Success {
    param([string]$Message)
    Write-ColorOutput "✓ $Message" $Green
}

function Write-Error {
    param([string]$Message)
    Write-ColorOutput "✗ $Message" $Red
}

function Write-Warning {
    param([string]$Message)
    Write-ColorOutput "⚠ $Message" $Yellow
}

function Write-Info {
    param([string]$Message)
    Write-ColorOutput "ℹ $Message" $Blue
}

# ============================================================================
# Check if running with appropriate permissions
# ============================================================================
Write-Header "Visual Studio 2022 C++ Compiler Detection"

$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
if ($isAdmin) {
    Write-Info "Running with Administrator privileges"
} else {
    Write-Warning "Not running as Administrator (may be needed for fixes)"
}

# ============================================================================
# Step 1: Find vswhere.exe
# ============================================================================
Write-Header "Step 1: Locating Visual Studio Installer Tools"

$vswherePath = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"

if (-not (Test-Path $vswherePath)) {
    Write-Error "vswhere.exe not found at: $vswherePath"
    Write-Info "This usually means Visual Studio is not installed at all."
    Write-Host ""
    Write-ColorOutput "Visual Studio 2022 is required to build this project." $Yellow
    Write-Host ""
    
    if ($AutoFix) {
        Write-Info "AutoFix enabled - Opening Visual Studio download page..."
        Start-Process "https://visualstudio.microsoft.com/downloads/"
        Write-Host ""
        Write-Info "Please download and install Visual Studio 2022 Community Edition"
        Write-Info "During installation, select 'Desktop development with C++' workload"
        exit 1
    } else {
        Write-Host ""
        Write-ColorOutput "Quick Fix:" $Cyan
        Write-Host "  1. Download Visual Studio 2022 Community (free): https://visualstudio.microsoft.com/downloads/"
        Write-Host "  2. Run the installer"
        Write-Host "  3. Select 'Desktop development with C++' workload"
        Write-Host "  4. Click Install"
        Write-Host ""
        Write-ColorOutput "Or run this script with -AutoFix to open the download page automatically" $Cyan
        exit 1
    }
}

Write-Success "vswhere.exe found at: $vswherePath"

# ============================================================================
# Step 2: Find Visual Studio 2022 Installation
# ============================================================================
Write-Header "Step 2: Detecting Visual Studio 2022 Installation"

$vsInstances = & $vswherePath -version "[17.0,18.0)" -products * -format json | ConvertFrom-Json

if ($vsInstances.Count -eq 0) {
    Write-Error "Visual Studio 2022 not found"
    Write-Info "vswhere.exe exists but no VS 2022 installation detected"
    Write-Host ""
    
    if ($AutoFix) {
        Write-Info "AutoFix enabled - Opening Visual Studio download page..."
        Start-Process "https://visualstudio.microsoft.com/downloads/"
        Write-Host ""
        Write-Info "Please download and install Visual Studio 2022 Community Edition"
        Write-Info "During installation, select 'Desktop development with C++' workload"
        exit 1
    } else {
        Write-Host ""
        Write-ColorOutput "Quick Fix:" $Cyan
        Write-Host "  1. Download Visual Studio 2022 Community (free): https://visualstudio.microsoft.com/downloads/"
        Write-Host "  2. Run the installer"
        Write-Host "  3. Select 'Desktop development with C++' workload"
        Write-Host "  4. Click Install"
        Write-Host ""
        Write-ColorOutput "Or run this script with -AutoFix to open the download page automatically" $Cyan
        exit 1
    }
}

$vsInstance = $vsInstances[0]
$vsPath = $vsInstance.installationPath
$vsVersion = $vsInstance.installationVersion
$vsDisplayName = $vsInstance.displayName

Write-Success "Found: $vsDisplayName"
Write-Info "Version: $vsVersion"
Write-Info "Path: $vsPath"

# ============================================================================
# Step 3: Check for C++ Workload Components
# ============================================================================
Write-Header "Step 3: Checking C++ Compiler Components"

# Check for required components
$requiredComponents = @(
    @{
        Id = "Microsoft.VisualStudio.Component.VC.Tools.x86.x64"
        Name = "MSVC v143 - VS 2022 C++ x64/x86 build tools (Latest)"
    },
    @{
        Id = "Microsoft.VisualStudio.Component.Windows11SDK"
        Name = "Windows SDK (10.0 or 11.0)"
        Alternative = "Microsoft.VisualStudio.Component.Windows10SDK"
    }
)

$missingComponents = @()
$installedComponents = $vsInstance.packages | Where-Object { $_.type -eq "Component" }

foreach ($component in $requiredComponents) {
    $found = $installedComponents | Where-Object { $_.id -eq $component.Id }
    
    if ($found) {
        Write-Success "$($component.Name)"
    } else {
        # Check for alternative component if specified
        if ($component.Alternative) {
            $altFound = $installedComponents | Where-Object { $_.id -like "$($component.Alternative)*" }
            if ($altFound) {
                Write-Success "$($component.Name) (using alternative: $($altFound.id))"
                continue
            }
        }
        
        Write-Error "$($component.Name) - NOT INSTALLED"
        $missingComponents += $component
    }
}

# ============================================================================
# Step 4: Check for MSVC Compiler Executable
# ============================================================================
Write-Header "Step 4: Verifying MSVC Compiler (cl.exe)"

$vcToolsPath = Join-Path $vsPath "VC\Tools\MSVC"
if (Test-Path $vcToolsPath) {
    $msvcVersions = Get-ChildItem $vcToolsPath -Directory | Sort-Object Name -Descending
    
    if ($msvcVersions.Count -gt 0) {
        $latestMsvc = $msvcVersions[0]
        $clPath = Join-Path $latestMsvc.FullName "bin\Hostx64\x64\cl.exe"
        
        if (Test-Path $clPath) {
            Write-Success "MSVC Compiler found: $clPath"
            
            # Get compiler version
            $clOutput = & $clPath 2>&1 | Select-Object -First 1
            Write-Info "Compiler: $clOutput"
        } else {
            Write-Error "cl.exe not found at expected path: $clPath"
            $missingComponents += @{
                Id = "Microsoft.VisualStudio.Component.VC.Tools.x86.x64"
                Name = "MSVC C++ Compiler (cl.exe not found)"
            }
        }
    } else {
        Write-Error "No MSVC versions found in: $vcToolsPath"
        $missingComponents += @{
            Id = "Microsoft.VisualStudio.Component.VC.Tools.x86.x64"
            Name = "MSVC C++ Compiler (no versions installed)"
        }
    }
} else {
    Write-Error "VC Tools directory not found: $vcToolsPath"
    $missingComponents += @{
        Id = "Microsoft.VisualStudio.Component.VC.Tools.x86.x64"
        Name = "MSVC C++ Build Tools (directory missing)"
    }
}

# ============================================================================
# Step 5: Check for Windows SDK
# ============================================================================
Write-Header "Step 5: Verifying Windows SDK"

# Check for Windows Kits in standard and environment variable locations
$windowsKitsFound = $false
$sdkPaths = @(
    "C:\Program Files (x86)\Windows Kits\10\Include",
    "C:\Program Files (x86)\Windows Kits\11\Include"
)

# Add environment variable path if it exists
if ($env:WindowsSdkDir) {
    $sdkPaths += Join-Path $env:WindowsSdkDir "Include"
}

foreach ($kitsPath in $sdkPaths) {
    if (Test-Path $kitsPath) {
        $sdkVersions = Get-ChildItem $kitsPath -Directory | Sort-Object Name -Descending
        if ($sdkVersions.Count -gt 0) {
            Write-Success "Windows SDK found: $($sdkVersions[0].Name) at $kitsPath"
            $windowsKitsFound = $true
            break
        }
    }
}

if (-not $windowsKitsFound) {
    Write-Warning "Windows SDK not found in standard locations"
    Write-Info "SDK may be installed in a custom location or missing"
}

# ============================================================================
# Step 6: Summary and Fix Options
# ============================================================================
Write-Header "Detection Summary"

if ($missingComponents.Count -eq 0) {
    Write-Host ""
    Write-ColorOutput "════════════════════════════════════════════════════════════════════════" $Green
    Write-ColorOutput "✓ SUCCESS: Visual Studio 2022 C++ Compiler is properly installed!" $Green
    Write-ColorOutput "════════════════════════════════════════════════════════════════════════" $Green
    Write-Host ""
    Write-Info "You can now build the project using build.bat or CMake"
    exit 0
} else {
    Write-Host ""
    Write-ColorOutput "════════════════════════════════════════════════════════════════════════" $Red
    Write-ColorOutput "✗ MISSING COMPONENTS DETECTED" $Red
    Write-ColorOutput "════════════════════════════════════════════════════════════════════════" $Red
    Write-Host ""
    Write-ColorOutput "The following components are required but not installed:" $Yellow
    Write-Host ""
    
    foreach ($component in $missingComponents) {
        Write-Host "  • $($component.Name)"
    }
    
    Write-Host ""
    Write-Host ""
    
    # Provide fix options
    Write-Header "How to Fix"
    
    Write-Host ""
    Write-ColorOutput "Option 1: Modify Visual Studio Installation (Recommended)" $Cyan
    Write-Host "  1. Close all Visual Studio instances"
    Write-Host "  2. Open 'Visual Studio Installer' (search in Start Menu)"
    Write-Host "  3. Click 'Modify' on Visual Studio 2022"
    Write-Host "  4. Select the 'Desktop development with C++' workload"
    Write-Host "  5. In the right panel, ensure these are checked:"
    Write-Host "     ✓ MSVC v143 - VS 2022 C++ x64/x86 build tools (Latest)"
    Write-Host "     ✓ Windows 11 SDK (or Windows 10 SDK)"
    Write-Host "     ✓ C++ CMake tools for Windows"
    Write-Host "  6. Click 'Modify' to install"
    Write-Host ""
    
    Write-ColorOutput "Option 2: Repair Visual Studio Installation" $Cyan
    Write-Host "  1. Open 'Visual Studio Installer'"
    Write-Host "  2. Click the '...' menu on Visual Studio 2022"
    Write-Host "  3. Select 'Repair'"
    Write-Host "  4. Wait for repair to complete"
    Write-Host ""
    
    if ($AutoFix) {
        Write-Host ""
        Write-ColorOutput "AutoFix Mode: Opening Visual Studio Installer..." $Cyan
        
        # Find Visual Studio Installer
        $vsInstallerPath = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vs_installer.exe"
        
        if (Test-Path $vsInstallerPath) {
            Write-Info "Launching Visual Studio Installer..."
            Start-Process $vsInstallerPath -ArgumentList "modify --installPath `"$vsPath`""
            Write-Host ""
            Write-Success "Visual Studio Installer opened!"
            Write-Host ""
            Write-Info "Please select 'Desktop development with C++' workload and click 'Modify'"
        } else {
            Write-Warning "Could not find Visual Studio Installer at: $vsInstallerPath"
            Write-Info "Please manually open Visual Studio Installer from Start Menu"
        }
    } else {
        Write-Host ""
        Write-ColorOutput "Quick Fix: Run this script with -AutoFix flag to open the installer automatically:" $Cyan
        Write-Host "  powershell -ExecutionPolicy Bypass -File tools\check-vs2022-compiler.ps1 -AutoFix"
        Write-Host ""
    }
    
    exit 1
}
