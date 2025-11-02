# Ubuntu 24.04 LTS Development Guide

This guide focuses on developing and building the GameEngine on **Ubuntu 24.04 LTS (Noble Numbat)**, the latest Long-Term Support release of Ubuntu.

## Why Ubuntu 24.04 LTS?

Ubuntu 24.04 LTS is an excellent choice for C++ game engine development:

- ✅ **GCC 13.3** - Excellent C++20 support out of the box
- ✅ **Long-Term Support** - Updates and support until 2029
- ✅ **Modern Kernel** - Linux kernel 6.8+ with improved graphics support
- ✅ **Mesa 24.x** - Latest OpenGL drivers with better performance
- ✅ **Stable Base** - Perfect balance between modern features and stability
- ✅ **Wayland Support** - Modern display server with better multi-monitor handling

## Quick Start on Ubuntu 24.04 LTS

### One-Command Setup

For the fastest setup experience:

```bash
sudo apt-get update && sudo apt-get install -y build-essential cmake git \
    libgl1-mesa-dev libglu1-mesa-dev \
    libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev && \
git clone --recursive https://github.com/shifty81/GameEngine.git && \
cd GameEngine && ./build.sh && ./run.sh
```

### Step-by-Step Setup

**1. Install Development Tools**
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake git
```

This installs:
- `build-essential` - GCC 13.3, g++, make, and essential build tools
- `cmake` - Build system generator (version 3.28+)
- `git` - Version control system

**2. Install Graphics Libraries**
```bash
sudo apt-get install -y libgl1-mesa-dev libglu1-mesa-dev
```

This installs:
- OpenGL development libraries
- Mesa 3D graphics library headers
- GLU (OpenGL Utility Library)

**3. Install X11 Libraries**
```bash
sudo apt-get install -y libx11-dev libxrandr-dev libxinerama-dev \
                        libxcursor-dev libxi-dev
```

These are required by GLFW for window management:
- `libx11-dev` - X11 core library
- `libxrandr-dev` - X11 RandR extension (monitor configuration)
- `libxinerama-dev` - X11 Xinerama extension (multi-monitor)
- `libxcursor-dev` - X11 cursor management
- `libxi-dev` - X11 input extension (mouse, keyboard)

**4. Clone and Build**
```bash
# Clone repository with all submodules
git clone --recursive https://github.com/shifty81/GameEngine.git
cd GameEngine

# Run automated setup (optional but recommended)
./setup.sh

# Build the engine
./build.sh

# Run the engine
./run.sh
```

## Verified System Configuration

The GameEngine has been fully tested on Ubuntu 24.04 LTS with the following configuration:

### Software Versions
- **OS**: Ubuntu 24.04.3 LTS (Noble Numbat)
- **Kernel**: Linux 6.8+
- **GCC**: 13.3.0 (with full C++20 support)
- **CMake**: 3.28+
- **Mesa**: 24.x
- **X11**: 1.21+

### Compiler Features
GCC 13.3 on Ubuntu 24.04 supports all required C++20 features:
- ✅ Concepts
- ✅ Ranges
- ✅ Coroutines
- ✅ Modules (experimental)
- ✅ Three-way comparison (spaceship operator)
- ✅ std::format
- ✅ constexpr improvements

### Build Performance
On Ubuntu 24.04 LTS with modern hardware:
- **First Build**: ~5-8 minutes (includes Assimp compilation)
- **Incremental Build**: ~5-10 seconds
- **Clean Rebuild**: ~3-5 minutes

## Development Workflow on Ubuntu 24.04

### Recommended IDE Setup

**Option 1: Visual Studio Code**
```bash
# Install VS Code
sudo snap install code --classic

