# Testing Guide for Visual Studio 2022 Compiler Detection

This document describes how to test the Visual Studio 2022 compiler detection and fix functionality on a Windows machine.

## Prerequisites for Testing

- Windows 10 or Windows 11
- Access to a machine where you can install/uninstall Visual Studio components
- Administrator privileges (recommended for some tests)

## Test Scenarios

### Test 1: No Visual Studio Installed

**Setup:**
- Ensure Visual Studio 2022 is not installed on the test machine

**Test Steps:**
1. Open Command Prompt
2. Navigate to the GameEngine directory
3. Run: `tools\check-vs2022-compiler.bat`

**Expected Results:**
- Script should detect that vswhere.exe is not found
- Should display error message about Visual Studio not being installed
- Should provide download link to Visual Studio 2022
- Should exit with error code 1

**Auto-Fix Test:**
1. Run: `tools\check-vs2022-compiler.bat -autofix`

**Expected Results:**
- Should open the Visual Studio download page in default browser
- Should display instructions to install VS 2022 with C++ workload

### Test 2: Visual Studio 2022 Installed Without C++ Workload

**Setup:**
1. Install Visual Studio 2022 Community Edition
2. During installation, do NOT select "Desktop development with C++" workload
3. Or if already installed, use Visual Studio Installer to remove the C++ workload

**Test Steps:**
1. Open Command Prompt
2. Navigate to the GameEngine directory
3. Run: `tools\check-vs2022-compiler.bat`

**Expected Results:**
- Script should detect Visual Studio 2022 installation
- Should display VS version and installation path
- Should detect missing C++ components:
  - MSVC v143 C++ build tools
  - Windows SDK
  - cl.exe compiler
- Should display "MISSING COMPONENTS DETECTED" error
- Should list all missing components
- Should provide detailed fix instructions
- Should exit with error code 1

**Auto-Fix Test:**
1. Run: `tools\check-vs2022-compiler.bat -autofix`

**Expected Results:**
- Should launch Visual Studio Installer
- Installer should open with the modify dialog
- Should display instructions to select "Desktop development with C++"

### Test 3: Visual Studio 2022 Properly Installed with C++ Workload

**Setup:**
1. Ensure Visual Studio 2022 is installed
2. Ensure "Desktop development with C++" workload is installed

**Test Steps:**
1. Open Command Prompt
2. Navigate to the GameEngine directory
3. Run: `tools\check-vs2022-compiler.bat`

**Expected Results:**
- Script should detect Visual Studio 2022 installation
- Should display VS version and installation path
- Should detect all required components:
  - ✓ MSVC v143 C++ build tools
  - ✓ Windows SDK
  - ✓ cl.exe compiler with version
- Should display "SUCCESS: Visual Studio 2022 C++ Compiler is properly installed!"
- Should exit with error code 0

**Quiet Mode Test:**
1. Run: `tools\check-vs2022-compiler.bat -quiet`

**Expected Results:**
- Should run with minimal output
- Should still exit with code 0
- No detailed information displayed

### Test 4: Integration with setup.bat

**Setup:**
- Various Visual Studio installation states (no VS, VS without C++, VS with C++)

**Test Steps:**
1. Run: `setup.bat`
2. Observe Step 3: "Checking for C++ compiler..."

**Expected Results:**

**If VS not properly configured:**
- Should display compiler detection results
- Should ask: "Would you like to automatically open the Visual Studio Installer to fix these issues?"
- If user selects Yes: should launch installer
- If user selects No: should ask "Continue setup anyway?"

**If VS properly configured:**
- Should display success message
- Should continue to Step 4 (submodules)

### Test 5: Integration with build.bat

**Setup:**
- Visual Studio installed but C++ workload not installed

**Test Steps:**
1. Run: `build.bat`

**Expected Results:**
- Should run compiler check at the start
- Should detect issues and ask: "Run detailed diagnostics and get fix instructions?"
- If Yes selected: should show detailed error report
- Should ask: "Would you like to automatically open the Visual Studio Installer?"
- If user selects Yes: should launch installer
- Should ask: "Continue with build anyway (may fail)?"
- If No: should exit cleanly

### Test 6: PowerShell Direct Execution

**Test Steps:**
1. Open PowerShell
2. Navigate to GameEngine directory
3. Run: `powershell -ExecutionPolicy Bypass -File tools\check-vs2022-compiler.ps1`

**Expected Results:**
- Should work identically to the batch wrapper
- Should display colorized output (green for success, red for errors, etc.)
- Same detection logic and results

**Test Flags:**
1. Run with AutoFix: `powershell -ExecutionPolicy Bypass -File tools\check-vs2022-compiler.ps1 -AutoFix`
2. Run with Quiet: `powershell -ExecutionPolicy Bypass -File tools\check-vs2022-compiler.ps1 -Quiet`
3. Run with both: `powershell -ExecutionPolicy Bypass -File tools\check-vs2022-compiler.ps1 -AutoFix -Quiet`

### Test 7: Edge Cases

**Test 7a: Multiple Visual Studio Versions**
- Install both VS 2019 and VS 2022
- Run detection script
- Should detect VS 2022 (latest) specifically

**Test 7b: Custom Installation Path**
- Install VS 2022 to a non-default location
- Run detection script
- Should still find it using vswhere.exe

**Test 7c: Partial C++ Installation**
- Install only some C++ components (e.g., MSVC but no Windows SDK)
- Run detection script
- Should list specifically which components are missing

**Test 7d: Running Without Admin Privileges**
- Run detection script as regular user
- Should still work and detect correctly
- Should warn about non-admin mode

## Validation Checklist

After running all tests, verify:

- [ ] Script correctly detects when VS is not installed
- [ ] Script correctly detects when C++ workload is missing
- [ ] Script correctly detects when all components are present
- [ ] Script provides clear, actionable error messages
- [ ] Auto-fix flag launches Visual Studio Installer
- [ ] Quiet mode suppresses detailed output
- [ ] setup.bat integration works smoothly
- [ ] build.bat integration catches issues before CMake fails
- [ ] Exit codes are correct (0 for success, 1 for failure)
- [ ] Script works in both Command Prompt and PowerShell
- [ ] Color output works correctly (if terminal supports it)
- [ ] Instructions are clear and easy to follow

## Manual Verification Steps

After fixing issues using the script:

1. Run the detection script again - should show success
2. Delete the `build` folder
3. Run `build.bat` - should successfully configure and build
4. Verify `GameEngine.exe` is created in `build\bin\Release\`
5. Run the game engine to ensure it works

## Known Limitations

- Script requires vswhere.exe (installed with VS 2017+)
- Only detects Visual Studio 2022 (version 17.x)
- Requires PowerShell (built into Windows)
- Auto-fix can only open the installer, cannot automatically install components
- User must manually select workload and confirm installation

## Reporting Issues

If any test fails or produces unexpected results:

1. Note the Visual Studio installation state
2. Capture the full script output
3. Note any error messages
4. Check Windows Event Viewer for installation errors
5. Report findings with reproduction steps

## Success Criteria

The feature is working correctly if:

1. All detection scenarios work as expected
2. Error messages are clear and helpful
3. Auto-fix successfully launches the installer
4. Integration with setup.bat and build.bat is smooth
5. Users can successfully fix their installation using the provided instructions
6. The script improves the user experience when setting up the project
