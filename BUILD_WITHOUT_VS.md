# Building Without Visual Studio

This guide explains how to build the GameEngine on Windows **without Visual Studio**, using the free and open-source MinGW-w64 compiler.

## Overview

While the GameEngine can be built with Visual Studio, you don't need it! The project includes an integrated MinGW-w64 build system that provides a complete Visual Studio-free build solution.

### What is MinGW-w64?

MinGW-w64 (Minimalist GNU for Windows) is a free, open-source C++ compiler for Windows that:
- Provides full C++20 standard support
- Requires no license or registration
- Is much smaller than Visual Studio (~500 MB vs ~10 GB)
- Offers comparable performance to MSVC
- Is used by many professional projects

## Quick Start (Automated Setup)

The easiest way to build without Visual Studio is to use the automated MinGW build script:

### Step 1: Double-click `build-mingw.bat`

That's it! The script will automatically:
1. ✅ Download and install MinGW-w64 (if not present)
2. ✅ Check for CMake and Git
3. ✅ Download all dependencies (GLFW, GLM, Assimp)
4. ✅ Configure the build system
5. ✅ Compile the GameEngine
6. ✅ Create the executable

### Step 2: Run the Engine

After successful build, run:
```
build-mingw\bin\GameEngine.exe
```

## Manual Setup (Advanced Users)

If you prefer more control over the process, follow these steps:

### Prerequisites

1. **CMake 3.20+**
   - Download from: https://cmake.org/download/
   - Select "Add CMake to system PATH" during installation

2. **Git for Windows**
   - Download from: https://git-scm.com/download/win

3. **MinGW-w64**
   - Either use our automated installer (see below)
   - Or download manually from: https://winlibs.com/

### Install MinGW-w64 (Automated)

Run the setup script:
```batch
tools\setup-mingw.bat
```

This will:
- Download MinGW-w64 (GCC 13.2.0) automatically
- Extract it to `external/mingw64/`
- Verify the installation
- ~150 MB download, takes 2-5 minutes

### Install MinGW-w64 (Manual)

If you prefer to install manually:

1. Download MinGW-w64 from: https://winlibs.com/
   - Choose: **GCC 13.2.0** or newer
   - Architecture: **x86_64** (64-bit)
   - Threading: **posix**
   - Exception: **seh**

2. Extract to `GameEngine/external/mingw64/`
   - Ensure `g++.exe` is at: `GameEngine/external/mingw64/bin/g++.exe`

3. (Optional) Add to PATH:
   - Press Windows + R, type: `sysdm.cpl`
   - Environment Variables → Path → Edit
   - Add: `C:\path\to\GameEngine\external\mingw64\bin`

### Build the Project

```batch
# Clone repository with dependencies
git clone --recursive https://github.com/shifty81/GameEngine.git
cd GameEngine

# Create build directory
mkdir build-mingw
cd build-mingw

# Configure with MinGW
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..

# Build (use -j flag for parallel compilation)
cmake --build . --config Release -j 8

# Run
..\build-mingw\bin\GameEngine.exe
```

## Comparison: Visual Studio vs MinGW

| Feature | Visual Studio | MinGW-w64 |
|---------|--------------|-----------|
| **Cost** | Free (Community) | Free (Open Source) |
| **Size** | ~10-15 GB | ~500 MB |
| **Installation Time** | 30-60 minutes | 2-5 minutes |
| **Build Speed** | Fast | Comparable |
| **C++20 Support** | Yes | Yes |
| **Debugging** | Excellent (Visual Studio) | Good (GDB, external tools) |
| **IDE** | Included | Use any (VS Code, etc.) |
| **License** | Microsoft EULA | GPL/BSD |

## Advantages of MinGW Build

✅ **No Registration**: No Microsoft account needed  
✅ **Small Download**: Only ~150 MB vs 10+ GB for Visual Studio  
✅ **Fast Setup**: 2-5 minutes vs 30-60 minutes  
✅ **Portable**: Can be placed anywhere, no system installation required  
✅ **Cross-Platform**: Same compiler used on Linux (GCC)  
✅ **Open Source**: Fully open-source toolchain  
✅ **CI/CD Friendly**: Easier to automate builds  

## Troubleshooting

### Error: "g++: command not found" or "g++ is not recognized"

**Solution**: MinGW bin directory is not in PATH

1. Re-run `tools\setup-mingw.bat`
2. Or manually add to PATH:
   - Press Windows + R, type: `sysdm.cpl`
   - Environment Variables → Path → Edit
   - Add: `C:\path\to\GameEngine\external\mingw64\bin`
   - Restart Command Prompt

