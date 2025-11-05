# Quick Start Guide - Windows

This guide will help you build and run the Game Engine on Windows 10/11.

## Prerequisites Installation

### 1. Install Visual Studio 2022

Download Visual Studio 2022 Community (Free):
https://visualstudio.microsoft.com/downloads/

**Required Workloads:**
- Desktop development with C++
- .NET desktop development

**Optional but Recommended:**
- C++ profiling tools
- C++ CMake tools

### 2. Install .NET 9 SDK

Download .NET 9 SDK:
https://dotnet.microsoft.com/download/dotnet/9.0

Or via command line:
```powershell
winget install Microsoft.DotNet.SDK.9
```

Verify installation:
```powershell
dotnet --version
# Should show 9.0.x
```

### 3. Verify DirectX 11

Windows 10/11 includes DirectX 11 by default. To verify:

```powershell
dxdiag
```

Look for "DirectX 12" or "DirectX 11" in the System tab.

## Building the Project

### Method 1: Visual Studio (Recommended)

1. **Clone the repository:**
   ```powershell
   git clone --recursive https://github.com/shifty81/GameEngine.git
   cd GameEngine
   ```

2. **Open in Visual Studio:**
   - Double-click `GameEngine.sln`
   - Or: Right-click → "Open with Visual Studio 2022"

3. **Set Configuration:**
   - Configuration: `Release` (or `Debug` for development)
   - Platform: `x64`

4. **Build:**
   - Press `Ctrl+Shift+B` or `F7`
   - Or: Build → Build Solution

5. **Run:**
   - Right-click `GameEngine.Editor` project → Set as Startup Project
   - Press `F5` to run with debugger
   - Or `Ctrl+F5` to run without debugger

### Method 2: PowerShell Scripts

1. **Clone the repository:**
   ```powershell
   git clone --recursive https://github.com/shifty81/GameEngine.git
   cd GameEngine
   ```

2. **Build:**
   ```powershell
   .\build.ps1
   ```

3. **Run:**
   ```powershell
   .\run.ps1
   ```

### Method 3: Command Line (MSBuild)

1. **Open Developer PowerShell for VS 2022**

2. **Build:**
   ```powershell
   msbuild GameEngine.sln /p:Configuration=Release /p:Platform=x64
   ```

3. **Run:**
   ```powershell
   dotnet run --project src\GameEngine.Editor\GameEngine.Editor.csproj -c Release
   ```

## Verifying the Installation

Once the editor launches, you should see:

1. **Main Window**
   - Title: "Game Engine Editor - .NET 9 + C++20"
   - Dockable panels (Hierarchy, Viewport, Properties, Console)

2. **Console Output**
   - "Engine initialized successfully"
   - "Generated initial terrain"
   - "Engine running" in status bar

3. **3D Viewport**
   - Voxel terrain should be visible
   - Sky gradient background

4. **Test Controls:**
   - `W/A/S/D` - Move camera
   - `Space` - Move up
   - `Shift` - Move down
   - Mouse should show camera position updating

## Testing Functionality

### 1. Test Camera Movement
```
1. Focus on viewport
2. Press W to move forward
3. Watch camera position update in toolbar
4. Try all WASD keys, Space, and Shift
```

### 2. Test Console Commands
```
1. Click in Console input box at bottom
2. Type: terrain 54321
3. Press Enter
4. Watch terrain regenerate with new seed
```

### 3. Test Editor Mode Toggle
```
1. Click "Edit" menu → "Editor Mode"
2. Or click "Editor Mode" toolbar button
3. Console should show "Editor mode: ON/OFF"
```

### 4. Test Terrain Generation
```
1. Click "Edit" menu → "Generate Terrain"
2. Console shows new seed
3. Terrain regenerates
```

### 5. Test Properties Panel
```
1. Look at Properties panel on right
2. See Transform controls
3. See Camera Settings
4. See Voxel Tools
```

## Common Issues and Solutions

### Issue: "Failed to initialize engine"

**Cause**: DirectX 11 not available or GPU driver issue

**Solutions:**
1. Update GPU drivers:
   - NVIDIA: https://www.nvidia.com/Download/index.aspx
   - AMD: https://www.amd.com/en/support
   - Intel: https://www.intel.com/content/www/us/en/download-center/home.html

