# Visual Studio 2022 Compiler Detection and Fix Tools

This directory contains tools to automatically detect and help fix issues with Visual Studio 2022 C++ compiler installation.

## Files

### check-vs2022-compiler.ps1
PowerShell script that performs comprehensive detection of Visual Studio 2022 and C++ compiler components.

**Features:**
- Detects Visual Studio 2022 installation
- Verifies C++ compiler (MSVC) is installed
- Checks for required components (C++ build tools, Windows SDK)
- Validates cl.exe compiler executable
- Provides detailed error messages with fix instructions
- Can automatically open Visual Studio Installer with `-AutoFix` flag

**Usage:**
```powershell
# Basic detection (shows detailed output)
powershell -ExecutionPolicy Bypass -File tools\check-vs2022-compiler.ps1

# Quiet mode (minimal output, useful for scripts)
powershell -ExecutionPolicy Bypass -File tools\check-vs2022-compiler.ps1 -Quiet

# Automatic fix (opens Visual Studio Installer if issues found)
powershell -ExecutionPolicy Bypass -File tools\check-vs2022-compiler.ps1 -AutoFix

# Combine flags
powershell -ExecutionPolicy Bypass -File tools\check-vs2022-compiler.ps1 -Quiet -AutoFix
```

**Exit Codes:**
- `0`: Success - Compiler is properly installed
- `1`: Error - Compiler or components are missing

### check-vs2022-compiler.bat
Batch file wrapper for the PowerShell script, making it easier to call from other batch files.

**Usage:**
```batch
call tools\check-vs2022-compiler.bat
call tools\check-vs2022-compiler.bat -quiet
call tools\check-vs2022-compiler.bat -autofix
call tools\check-vs2022-compiler.bat -quiet -autofix
```

## Integration with Build Scripts

### setup.bat Integration
The `setup.bat` script now calls the compiler detection tool during Step 3 of the setup process:
- Runs compiler detection
- If issues are found, offers to automatically open Visual Studio Installer
- Provides option to continue anyway or abort setup

### build.bat Integration
The `build.bat` script now checks the compiler before attempting to build:
- Runs quick compiler check in quiet mode
- If issues are found, offers detailed diagnostics
- Provides option to auto-fix or continue anyway
- Helps prevent confusing CMake errors by catching compiler issues early

## What the Detection Script Checks

1. **vswhere.exe** - Visual Studio installer tools presence
2. **Visual Studio 2022** - VS 2022 installation (version 17.x)
3. **C++ Components**:
   - MSVC v143 C++ build tools
   - Windows SDK (10 or 11)
4. **MSVC Compiler** - cl.exe executable presence and version
5. **Windows SDK** - SDK installation and configuration

## Common Issues Detected

### Issue: Visual Studio not installed
**Detection:** vswhere.exe not found or no VS 2022 installation
**Fix:** Script opens Visual Studio download page with `-AutoFix`

### Issue: C++ workload not installed
**Detection:** MSVC component missing, cl.exe not found
**Fix:** Script opens Visual Studio Installer to modify installation

### Issue: Partial installation
**Detection:** Some required components missing
**Fix:** Detailed list of missing components with installation instructions

## Manual Usage Example

If you suspect compiler issues, run the detection manually:

```batch
cd C:\path\to\GameEngine
tools\check-vs2022-compiler.bat
```

This will show you detailed information about your Visual Studio installation and any problems detected.

## Requirements

- Windows 10/11
- PowerShell 5.0 or higher (built into Windows)
- Visual Studio 2022 (Community, Professional, or Enterprise)
- Desktop development with C++ workload

## Troubleshooting

### "Running scripts is disabled on this system"
If you get a PowerShell execution policy error, you have two options:

1. Use the batch wrapper (recommended):
   ```batch
   tools\check-vs2022-compiler.bat
   ```

2. Or temporarily bypass the policy:
   ```powershell
   powershell -ExecutionPolicy Bypass -File tools\check-vs2022-compiler.ps1
   ```

### Script shows wrong information
Try running as Administrator:
```batch
# Right-click Command Prompt -> "Run as administrator"
cd C:\path\to\GameEngine
tools\check-vs2022-compiler.bat
```

### AutoFix doesn't work
If `-AutoFix` doesn't launch the installer:
1. Manually open "Visual Studio Installer" from Start Menu
2. Click "Modify" on Visual Studio 2022
3. Select "Desktop development with C++" workload
4. Click "Modify" button

## Benefits

- **Early Error Detection**: Catch compiler issues before CMake configuration
- **Clear Error Messages**: Understand exactly what's missing
- **Automated Fixes**: Quick path to fixing common issues
- **Better User Experience**: Less confusion for developers setting up the project
- **Comprehensive Validation**: Checks multiple components, not just vswhere

## Technical Details

The detection script uses:
- `vswhere.exe` to query Visual Studio installations
- File system checks for compiler binaries
- Component ID validation against installed packages
- Exit codes for script integration
- ANSI color codes for readable output

## See Also

- [WINDOWS_TROUBLESHOOTING.md](../WINDOWS_TROUBLESHOOTING.md) - Detailed troubleshooting guide
- [README.md](../README.md) - Main project documentation
- [setup.bat](../setup.bat) - Initial setup script
- [build.bat](../build.bat) - Build script