### Error: "cmake: command not found"

**Solution**: CMake is not installed or not in PATH

1. Download CMake from: https://cmake.org/download/
2. During installation, select "Add CMake to system PATH"
3. Restart Command Prompt

### Error: "Could not find GLFW" or "Could not find GLM"

**Solution**: Git submodules not initialized

```batch
git submodule update --init --recursive
```

### Error: Long build time (>30 minutes)

**Solution**: Use parallel compilation

```batch
cmake --build . --config Release -j 8
```

The `-j 8` flag uses 8 parallel jobs. Adjust based on your CPU cores.

### Build fails with linking errors

**Solution**: Clean build directory and rebuild

```batch
cd GameEngine
rmdir /s /q build-mingw
build-mingw.bat
```

### MinGW download fails

**Solution**: Download manually

1. Go to: https://winlibs.com/
2. Download: **GCC 13.2.0+ x86_64-posix-seh**
3. Extract to: `GameEngine/external/mingw64/`
4. Run: `build-mingw.bat`

## Using with IDEs

### Visual Studio Code

1. Install C/C++ extension
2. Install CMake Tools extension
3. Select "MinGW Makefiles" as generator
4. Press F7 to build

### CLion

1. Settings → Build, Execution, Deployment → Toolchains
2. Add MinGW toolchain
3. Set paths:
   - C Compiler: `external/mingw64/bin/gcc.exe`
   - C++ Compiler: `external/mingw64/bin/g++.exe`
   - Make: `external/mingw64/bin/mingw32-make.exe`
4. Build normally

### Code::Blocks

1. Settings → Compiler → Select "GNU GCC Compiler"
2. Compiler settings → Toolchain executables
3. Set compiler's installation directory to: `external/mingw64`
4. Build normally

## Debugging

MinGW includes GDB (GNU Debugger):

```batch
# Build with debug symbols
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug

# Run with debugger
gdb build-mingw\bin\GameEngine.exe
```

Or use with Visual Studio Code's C++ debugger.

## Performance

MinGW-compiled binaries have comparable performance to Visual Studio:
- Both use modern optimizations (-O3 / /O2)
- Both support C++20 features
- Frame rates are typically within 1-3% of each other

## Continuous Integration

MinGW is excellent for CI/CD pipelines:

```yaml
# Example GitHub Actions workflow
- name: Setup MinGW
  run: tools\setup-mingw.bat

- name: Build
  run: build-mingw.bat
```

## Distribution

Executables built with MinGW require MinGW runtime DLLs:
- `libgcc_s_seh-1.dll`
- `libstdc++-6.dll`
- `libwinpthread-1.dll`

These are located in `external/mingw64/bin/` and must be distributed with your executable, or you can statically link them:

```cmake
# Add to CMakeLists.txt for static linking
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif()
```

## FAQ

**Q: Can I use both Visual Studio and MinGW?**  
A: Yes! Use `build.bat` for Visual Studio builds and `build-mingw.bat` for MinGW builds. They use separate build directories.

**Q: Which should I use: Visual Studio or MinGW?**  
A: 
- **Visual Studio**: Best if you already have it installed, want the integrated debugger, or prefer the IDE
- **MinGW**: Best if you want a lightweight solution, don't want to install Visual Studio, or prefer command-line builds

**Q: Is the game engine as fast with MinGW?**  
A: Yes, performance is virtually identical. Both compilers produce highly optimized code.

**Q: Can I use MinGW for commercial projects?**  
A: Yes! MinGW is open source and can be used for commercial projects without restrictions.

**Q: Does this work on other Windows compilers (Clang, Intel)?**  
A: The project is primarily tested with Visual Studio and MinGW, but CMake supports other compilers. You may need to adjust CMakeLists.txt.

## Additional Resources

- **MinGW-w64 Official**: https://www.mingw-w64.org/
- **WinLibs (Recommended Builds)**: https://winlibs.com/
- **GCC Documentation**: https://gcc.gnu.org/onlinedocs/
- **CMake with MinGW**: https://cmake.org/cmake/help/latest/generator/MinGW%20Makefiles.html

## Support

If you encounter issues with the MinGW build:
1. Check this documentation first
2. See [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for general issues
3. Open an issue on GitHub with:
   - Output of `tools\setup-mingw.bat`
   - CMake configuration output
   - Build error messages