# Install recommended extensions
code --install-extension ms-vscode.cpptools
code --install-extension ms-vscode.cmake-tools
code --install-extension twxs.cmake
```

**Option 2: CLion**
```bash
# Download from JetBrains website or use snap
sudo snap install clion --classic
```

**Option 3: Qt Creator**
```bash
sudo apt-get install qtcreator
```

### Debugging Tools

**GDB (GNU Debugger)**
```bash
# Already included with build-essential
gdb ./build/bin/GameEngine
```

**Valgrind (Memory Debugging)**
```bash
sudo apt-get install valgrind
valgrind --leak-check=full ./build/bin/GameEngine
```

**perf (Performance Analysis)**
```bash
sudo apt-get install linux-tools-generic
perf record ./build/bin/GameEngine
perf report
```

### Graphics Debugging

**apitrace (OpenGL Tracing)**
```bash
sudo apt-get install apitrace
apitrace trace ./build/bin/GameEngine
qapitrace GameEngine.trace
```

**RenderDoc (GPU Debugger)**
```bash
# Download from https://renderdoc.org
# or build from source
```

## Optimizing for Ubuntu 24.04

### Compiler Optimization Flags

The project uses these optimizations in Release mode:
```cmake
-O3                    # Maximum optimization
-march=native         # Optimize for your CPU
-DNDEBUG             # Disable assertions
```

### GPU Driver Recommendations

**For NVIDIA GPUs:**
```bash
# Install proprietary driver for best performance
sudo ubuntu-drivers autoinstall
sudo reboot
```

**For AMD GPUs:**
```bash
# Mesa drivers are excellent on Ubuntu 24.04
# Already installed with libgl1-mesa-dev
# For latest drivers, use:
sudo add-apt-repository ppa:kisak/kisak-mesa
sudo apt-get update && sudo apt-get upgrade
```

**For Intel GPUs:**
```bash
# Mesa drivers work well with Intel
# Ensure you have the latest:
sudo apt-get install mesa-vulkan-drivers intel-media-va-driver
```

### Verify OpenGL Support

```bash
# Check OpenGL version
glxinfo | grep "OpenGL version"

# Should show: OpenGL version string: 4.6+ (or at least 3.3)

# If glxinfo is not installed:
sudo apt-get install mesa-utils
```

## Troubleshooting on Ubuntu 24.04

### Build Issues

**Problem: Missing compiler**
```bash
# Verify GCC installation
gcc --version
g++ --version

# Should show: gcc (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0
```

**Problem: Missing libraries**
```bash
# Run the automated checker
./tools/check-compiler.sh

# Or install everything at once
sudo apt-get install -y build-essential cmake git \
    libgl1-mesa-dev libglu1-mesa-dev \
    libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

### Runtime Issues

**Problem: Black screen or no window**
```bash
# Check OpenGL support
glxinfo | grep "OpenGL"

# Update graphics drivers
sudo ubuntu-drivers autoinstall  # For NVIDIA
sudo apt-get upgrade              # For AMD/Intel Mesa
```

**Problem: Poor performance**
```bash
# Check if using software rendering
glxinfo | grep "OpenGL renderer"

# Should NOT show "llvmpipe" or "software"
# If it does, your GPU drivers are not working correctly
```

**Problem: "Failed to create GLFW window"**

This error occurs when GLFW cannot create an OpenGL window context. Common causes:

1. **No display available** (headless system/server):
```bash
# Install Xvfb (X Virtual Frame Buffer)
sudo apt-get install xvfb

# Start Xvfb
Xvfb :99 -screen 0 1024x768x24 &

# Set DISPLAY environment variable
export DISPLAY=:99

# Run the engine
./run.sh
```

2. **OpenGL version not supported**:
```bash
# Check OpenGL version (requires mesa-utils)
sudo apt-get install mesa-utils
glxinfo | grep "OpenGL version"

# Should show OpenGL 3.3 or higher
# If not, update your graphics drivers
```

3. **Missing DISPLAY environment variable**:
```bash
# Check if DISPLAY is set
echo $DISPLAY

# If empty, either:
# - You're running from SSH without X forwarding
# - Use Xvfb as shown above
```

4. **Graphics driver issues**:
```bash
# For NVIDIA cards
sudo ubuntu-drivers autoinstall
sudo reboot

# For AMD/Intel (Mesa drivers)
sudo apt-get update
sudo apt-get upgrade
```

### Wayland vs X11

Ubuntu 24.04 uses Wayland by default, but the engine works on both:

