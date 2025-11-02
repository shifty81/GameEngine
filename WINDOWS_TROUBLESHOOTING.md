# Windows Build Troubleshooting Guide

**Before troubleshooting:** For detailed Visual Studio installation instructions with visual guides, see [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md).

## Visual Studio Version Requirements

**This project requires:**
- **Visual Studio Community 2022** (version 17.8 or newer, recommended: 17.11+)
- **OR Visual Studio Community 2019** (version 16.11 or newer)
- With **"Desktop development with C++"** workload installed

**How to check your version:**
- Open "Visual Studio Installer" from Windows Start menu
- Look at the version number below "Visual Studio Community 2022" or "2019"
- Example: "Version 17.11.5" means Visual Studio 2022 version 17.11.5

## Error: "No CMAKE_CXX_COMPILER could be found" or "The CXX compiler identification is unknown"

This error occurs when Visual Studio is installed without the C++ compiler tools. Here's how to fix it:

### Solution 1: Install C++ Desktop Development Workload (Recommended)

**📖 For detailed visual guide, see [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md) Step 3**

1. **Open Visual Studio Installer**
   - Search for "Visual Studio Installer" in Windows Start Menu
   - Or download from: https://visualstudio.microsoft.com/downloads/
   - **Recommended version:** Visual Studio Community 2022 (v17.8+ or newer)

2. **Modify Your Installation**
   - Click the "Modify" button next to your Visual Studio installation
   - If Visual Studio isn't installed, click "Install" and select Community Edition (free)
   - **Important:** Make sure you're installing Visual Studio 2022 (v17.8+) or Visual Studio 2019 (v16.11+)

3. **Select the Required Workload**
   - Check the box for **"Desktop development with C++"**
   - This installs:
     - MSVC v143 C++ compiler (cl.exe) for VS 2022
     - MSVC v142 C++ compiler (cl.exe) for VS 2019
     - Windows SDK (10.0.19041.0 or newer)
     - MSBuild tools
     - CMake integration

4. **Verify Installation**
   - Click "Modify" or "Install" button (bottom right)
   - Wait for installation to complete (may take 10-20 minutes)
   - Restart your computer if prompted

5. **Verify Compiler is Installed**
   ```powershell
   # Open "Developer Command Prompt for VS 2022" or "Developer PowerShell for VS 2022"
   # You can find these in Start Menu under Visual Studio 2022
   
   cl
   # Should output: "Microsoft (R) C/C++ Optimizing Compiler Version 19.38.xxxxx..."
   # Version 19.38.x = VS 2022 v17.11
   # Version 19.29.x = VS 2019 v16.11
   ```

6. **Clean and Rebuild**
   ```powershell
   cd C:\Users\Shifty\Desktop\GameEngine
   Remove-Item -Recurse -Force build
   mkdir build
   cd build
   cmake ..
   cmake --build . --config Release
   ```

### Solution 2: Use Build Tools Only (No IDE)

If you don't want the full Visual Studio IDE:

1. **Download Build Tools for Visual Studio 2022**
   - https://visualstudio.microsoft.com/downloads/
   - Scroll down to "Tools for Visual Studio"
   - Download "Build Tools for Visual Studio 2022" (version 17.8 or newer)

2. **Install with C++ Support**
   - Run the installer
   - Select "Desktop development with C++"
   - Ensure these components are selected:
     - MSVC v143 - VS 2022 C++ x64/x86 build tools
     - Windows SDK (10.0.19041.0 or newer)
   - Click Install

3. **Use from Command Line**
   ```powershell
   # Open "Developer Command Prompt for VS 2022"
   cd C:\Users\Shifty\Desktop\GameEngine
   build.bat
   ```

### Solution 3: Specify Generator Manually

If Visual Studio is installed but CMake can't find it:

```powershell
cd C:\Users\Shifty\Desktop\GameEngine\build

# For Visual Studio 2022:
cmake -G "Visual Studio 17 2022" -A x64 ..

# For Visual Studio 2019:
cmake -G "Visual Studio 16 2019" -A x64 ..

# Then build:
cmake --build . --config Release
```

### Verifying Your Setup

After installing the C++ workload, verify everything is working:

```powershell
# Check CMake version
cmake --version
# Should show: cmake version 3.20.0 or higher

# Check for C++ compiler (from Developer Command Prompt)
cl
# Should show: Microsoft (R) C/C++ Optimizing Compiler

# Check Visual Studio installation
where cl
# Should show path like: C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\...\cl.exe
```

## Current Error Analysis

Based on your output:
```
-- Building for: Visual Studio 17 2022
-- The CXX compiler identification is unknown
CMake Error at CMakeLists.txt:27 (project):
  No CMAKE_CXX_COMPILER could be found.
```

