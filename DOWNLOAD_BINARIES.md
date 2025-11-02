# Download Pre-built Binaries

**Don't want to compile from source?** Download pre-built binaries and start using the GameEngine immediately!

## 🎮 Quick Download

### Latest Release

Visit the [Releases page](https://github.com/shifty81/GameEngine/releases/latest) to download the latest pre-built version.

**Available Platforms:**
- 🪟 **Windows** (64-bit): `GameEngine-Windows-x64.zip`
- 🐧 **Linux** (64-bit): `GameEngine-Linux-x64.tar.gz`

### Development/Test Binaries

Want to test the latest development version? 

Every commit is automatically built and tested. Test binaries are available from the **Actions** tab for 7 days.

👉 **[See TESTING_BINARIES.md](TESTING_BINARIES.md)** for instructions on accessing test builds.

---

## Windows Installation

### Step 1: Download

1. Go to [Releases](https://github.com/shifty81/GameEngine/releases/latest)
2. Download `GameEngine-Windows-x64.zip`
3. Extract the ZIP file to a folder (e.g., `C:\GameEngine\`)

### Step 2: Install Visual C++ Redistributable (If Needed)

The GameEngine requires Microsoft Visual C++ Redistributable 2022.

**Check if you need it:**
- Try running `GameEngine.exe`
- If you see an error about missing DLLs (VCRUNTIME140.dll, MSVCP140.dll), you need to install it

**Install Visual C++ Redistributable:**
1. Download from: https://aka.ms/vs/17/release/vc_redist.x64.exe
2. Run the installer
3. Follow the installation prompts
4. Restart your computer if prompted

### Step 3: Run the Engine

**Option A: Using the batch script (Recommended)**
```
Double-click run.bat
```

**Option B: Running directly**
```
Double-click GameEngine.exe
```

**Expected result:** A window should open showing a procedurally generated 3D world!

### System Requirements (Windows)

- **Operating System:** Windows 10 (64-bit) or Windows 11
- **Graphics Card:** GPU with OpenGL 3.3 or higher support
  - Most GPUs from 2010 or newer support OpenGL 3.3
  - Update your graphics drivers if you encounter issues
- **RAM:** 2GB minimum, 4GB recommended
- **Disk Space:** 100MB for the engine + assets
- **Dependencies:** Microsoft Visual C++ Redistributable 2022 (x64)

### Troubleshooting (Windows)

#### "The code execution cannot proceed because VCRUNTIME140.dll was not found"
**Solution:** Install Visual C++ Redistributable 2022 (see Step 2 above)

#### Black screen or window doesn't open
**Solution:** 
1. Update your graphics drivers:
   - NVIDIA: https://www.nvidia.com/Download/index.aspx
   - AMD: https://www.amd.com/en/support
   - Intel: https://www.intel.com/content/www/us/en/download-center/home.html
2. Your GPU may not support OpenGL 3.3+

#### "Windows protected your PC" security warning
**Solution:** 
1. Click "More info"
2. Click "Run anyway"
3. This warning appears because the executable isn't signed with a certificate

---

## Linux Installation

### Step 1: Download

1. Go to [Releases](https://github.com/shifty81/GameEngine/releases/latest)
2. Download `GameEngine-Linux-x64.tar.gz`
3. Extract the archive:
   ```bash
   tar -xzf GameEngine-Linux-x64.tar.gz
   cd GameEngine-Linux
   ```

### Step 2: Install Dependencies

The GameEngine requires OpenGL and X11 libraries:

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install libgl1-mesa-glx libglu1-mesa libx11-6 libxrandr2 \
                     libxinerama1 libxcursor1 libxi6
```

**Fedora/RHEL:**
```bash
sudo dnf install mesa-libGL mesa-libGLU libX11 libXrandr libXinerama \
                 libXcursor libXi
```

**Arch Linux:**
```bash
sudo pacman -S mesa libx11 libxrandr libxinerama libxcursor libxi
```

### Step 3: Run the Engine

**Option A: Using the shell script (Recommended)**
```bash
./run.sh
```

**Option B: Running directly**
```bash
./GameEngine
```

**Expected result:** A window should open showing a procedurally generated 3D world!

### System Requirements (Linux)

- **Operating System:** Ubuntu 20.04+ or equivalent distribution (64-bit)
- **Graphics Card:** GPU with OpenGL 3.3 or higher support
- **RAM:** 2GB minimum, 4GB recommended
- **Disk Space:** 100MB for the engine + assets
- **Dependencies:** OpenGL libraries, X11 libraries (see Step 2)

### Troubleshooting (Linux)

#### "error while loading shared libraries: libGL.so.1"
**Solution:** Install OpenGL libraries (see Step 2)

#### "error while loading shared libraries: libX11.so.6"
**Solution:** Install X11 libraries (see Step 2)

#### Permission denied when running
**Solution:** Make the executable runnable:
```bash
chmod +x GameEngine
chmod +x run.sh
./run.sh
```

#### Black screen or window doesn't open
**Solution:**
1. Update your graphics drivers
2. Check OpenGL support: `glxinfo | grep "OpenGL version"`
   - Should show version 3.3 or higher
   - If command not found, install: `sudo apt-get install mesa-utils`

---

## Controls

Once the engine is running:

| Key | Action |
|-----|--------|
| **W** | Move forward |
| **A** | Move left |
| **S** | Move backward |
| **D** | Move right |
| **Space** | Fly up |
| **Left Shift** | Fly down |
| **Mouse** | Look around |
| **F** | Toggle wireframe mode |
| **C** | Toggle cell shading |
| **`** (Grave/Tilde) | Toggle debug console |
| **ESC** | Exit application |

### Debug Console Commands

Press **`** (grave/tilde key) to open the debug console, then try these commands:

- `help` - List all commands
- `wireframe` - Toggle wireframe rendering
- `cellshading on/off` - Toggle cell shading
- `setcam <x> <y> <z>` - Set camera position
- `getcam` - Show current camera position
- `clear` - Clear console output

---

## What's Included

The pre-built binary package includes:

- ✅ **GameEngine executable** - Fully compiled and optimized
- ✅ **Assets folder** - All required 3D models and textures
- ✅ **Documentation** - README, QUICKSTART, and LICENSE files
- ✅ **Run scripts** - Easy-to-use batch/shell scripts

**What's NOT included:**
- ❌ Source code (download from [repository](https://github.com/shifty81/GameEngine))
- ❌ Development tools (Visual Studio, CMake, etc.)
- ❌ Debug symbols

---

## Features

Experience these features out of the box:

- 🎨 **Cell Shading** - Cartoon-style rendering
- 🏔️ **Procedural Terrain** - Infinite landscapes generated with Perlin noise
- ⛰️ **Cave Systems** - Underground caverns to explore
- 💧 **Water System** - Animated flowing water
- 🌬️ **Wind System** - Dynamic environmental effects
- 🎮 **Full Controls** - WASD movement with mouse look
- 🐛 **Debug Console** - Built-in command console for tweaking
- 🖼️ **Advanced Rendering** - PBR materials, lighting, post-processing

---

## For Developers

### Want to Modify or Build From Source?

If you want to:
- Modify the engine code
- Add new features
- Contribute to development
- Build the latest development version

See the development setup guides:
- **[README.md](README.md)** - Complete build instructions
- **[VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md)** - Detailed Visual Studio installation guide
- **[QUICKSTART.md](QUICKSTART.md)** - Quick start guide for developers

**Requirements for building from source:**
- Visual Studio 2022 (v17.8 through v17.14+, all supported) or Visual Studio 2019 (v16.11+) on Windows
- GCC 10+ or Clang 11+ on Linux
- CMake 3.20+
- Git

---

## Automatic Builds

Pre-built binaries are automatically generated for every release using GitHub Actions:
- Builds are tested on clean environments
- Includes all required dependencies
- Optimized for performance (Release configuration)

**Build information:**
- Windows builds use Visual Studio 2022 with MSVC compiler
- Linux builds use GCC on Ubuntu latest
- All builds use CMake 3.20+

---

## Release Schedule

- **Stable releases:** Tagged as `vX.Y.Z` (e.g., v1.0.0)
- **Check for updates:** Visit the [Releases page](https://github.com/shifty81/GameEngine/releases)
- **Stay informed:** Watch the repository for notifications

---

## Support

### Having Issues?

1. **Check troubleshooting sections above** for your platform
2. **Update graphics drivers** - Many issues are driver-related
3. **Check system requirements** - Ensure your system meets minimum specs
4. **Review the documentation:**
   - [TROUBLESHOOTING.md](TROUBLESHOOTING.md) - Cross-platform troubleshooting
   - [WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md) - Windows-specific issues

### Still Need Help?

Open an issue on GitHub with:
1. Your operating system and version
2. Graphics card model
3. The error message or problem description
4. Steps to reproduce the issue

---

## License

This software is licensed under the MIT License. See [LICENSE](LICENSE) file for details.

---

## Credits

- **OpenGL** - Graphics API
- **GLFW** - Window and input management
- **GLM** - Mathematics library
- **Assimp** - 3D model loading
- **stb_image** - Image loading

---

**Enjoy the GameEngine! 🎮**

For source code, documentation, and more: https://github.com/shifty81/GameEngine
