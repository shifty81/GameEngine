# Visual Studio Setup Guide - Complete Installation Instructions

This guide provides **specific version numbers** and **detailed visual instructions** for installing Visual Studio Community for GameEngine development.

## Table of Contents
- [Version Requirements](#version-requirements)
- [Visual Studio 2022 Installation (Recommended)](#visual-studio-2022-installation-recommended)
- [Visual Studio 2019 Installation (Alternative)](#visual-studio-2019-installation-alternative)
- [Verification Steps](#verification-steps)
- [Troubleshooting](#troubleshooting)

**⚠️ Having installation issues?** See the [Troubleshooting](#troubleshooting) section at the end of this guide, or refer to:
- **[TROUBLESHOOTING.md](TROUBLESHOOTING.md)** - Cross-platform troubleshooting
- **[WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md)** - Windows-specific detailed solutions

---

## Version Requirements

### Recommended: Visual Studio 2022 (Version 17.8 or newer)

**Specific Version Details:**
- **Product Name:** Visual Studio Community 2022
- **Minimum Version:** 17.8.0 (Released November 2023)
- **Recommended Version:** 17.11.5 or later (Latest stable)
- **Tested Versions:** 17.8.x, 17.9.x, 17.10.x, 17.11.x, 17.12.x, 17.13.x, 17.14.x
- **Version Number Format:** 17.x.y where x is the minor version and y is the patch
- **C++ Compiler (MSVC):** v143 or newer
- **C++ Standard Support:** C++20 (required for this project)
- **Windows SDK:** Windows 10 SDK (10.0.19041.0) or Windows 11 SDK (10.0.22000.0) or newer

**Why Version 17.8+?**
- Full C++20 support with all standard library features
- Improved CMake integration
- Better IntelliSense and debugging experience
- Enhanced compiler optimizations
- Stable and widely tested
- **Note:** All Visual Studio 2022 versions from 17.8 onwards (including 17.14.x) are fully supported

### Alternative: Visual Studio 2019 (Version 16.11 or newer)

**Specific Version Details:**
- **Product Name:** Visual Studio Community 2019
- **Minimum Version:** 16.11.0 (Released August 2021)
- **Version Number Format:** 16.x.y
- **C++ Compiler (MSVC):** v142
- **C++ Standard Support:** C++20 (partial, sufficient for this project)
- **Windows SDK:** Windows 10 SDK (10.0.19041.0) or newer

**Note:** Visual Studio 2019 is supported but Visual Studio 2022 is strongly recommended for the best development experience.

---

## Visual Studio 2022 Installation (Recommended)

### Step 1: Download Visual Studio Community 2022

1. **Navigate to the Visual Studio website:**
   - URL: https://visualstudio.microsoft.com/downloads/
   - Or search "Visual Studio Community 2022 download" in your browser

2. **Locate Visual Studio Community 2022:**
   ```
   ┌─────────────────────────────────────────────────────────┐
   │  Visual Studio 2022                                     │
   │                                                         │
   │  Community 2022                                         │
   │  Free download for students, open-source, and          │
   │  individual developers                                  │
   │                                                         │
   │  [ Free download ]  <-- Click this button              │
   │                                                         │
   │  Professional 2022                                      │
   │  [ Free trial ]                                         │
   │                                                         │
   │  Enterprise 2022                                        │
   │  [ Free trial ]                                         │
   └─────────────────────────────────────────────────────────┘
   ```

3. **Click the "Free download" button** under "Community 2022"
   - File name: `VisualStudioSetup.exe` or `vs_community.exe`
   - File size: ~3-5 MB (web installer)

4. **Save the installer** to your Downloads folder

### Step 2: Launch Visual Studio Installer

1. **Run the downloaded installer** (`VisualStudioSetup.exe`)
   - You may see a User Account Control (UAC) prompt
   - Click **"Yes"** to allow the installer to make changes

2. **Wait for the installer to initialize** (30-60 seconds)
   - You'll see "Getting things ready" or "Preparing installer"

3. **Visual Studio Installer window will open:**
   ```
   ┌────────────────────────────────────────────────────────────────┐
   │ Visual Studio Installer                              [_][□][X] │
   ├────────────────────────────────────────────────────────────────┤
   │                                                                │
   │  Visual Studio Community 2022                                 │
   │  Version 17.11.5                                              │
   │                                                                │
   │  [ Install ]  [ Modify ]  [ More ▼ ]                          │
   │                                                                │
   │  Installation location:                                        │
   │  C:\Program Files\Microsoft Visual Studio\2022\Community\     │
   │                                                                │
   └────────────────────────────────────────────────────────────────┘
   ```

### Step 3: Select Workloads

**This is the MOST IMPORTANT step!** You must select the correct workload.

**⚠️ Important Notes:**
- If you skip this step or don't select the C++ workload, the build will fail
- See [WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md) if you encounter "No CMAKE_CXX_COMPILER could be found" errors later

1. **Click the "Install" button** (for new installation)
   - Or click "Modify" if Visual Studio is already installed

2. **You'll see the Workloads selection screen:**
   ```
   ┌──────────────────────────────────────────────────────────────────────┐
   │ Visual Studio Installer                                   [_][□][X] │
   ├──────────────────────────────────────────────────────────────────────┤
   │ Workloads | Individual components | Language packs | Installation... │
   ├──────────────────────────────────────────────────────────────────────┤
   │                                                                      │
   │ ☐ ASP.NET and web development                                       │
   │                                                                      │
   │ ☑ Desktop development with C++          <--- CHECK THIS BOX!       │
   │   ├─ MSVC v143 - VS 2022 C++ x64/x86 build tools (Latest)         │
   │   ├─ Windows 11 SDK (10.0.22621.0)                                 │
   │   ├─ C++ CMake tools for Windows                                   │
   │   ├─ Testing tools core features                                   │
   │   └─ C++ AddressSanitizer                                          │
   │                                                                      │
   │ ☐ Game development with Unity                                       │
   │ ☐ Game development with Unreal Engine                              │
   │ ☐ Mobile development with .NET                                      │
   │ ☐ .NET desktop development                                          │
   │                                                                      │
   └──────────────────────────────────────────────────────────────────────┘
   
   Installation details:                                    ┌───────────┐
   Desktop development with C++                             │  Install  │
   Required: 9.5 GB                                         └───────────┘
   ```

3. **Check the box for "Desktop development with C++"**
   - This is **ESSENTIAL** - the project will not build without it
   - The checkmark should appear: ☑

4. **Verify the right panel shows these components** (automatically selected):
   ```
   Installation details
   ├─☑ MSVC v143 - VS 2022 C++ x64/x86 build tools (Latest)
   │   └─ This is the actual C++ compiler (cl.exe)
   ├─☑ Windows 11 SDK (10.0.22621.0) or Windows 10 SDK (10.0.19041.0)
   │   └─ Required for Windows API and OpenGL
   ├─☑ C++ CMake tools for Windows
   │   └─ CMake integration support
   ├─☑ C++ core features
   ├─☑ Just-In-Time debugger
   └─☑ C++ profiling tools
   ```

5. **Optional but recommended components** (check these for better experience):
   - ☑ C++ AddressSanitizer (helps find memory bugs)
   - ☑ C++ Clang tools for Windows (additional code analysis)
   - ☑ C++ ATL for latest v143 build tools (x86 & x64)

### Step 4: Choose Installation Location (Optional)

1. **By default, Visual Studio installs to:**
   ```
   C:\Program Files\Microsoft Visual Studio\2022\Community\
   ```

2. **To change location** (optional):
   - Click "Installation locations" tab at the top
   - You can change the installation directory
   - **Recommended:** Keep the default location unless you have a specific reason to change it
   - **Minimum space required:** ~10-15 GB

### Step 5: Begin Installation

1. **Review your selections:**
   ```
   ┌────────────────────────────────────────────────────────┐
   │  Installation details                                  │
   │  ─────────────────────────────────────────────────     │
   │  Desktop development with C++                          │
   │  Required: 9.5 GB                                      │
   │  Optional: 2.3 GB                                      │
   │                                                        │
   │  Total: 11.8 GB                                        │
   │                                                        │
   │  Download location: C:\ProgramData\Microsoft\VisualS...│
   │  Installation location: C:\Program Files\Microsoft V...│
   │                                                        │
   │                          [ Install ] [ Cancel ]        │
   └────────────────────────────────────────────────────────┘
   ```

2. **Click "Install" button** (bottom right)

3. **Installation progress window:**
   ```
   ┌─────────────────────────────────────────────────────────┐
   │ Visual Studio Installer                                │
   │                                                         │
   │ Installing Visual Studio Community 2022                │
   │                                                         │
   │ Acquiring: MSVC v143 build tools                       │
   │ ████████████████░░░░░░░░░░░░░░░░░░░ 45%               │
   │                                                         │
   │ Status: Downloading (1.2 GB of 2.8 GB)                │
   │                                                         │
   │ Time remaining: About 15 minutes                       │
   │                                                         │
   │ [ Pause ] [ Cancel ]                                   │
   └─────────────────────────────────────────────────────────┘
   ```

4. **Wait for installation to complete:**
   - **Download time:** 10-30 minutes (depends on internet speed)
   - **Installation time:** 10-20 minutes (depends on disk speed)
   - **Total time:** 20-50 minutes
   - You can pause and resume if needed

5. **Installation complete screen:**
   ```
   ┌─────────────────────────────────────────────────────────┐
   │ Visual Studio Installer                                │
   │                                                         │
   │ ✓ Installation succeeded!                              │
   │                                                         │
   │ Visual Studio Community 2022 is ready to use.          │
   │ Version 17.11.5                                         │
   │                                                         │
   │ [ Launch ] [ Close ]                                   │
   └─────────────────────────────────────────────────────────┘
   ```

6. **Click "Close"** (you don't need to launch Visual Studio IDE yet)

### Step 6: Verify Installation

After installation completes, verify the C++ compiler is installed:

1. **Open "Developer Command Prompt for VS 2022":**
   - Press Windows key
   - Type "Developer Command Prompt"
   - You should see:
     ```
     Developer Command Prompt for VS 2022
     Developer PowerShell for VS 2022
     ```
   - Click "Developer Command Prompt for VS 2022"

2. **Check compiler version:**
   ```batch
   cl
   ```
   
   **Expected output:**
   ```
   Microsoft (R) C/C++ Optimizing Compiler Version 19.38.33134 for x64
   Copyright (C) Microsoft Corporation.  All rights reserved.
   
   usage: cl [ option... ] filename... [ /link linkoption... ]
   ```

3. **Verify CMake can find the compiler:**
   ```batch
   cd C:\path\to\GameEngine
   mkdir build-test
   cd build-test
   cmake ..
   ```
   
   **Expected output:**
   ```
   -- Building for: Visual Studio 17 2022
   -- The CXX compiler identification is MSVC 19.38.33134.0
   -- Detecting CXX compiler ABI info
   -- Detecting CXX compiler ABI info - done
   -- Configuring done
   -- Generating done
   ```

**✓ SUCCESS!** If you see this output, your Visual Studio installation is correct and ready to build the GameEngine!

**⚠️ If you see errors instead**, refer to:
- The [Troubleshooting](#troubleshooting) section below
- [WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md) for detailed error resolution
- [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for cross-platform issues

---

## Visual Studio 2019 Installation (Alternative)

If you prefer Visual Studio 2019 or are on an older system:

### Version Requirements
- **Minimum Version:** Visual Studio Community 2019 version 16.11.0 or newer
- **Latest Version:** 16.11.35 (final release)
- **Download URL:** https://visualstudio.microsoft.com/vs/older-downloads/

### Installation Steps

The installation process is nearly identical to Visual Studio 2022:

1. **Download Visual Studio Community 2019** from the Microsoft website
   - You may need to expand "Older downloads" section
   - File: `vs_community__*.exe`

2. **Run the installer** and wait for it to initialize

3. **Select "Desktop development with C++" workload:**
   ```
   ☑ Desktop development with C++
     ├─ MSVC v142 - VS 2019 C++ x64/x86 build tools (Latest)
     ├─ Windows 10 SDK (10.0.19041.0 or newer)
     └─ C++ CMake tools for Windows
   ```

4. **Complete installation** (follow same steps as VS 2022)

5. **Verify installation:**
   ```batch
   cl
   ```
   Expected output:
   ```
   Microsoft (R) C/C++ Optimizing Compiler Version 19.29.xxxxx for x64
   ```

### CMake Generator for VS 2019

When running CMake, you may need to specify:
```batch
cmake -G "Visual Studio 16 2019" -A x64 ..
```

---

## Verification Steps

After installing Visual Studio, verify your setup:

### Method 1: Using GameEngine's Detection Tool

The GameEngine includes a comprehensive compiler detection tool:

```batch
cd C:\path\to\GameEngine
tools\check-vs2022-compiler.bat
```

**Expected output:**
```
========================================
Visual Studio 2022 Compiler Detection
========================================

[1/5] Checking for vswhere.exe...
   ✓ Found: C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe

[2/5] Detecting Visual Studio 2022...
   ✓ Found: Visual Studio Community 2022
   ✓ Version: 17.11.35327.3
   ✓ Path: C:\Program Files\Microsoft Visual Studio\2022\Community

[3/5] Checking for C++ components...
   ✓ MSVC v143 build tools: Installed
   ✓ Windows SDK: Installed (10.0.22621.0)

[4/5] Verifying MSVC compiler...
   ✓ Found: C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\bin\Hostx64\x64\cl.exe
   ✓ Version: 19.38.33134

[5/5] Checking Windows SDK...
   ✓ Windows SDK 10.0.22621.0 is installed

========================================
✓ SUCCESS: Visual Studio 2022 C++ compiler is properly configured!
========================================
```

### Method 2: Manual Verification

1. **Check Visual Studio Installer:**
   - Open "Visual Studio Installer" from Start menu
   - You should see:
     ```
     Visual Studio Community 2022
     Version 17.11.5
     [Modify] [Update] [Launch] [More ▼]
     ```

2. **Click "Modify"** and verify:
   - ☑ Desktop development with C++ is checked
   - Right panel shows MSVC and Windows SDK components

3. **Check compiler executable exists:**
   ```batch
   dir "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\*\bin\Hostx64\x64\cl.exe"
   ```
   Should find `cl.exe` file

---

## Troubleshooting

**⚠️ For comprehensive troubleshooting**, see:
- **[WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md)** - Detailed Windows-specific troubleshooting with step-by-step solutions
- **[TROUBLESHOOTING.md](TROUBLESHOOTING.md)** - Cross-platform troubleshooting guide

### Issue: "No CMAKE_CXX_COMPILER could be found"

**Cause:** Visual Studio is installed but the C++ workload is not.

**Visual Guide to Fix:**

1. **Open Visual Studio Installer:**
   ```
   Windows Start Menu → Type "Visual Studio Installer"
   ```

2. **You'll see this screen:**
   ```
   ┌────────────────────────────────────────────────────┐
   │ Visual Studio Installer                            │
   │                                                    │
   │ Visual Studio Community 2022                       │
   │ Version 17.11.5                                    │
   │                                                    │
   │ [ Modify ]  <-- Click this button                 │
   │ [ Update ]                                         │
   │ [ Launch ]                                         │
   │                                                    │
   └────────────────────────────────────────────────────┘
   ```

3. **Click "Modify"** button

4. **On Workloads tab, check the box:**
   ```
   ☐ Desktop development with C++  <-- Currently unchecked
   
   Change to:
   
   ☑ Desktop development with C++  <-- Check this!
   ```

5. **Click "Modify" button** (bottom right)

6. **Wait for installation** (10-15 minutes)

7. **Try building again:**
   ```batch
   cd C:\path\to\GameEngine
   rmdir /s /q build
   build.bat
   ```

**💡 For detailed step-by-step instructions with screenshots**, see [WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md#error-no-cmake_cxx_compiler-could-be-found-or-the-cxx-compiler-identification-is-unknown).

### Issue: Wrong Visual Studio Version Detected

**Scenario:** You have multiple Visual Studio versions installed.

**Solution:** Specify the generator explicitly:

```batch
cd C:\path\to\GameEngine\build
cmake -G "Visual Studio 17 2022" -A x64 ..
```

### Issue: Missing Components

If some components are missing, add them via Visual Studio Installer:

1. Open Visual Studio Installer
2. Click "Modify"
3. Click "Individual components" tab at the top
4. Search for and check:
   - ☑ MSVC v143 - VS 2022 C++ x64/x86 build tools
   - ☑ Windows 11 SDK (10.0.22621.0) or Windows 10 SDK
   - ☑ C++ CMake tools for Windows
5. Click "Modify" to install

**💡 Need more help?** See [WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md) for detailed troubleshooting steps.

---

## Visual Studio Version History Reference

For reference, here are the Visual Studio version numbers:

| Product Name | Version Number | Release Date | C++ Standard | Recommended |
|--------------|----------------|--------------|--------------|-------------|
| Visual Studio 2022 v17.14 | 17.14.x | November 2024 | C++20, C++23 (partial) | ✓ Yes (Tested) |
| Visual Studio 2022 v17.13 | 17.13.x | October 2024 | C++20, C++23 (partial) | ✓ Yes |
| Visual Studio 2022 v17.12 | 17.12.x | September 2024 | C++20, C++23 (partial) | ✓ Yes |
| Visual Studio 2022 v17.11 | 17.11.x | August 2024 | C++20, C++23 (partial) | ✓ Yes |
| Visual Studio 2022 v17.10 | 17.10.x | May 2024 | C++20, C++23 (partial) | ✓ Yes |
| Visual Studio 2022 v17.9 | 17.9.x | February 2024 | C++20, C++23 (partial) | ✓ Yes |
| Visual Studio 2022 v17.8 | 17.8.x | November 2023 | C++20 | ✓ Yes (Minimum) |
| Visual Studio 2022 v17.0-17.7 | 17.0.x - 17.7.x | November 2021 - August 2023 | C++20 | Works but update recommended |
| Visual Studio 2019 v16.11 | 16.11.x | August 2021 | C++20 (partial) | ✓ Yes (Alternative) |
| Visual Studio 2019 v16.10 and older | 16.10.x and below | May 2021 and earlier | C++17, C++20 (partial) | Update recommended |
| Visual Studio 2017 | 15.x.x | March 2017 | C++17 | Not recommended |

### How to Check Your Version

**In Visual Studio Installer:**
- Look at the version number below the product name
- Example: "Version 17.11.5" means Visual Studio 2022 version 17.11.5

**In Visual Studio IDE:**
- Help → About Microsoft Visual Studio
- Shows detailed version information

**From Command Line:**
```batch
cl
```
Look for "Version 19.xx.xxxxx" in the output:
- 19.42.x = Visual Studio 2022 v17.14
- 19.41.x = Visual Studio 2022 v17.13
- 19.40.x = Visual Studio 2022 v17.12
- 19.39.x = Visual Studio 2022 v17.11
- 19.38.x = Visual Studio 2022 v17.10
- 19.37.x = Visual Studio 2022 v17.9
- 19.29.x = Visual Studio 2019 v16.11

---

## Summary

**For GameEngine Development, Install:**

✓ **Visual Studio Community 2022** (version 17.8 or newer)
  - Free for individual developers, students, and open-source projects
  - Download: https://visualstudio.microsoft.com/downloads/
  
✓ **With "Desktop development with C++" workload**
  - Includes MSVC v143 C++ compiler
  - Includes Windows SDK
  - Includes CMake tools

✓ **Expected installation size:** ~10-15 GB

✓ **Expected installation time:** 20-50 minutes

**After installation:**
- Run `tools\check-vs2022-compiler.bat` to verify
- Run `build.bat` to build the engine
- Run `run.bat` to start the engine

---

## Need More Help?

- **Troubleshooting Guide:** See [WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md) for comprehensive Windows troubleshooting
- **Cross-platform Issues:** See [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for general build issues
- **Quick Start:** See [QUICKSTART.md](QUICKSTART.md) for a streamlined setup guide
- **Main Documentation:** See [README.md](README.md) for full documentation
- **Compiler Detection:** Run `tools\check-vs2022-compiler.bat` for automated diagnostics

**Still having issues?** Open an issue on GitHub with:
1. Your Visual Studio version (from Visual Studio Installer)
2. Output of `tools\check-vs2022-compiler.bat`
3. The error message you're seeing