2. Check DirectX:
   ```powershell
   dxdiag
   ```

3. Try Debug mode for more info:
   - Open Visual Studio
   - Set Configuration to Debug
   - Press F5 and check Output window

### Issue: "GameEngine.Core.dll not found"

**Cause**: C++ DLL not built or in wrong location

**Solutions:**
1. Rebuild C++ project:
   ```powershell
   msbuild src\GameEngine.Core\GameEngine.Core.vcxproj /p:Configuration=Release /p:Platform=x64
   ```

2. Check DLL exists:
   ```powershell
   Test-Path bin\Release\x64\GameEngine.Core.dll
   ```

3. Copy DLL manually if needed:
   ```powershell
   Copy-Item bin\Release\x64\GameEngine.Core.dll src\GameEngine.Editor\bin\Release\net9.0-windows\
   ```

### Issue: ".NET 9 SDK not found"

**Cause**: .NET 9 not installed or not in PATH

**Solutions:**
1. Install .NET 9:
   ```powershell
   winget install Microsoft.DotNet.SDK.9
   ```

2. Verify installation:
   ```powershell
   dotnet --version
   ```

3. Restart PowerShell/VS after install

### Issue: "MSBuild not found"

**Cause**: Visual Studio not installed or not in PATH

**Solutions:**
1. Install Visual Studio 2022 with C++ workload
2. Use Developer PowerShell for VS 2022
3. Or add MSBuild to PATH:
   ```powershell
   $env:Path += ";C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin"
   ```

### Issue: Build errors in Visual Studio

**Cause**: Missing dependencies or wrong configuration

**Solutions:**
1. Clean and rebuild:
   - Build → Clean Solution
   - Build → Rebuild Solution

2. Verify platform is x64:
   - Configuration Manager → Platform: x64

3. Update NuGet packages:
   - Right-click solution → Restore NuGet Packages

4. Check Windows SDK version:
   - Project properties → General → Windows SDK Version
   - Should be 10.0 or higher

## Performance Expectations

### Minimum System:
- CPU: Intel i5 / AMD Ryzen 5
- GPU: Integrated graphics with DX11 support
- RAM: 4 GB
- Expected FPS: 30-60

### Recommended System:
- CPU: Intel i7 / AMD Ryzen 7
- GPU: Dedicated GPU (GTX 1060 / RX 580 or better)
- RAM: 8 GB
- Expected FPS: 60+ (VSync limited)

## Next Steps

After verifying the engine works:

1. **Explore the Editor:**
   - Try different console commands
   - Move camera around terrain
   - Toggle editor mode
   - Generate terrain with different seeds

2. **Read Documentation:**
   - README_WINDOWS.md - Detailed project info
   - ROADMAP.md - Future features planned
   - Review code in Visual Studio

3. **Start Developing:**
   - Add new block types in VoxelChunk.h
   - Modify terrain generation in VoxelChunk.cpp
   - Add UI panels in MainWindow.xaml
   - Extend C++ API in EngineCore.h

4. **Report Issues:**
   - GitHub Issues: Report bugs or request features
   - Include console output and system specs

## Development Workflow

### Daily Development:
```powershell
# 1. Update from git
git pull

# 2. Build
.\build.ps1

# 3. Run with debugger
# Press F5 in Visual Studio

# 4. Make changes to C# code
# Visual Studio hot reload applies changes

# 5. For C++ changes, rebuild:
# Ctrl+Shift+B in Visual Studio
```

### Before Committing:
```powershell
# 1. Clean build
msbuild /t:Clean,Build

# 2. Test in Release mode
.\run.ps1

# 3. Commit
git add .
git commit -m "Your changes"
git push
```

## Getting Help

- **Documentation**: README_WINDOWS.md, ROADMAP.md
- **GitHub Issues**: https://github.com/shifty81/GameEngine/issues
- **Console Help**: Type `help` in console
- **Code Comments**: Review source code comments

## Success!

If you see the editor with terrain rendering, congratulations! You have successfully:
- ✅ Built the C++ core engine
- ✅ Built the .NET 9 WPF editor
- ✅ Launched the hybrid application
- ✅ Verified DirectX 11 rendering
- ✅ Tested C++/C# interop

You're ready to start developing features! Check ROADMAP.md for the comprehensive feature plan.
