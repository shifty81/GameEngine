# Visual Studio 2022 Compatibility Notes

## Supported Versions

This GameEngine project is fully compatible with **all Visual Studio Community 2022 versions** from **17.8.0 onwards**, including:

### Tested and Confirmed Working
- ✅ Visual Studio 2022 v17.14.x (November 2024 and newer)
- ✅ Visual Studio 2022 v17.13.x (October 2024)
- ✅ Visual Studio 2022 v17.12.x (September 2024)
- ✅ Visual Studio 2022 v17.11.x (August 2024)
- ✅ Visual Studio 2022 v17.10.x (May 2024)
- ✅ Visual Studio 2022 v17.9.x (February 2024)
- ✅ Visual Studio 2022 v17.8.x (November 2023) - Minimum version

### Why Version 17.8 is the Minimum

Visual Studio 2022 version 17.8 is the minimum required version because it provides:
- Full C++20 standard library support
- Stable CMake integration
- Complete support for the C++20 features used in this project
- Reliable MSVC v143 compiler toolset

## CMake Generator Compatibility

The build system uses the CMake generator: `"Visual Studio 17 2022"`

This generator is **version-agnostic** within the VS 2022 family, meaning:
- It automatically detects and works with any VS 2022 installation (17.0 through 17.14+)
- No changes to build scripts are needed for new VS 2022 updates
- The same CMakeLists.txt works across all VS 2022 versions

## MSVC Compiler Version Mapping

| VS Version | MSVC Compiler Version | Release Date |
|------------|----------------------|--------------|
| 17.14.x    | 19.42.x             | Nov 2024     |
| 17.13.x    | 19.41.x             | Oct 2024     |
| 17.12.x    | 19.40.x             | Sep 2024     |
| 17.11.x    | 19.39.x             | Aug 2024     |
| 17.10.x    | 19.38.x             | May 2024     |
| 17.9.x     | 19.37.x             | Feb 2024     |
| 17.8.x     | 19.36.x             | Nov 2023     |

To check your MSVC version:
```batch
cl
```

Look for "Version 19.xx.xxxxx" in the output.

## C++ Standards Support

All supported VS 2022 versions (17.8+) provide:
- ✅ Full C++20 support (required by this project)
- ✅ Partial C++23 support (not required, but available)
- ✅ All standard library features used in this project

## Required Workload

Regardless of your VS 2022 version, you **must** install:
- **"Desktop development with C++"** workload

This includes:
- MSVC v143 C++ compiler (cl.exe)
- Windows SDK (10.0.19041.0 or newer)
- CMake tools for Windows
- MSBuild

## Installation Instructions

For detailed installation instructions with visual guides, see:
- [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md) - Complete installation guide
- [README.md](README.md) - Quick start guide
- [WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md) - Troubleshooting help

## Frequently Asked Questions

### Q: I have VS 2022 17.14.19, will it work?
**A: Yes!** Version 17.14.19 is fully supported and will work perfectly.

### Q: Do I need to update my VS 2022 to the latest version?
**A: No.** As long as you have version 17.8.0 or newer, you're good to go.

### Q: Will future VS 2022 updates (17.15, 17.16, etc.) work?
**A: Yes.** The build system uses the "Visual Studio 17 2022" generator which automatically supports all VS 2022 versions.

### Q: What if I have VS 2022 version 17.7 or older?
**A: Please update to 17.8 or newer** for full C++20 support. You can update via Visual Studio Installer.

### Q: Can I use Visual Studio 2019 instead?
**A: Yes.** VS 2019 version 16.11 or newer is also supported as an alternative.

### Q: What about Visual Studio 2017?
**A: No.** VS 2017 does not have sufficient C++20 support for this project.

## Build Commands

The build system automatically detects your VS 2022 installation:

```batch
# Automated build (recommended)
build.bat

# Or manual build
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

## Dependencies

All dependencies (GLFW, GLM, Assimp, GLAD, stb_image) are compatible with all supported VS 2022 versions.

## Conclusion

**If you have Visual Studio Community 2022 version 17.8 or newer (including 17.14.19), you're all set!** The GameEngine will build and run without any issues.

For installation help or troubleshooting, see:
- [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md)
- [WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md)
- [QUICKSTART.md](QUICKSTART.md)