**Analysis**: 
- CMake detected Visual Studio 2022 ✅
- Windows SDK found (10.0.26100.0) ✅  
- BUT: No C++ compiler found ❌

**Root Cause**: Visual Studio 2022 is installed, but the "Desktop development with C++" workload is NOT installed.

**Fix**: Follow Solution 1 above to add the C++ workload to your existing Visual Studio installation.

## Step-by-Step Resolution for Your Specific Case

1. **Close all Visual Studio windows and PowerShell**

2. **Open Visual Studio Installer**
   - Press Windows key
   - Type "Visual Studio Installer"
   - Press Enter

3. **You should see "Visual Studio Community 2022" or "Visual Studio Professional 2022"**
   - Click the "Modify" button

4. **In the Workloads tab**
   - Scroll down and check **"Desktop development with C++"**
   - Make sure these are selected in the right panel:
     - ✅ MSVC v143 - VS 2022 C++ x64/x86 build tools
     - ✅ Windows 11 SDK (or Windows 10 SDK)
     - ✅ C++ CMake tools for Windows

5. **Click "Modify" button (bottom right)**
   - Wait for installation (10-20 minutes)
   - You might need to restart your computer

6. **After installation, clean and rebuild**
   ```powershell
   cd C:\Users\Shifty\Desktop\GameEngine
   Remove-Item -Recurse -Force build
   mkdir build
   cd build
   cmake ..
   ```

   You should now see:
   ```
   -- The CXX compiler identification is MSVC 19.xx.xxxxx
   -- Configuring done
   -- Generating done
   ```

7. **Build the project**
   ```powershell
   cmake --build . --config Release
   ```

8. **Run the game**
   ```powershell
   cd ..
   .\build\bin\Release\GameEngine.exe
   ```

## Alternative: Using the Build Script

Once the C++ compiler is installed, you can use the convenient build script:

```powershell
cd C:\Users\Shifty\Desktop\GameEngine
.\build.bat
```

This script will:
- Update git submodules
- Create build directory
- Run CMake configuration
- Build in Release mode
- Report success/failure

## Requirements Summary

To build this project on Windows, you need:

1. **CMake 3.20 or higher**
   - Download from: https://cmake.org/download/
   - Select "Add CMake to system PATH" during installation

2. **Visual Studio 2022 (v17.8+) or Visual Studio 2019 (v16.11+)** with:
   - **Recommended:** Visual Studio Community 2022 version 17.8 or newer (latest: 17.11+)
   - **Alternative:** Visual Studio Community 2019 version 16.11 or newer
   - Desktop development with C++ workload
   - MSVC compiler (v143 for 2022, v142 for 2019)
   - Windows SDK (10.0.19041.0 or newer)
   - C++20 support
   - **📖 See [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md) for complete installation guide**

3. **Git for Windows**
   - Download from: https://git-scm.com/download/win

## Still Having Issues?

### Check Event Viewer
1. Press Windows + X
2. Select "Event Viewer"
3. Go to Windows Logs → Application
4. Look for Visual Studio installation errors

### Repair Visual Studio
1. Open Visual Studio Installer
2. Click the "More" dropdown (three dots)
3. Select "Repair"
4. Wait for repair to complete

### Reinstall Visual Studio
If all else fails:
1. Uninstall Visual Studio completely
2. Download fresh installer from Microsoft
3. Install with "Desktop development with C++" workload
4. Try building again

## Quick Reference

### Required Visual Studio Components
- ✅ Desktop development with C++
- ✅ MSVC v143 (or v142 for VS 2019)
- ✅ Windows SDK 10.x or 11.x
- ✅ C++ CMake tools for Windows
- ✅ C++20 support (included in recent MSVC)

### Build Commands
```powershell
# Clean build
Remove-Item -Recurse -Force build
mkdir build
cd build

# Configure
cmake ..

# Build
cmake --build . --config Release

# Or use the script
cd ..
.\build.bat
```

## Additional Help

If you continue having issues:
1. Check the detailed error messages in CMake output
2. Verify Visual Studio installation in "Visual Studio Installer"
3. Check if cl.exe exists:
   ```powershell
   Get-ChildItem "C:\Program Files\Microsoft Visual Studio" -Recurse -Filter cl.exe
   ```
4. Make sure you're using a recent CMake version (3.20+)

## Success Indicators

You'll know it's working when you see:
```
-- The CXX compiler identification is MSVC 19.xx.xxxxx
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: ...\cl.exe - skipped
-- Configuring done
-- Generating done
-- Build files have been written to: .../GameEngine/build
```

Then you can build and run the game successfully!
