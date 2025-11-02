# New Feature Requirements

This document tracks new feature requirements for the GameEngine project.

## Requirement 1: Visual Studio 2022 Test Adapter Extension

### Description
Create a Visual Studio 2022 extension similar to the Unreal Engine Test Adapter that integrates with the GameEngine project.

### Purpose
- Provide seamless integration with VS2022's Test Explorer
- Allow running and debugging game engine tests directly from the IDE
- Provide visual feedback on test results
- Support test discovery and filtering

### Components Needed
1. **VS2022 Extension Project (VSIX)**
   - Language: C#
   - SDK: Visual Studio 2022 SDK
   - Template: Test Adapter Extension

2. **Test Adapter Implementation**
   - Implement `ITestDiscoverer` interface for test discovery
   - Implement `ITestExecutor` interface for test execution
   - Handle test result reporting to Test Explorer

3. **GameEngine Test Framework Integration**
   - Define test format/structure for game engine tests
   - Create test runner executable or use existing build
   - Parse test output and report to VS Test Explorer

4. **Extension Features**
   - Test discovery from project files
   - Run individual tests or test suites
   - Debug support for failing tests
   - Test filtering and grouping
   - Real-time test output display

### Implementation Steps
1. Set up VS2022 VSIX project structure
2. Implement test discovery mechanism
3. Implement test execution and result parsing
4. Add configuration options
5. Create extension manifest and assets
6. Package and test the extension
7. Create documentation for extension usage

### Technical Considerations
- Need to decide on test framework (Google Test, Catch2, or custom)
- Extension needs to work with CMake-based builds
- Should support both Debug and Release configurations
- Consider performance for large test suites

### Files to Create
- `extensions/VSTestAdapter/` - Extension source directory
- `extensions/VSTestAdapter/GameEngineTestAdapter.csproj` - C# project
- `extensions/VSTestAdapter/TestDiscoverer.cs` - Test discovery implementation
- `extensions/VSTestAdapter/TestExecutor.cs` - Test execution implementation
- `extensions/VSTestAdapter/source.extension.vsixmanifest` - Extension manifest
- `docs/TEST_ADAPTER_GUIDE.md` - Usage documentation

---

## Requirement 2: Integrated Compiler in Project

### Description
Integrate a C++ compiler directly into the GameEngine project to make it self-contained, so users don't need to install external compilers.

### Purpose
- Eliminate external compiler dependencies
- Provide consistent build environment across all systems
- Simplify setup process for new developers
- Enable truly portable game engine

### Challenges
1. **Compiler Size**: Modern C++ compilers are large (GCC ~200MB, Clang ~500MB, MSVC ~1-2GB)
2. **Licensing**: Compiler licenses must be compatible with project (GPL for GCC, Apache 2.0 for Clang)
3. **Platform Specificity**: Different compilers for Windows, Linux, macOS
4. **System Libraries**: Compilers need system headers and libraries (Windows SDK, glibc, etc.)
5. **Maintenance**: Keeping embedded compiler up-to-date

### Feasible Approaches

#### Option 1: Portable Clang/LLVM (Recommended)
- **What**: Bundle pre-built portable Clang/LLVM compiler
- **Size**: ~500MB compressed, ~1.5GB extracted
- **Pros**: 
  - Cross-platform (Windows, Linux, macOS)
  - Apache 2.0 license (permissive)
  - Modern C++20/23 support
  - Good performance
- **Cons**: 
  - Large download size
  - Still needs system headers
  - Git repo becomes very large

#### Option 2: MinGW-w64 (Windows Only)
- **What**: Bundle MinGW-w64 (GCC for Windows)
- **Size**: ~100-200MB
- **Pros**:
  - Smaller than MSVC
  - Works without Visual Studio
  - Good C++ standard support
- **Cons**:
  - Windows only
  - GPL license (must document)
  - Less optimized than MSVC/Clang

#### Option 3: TCC (Tiny C Compiler) - Not Recommended
- **What**: Ultra-small C compiler
- **Size**: ~1MB
- **Pros**: Extremely lightweight
- **Cons**: 
  - Limited C++ support (no modern C++)
  - Not suitable for game engine development

#### Option 4: Download on Demand (Hybrid Approach) - Most Practical
- **What**: Script downloads and caches compiler on first build
- **Size**: ~10KB script, downloads ~200-500MB on first use
- **Pros**:
  - Keeps Git repo small
  - Can choose best compiler for platform
  - Easy to update compiler version
  - Users who already have compiler don't download
- **Cons**:
  - Requires internet connection on first build
  - Still need to download compiler

### Recommended Implementation: Download-on-Demand System

Create a smart compiler management system:

```bash
# Structure
tools/
  compiler-manager.sh      # Linux/macOS script
  compiler-manager.bat     # Windows script
  compilers/
    .gitignore            # Ignore downloaded compilers
    windows/
      README.md           # Placeholder
    linux/
      README.md           # Placeholder
    macos/
      README.md           # Placeholder
```

**Features:**
1. Check if system compiler exists and meets requirements
2. If not, offer to download portable compiler:
   - Windows: MinGW-w64 or portable Clang
   - Linux: Portable GCC or Clang
   - macOS: Portable Clang (if Xcode not available)
3. Cache downloaded compiler in `tools/compilers/`
4. Automatically configure CMake to use embedded compiler
5. Verify compiler works before building

**Implementation Steps:**
1. Create compiler manager scripts
2. Add compiler download URLs and checksums
3. Implement extraction and verification
4. Integrate with setup.bat/setup.sh
5. Configure CMake toolchain files
6. Update documentation
7. Test on all platforms

### Files to Create
- `tools/compiler-manager.sh` - Unix compiler manager
- `tools/compiler-manager.bat` - Windows compiler manager
- `tools/compiler-manager.ps1` - Windows PowerShell version
- `tools/toolchains/` - CMake toolchain files for embedded compilers
- `docs/EMBEDDED_COMPILER.md` - Documentation

### Estimated Sizes
- Scripts and configuration: ~50KB
- Downloaded compilers (not in Git):
  - Windows MinGW: ~200MB
  - Linux Portable GCC: ~150MB
  - macOS Portable Clang: ~300MB

---

## Priority and Timeline

### Current Task (Completed) ✅
- Cross-platform compiler detection and fix tool

### Next Steps

#### High Priority
1. **Test Adapter Extension** (Est. 2-3 days)
   - More complex, needs new infrastructure
   - Requires test framework integration
   - High value for developers

2. **Embedded Compiler System** (Est. 1-2 days)
   - Build on existing compiler detection work
   - Primarily scripting work
   - High value for end users

#### Implementation Order Recommendation
1. Complete current compiler detection PR
2. Implement test adapter extension (new PR)
3. Implement embedded compiler system (new PR)

---

## Notes

- Both features are substantial additions
- Test adapter requires C# and VS2022 SDK knowledge
- Embedded compiler needs careful licensing consideration
- Both should be optional features (existing setup should still work)
- Good documentation is critical for both