**To check your session type:**
```bash
echo $XDG_SESSION_TYPE
```

**To use X11 instead:**
1. Log out
2. At login screen, click gear icon
3. Select "Ubuntu on Xorg"
4. Log in

The engine works on both Wayland and X11, but X11 may have better compatibility with older tools.

## Running in Headless Environments

For servers, CI/CD pipelines, or SSH sessions without X forwarding, you can use Xvfb (X Virtual Frame Buffer):

### Quick Setup

```bash
# Install Xvfb
sudo apt-get install xvfb mesa-utils

# Start Xvfb on display :99
Xvfb :99 -screen 0 1024x768x24 > /dev/null 2>&1 &

# Set DISPLAY variable
export DISPLAY=:99

# Verify OpenGL is available
glxinfo | grep "OpenGL version"

# Run the engine
./run.sh
```

### Automated Startup Script

Create a wrapper script `run-headless.sh`:

```bash
#!/bin/bash
# Start Xvfb if not already running
if ! pgrep -x "Xvfb" > /dev/null; then
    echo "Starting Xvfb..."
    Xvfb :99 -screen 0 1024x768x24 > /dev/null 2>&1 &
    sleep 2
fi

# Export DISPLAY
export DISPLAY=:99

# Run the game engine
./build/bin/GameEngine
```

Make it executable and run:
```bash
chmod +x run-headless.sh
./run-headless.sh
```

### Docker/Container Support

For running in containers, add Xvfb to your Dockerfile:

```dockerfile
RUN apt-get update && apt-get install -y \
    xvfb \
    libgl1-mesa-dev \
    mesa-utils

# Start Xvfb in your entrypoint
CMD Xvfb :99 -screen 0 1024x768x24 & \
    export DISPLAY=:99 && \
    ./build/bin/GameEngine
```

### GitHub Actions / CI/CD

For automated testing in CI/CD:

```yaml
- name: Setup Display
  run: |
    sudo apt-get install -y xvfb
    Xvfb :99 -screen 0 1024x768x24 &
    echo "DISPLAY=:99" >> $GITHUB_ENV

- name: Run Engine
  run: ./run.sh
```

## Performance Tips

### CPU Optimization
```bash
# Use all CPU cores for building
./build.sh  # Already uses make -j$(nproc)

# Or specify core count manually
cd build && make -j8
```

### Memory Management
```bash
# Monitor memory usage
watch -n 1 free -h

# The engine uses ~200-500MB RAM typically
```

### Graphics Performance
```bash
# Enable triple buffering for smoother rendering
export vblank_mode=0  # Disable vsync (for testing)
./run.sh

# Or force vsync on
export vblank_mode=1
./run.sh
```

## Contributing from Ubuntu 24.04

All development tools needed for contributions are available on Ubuntu 24.04:

```bash
# Code formatting
sudo apt-get install clang-format
clang-format -i src/*.cpp src/*.h

# Static analysis
sudo apt-get install cppcheck
cppcheck --enable=all src/

# Documentation
sudo apt-get install doxygen graphviz
doxygen Doxyfile
```

## Additional Resources

- [Ubuntu 24.04 Release Notes](https://discourse.ubuntu.com/t/noble-numbat-release-notes/39890)
- [GCC 13 Release Series](https://gcc.gnu.org/gcc-13/)
- [Mesa 24.0 Release Notes](https://docs.mesa3d.org/relnotes/24.0.0.html)
- [CMake Documentation](https://cmake.org/documentation/)

## Support

If you encounter issues on Ubuntu 24.04:

1. Check [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for common solutions
2. Run `./tools/check-compiler.sh` for automated diagnostics
3. Open an issue on GitHub with:
   - Ubuntu version: `lsb_release -a`
   - GCC version: `gcc --version`
   - CMake version: `cmake --version`
   - Build log output

## Summary

Ubuntu 24.04 LTS provides an excellent, stable, and modern platform for C++ game engine development. With GCC 13.3's robust C++20 support and Mesa 24.x's improved OpenGL implementation, you have everything needed for professional game development on Linux.

**Happy coding on Ubuntu! 🐧🎮**
