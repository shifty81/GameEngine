# Quick Start Guide: Custom Compiler & One-Click Build

## For First-Time Users on Windows

### Building the Engine - One Click!

1. **Download** or clone the repository
2. **Double-click** `OneClickBuild.bat`
3. **Follow** any prompts if prerequisites are missing
4. **Wait** for build to complete (10-15 minutes first time)
5. **Run** with `run.bat` or `build\bin\Release\GameEngine.exe`

That's it! Everything is automated.

## Using Scripts

### Quick Start

1. **Press** `` ` `` (grave/tilde key) to open debug console
2. **Type** script commands:
   ```
   script print Hello World
   script setcam 50 30 50
   script wireframe
   ```

### Example Scripts

**scripts/startup.script** - Runs automatically at engine start:
```bash
# My startup config
print Welcome to GameEngine!
cellshading on
```

**scripts/demo.script** - Load with `loadscript scripts/demo.script`:
```bash
print === Demo ===
setcam 50 30 50
wireframe
getcam
```

### Basic Commands

| Command | What It Does | Example |
|---------|--------------|---------|
| `print` | Show message | `print Hello` |
| `set` | Set variable | `set x 50` |
| `get` | Show variable | `get x` |
| `wireframe` | Toggle wireframe | `wireframe` |
| `cellshading` | Toggle cell shading | `cellshading on` |
| `setcam` | Move camera | `setcam 100 50 100` |
| `getcam` | Show camera position | `getcam` |

### Variables

```bash
# Set a variable
set myvar 100

# Use it (with $ prefix)
setcam $myvar 30 50
```

## Common Use Cases

### 1. Quick Camera Positioning
```bash
script setcam 75 40 75
script getcam
```

### 2. Toggle Rendering Modes
```bash
script wireframe
script cellshading off
```

### 3. Configuration Script
Create `scripts/myconfig.script`:
```bash
print Loading my config
cellshading on
setcam 50 100 50
set quality ultra
```

Load it:
```
loadscript scripts/myconfig.script
```

## Need Help?

- **Script Compiler**: See [SCRIPT_COMPILER.md](SCRIPT_COMPILER.md)
- **One-Click Build**: See [ONECLICK_BUILD.md](ONECLICK_BUILD.md)
- **General Help**: See [README.md](README.md)
- **Troubleshooting**: See [TROUBLESHOOTING.md](TROUBLESHOOTING.md)

## Tips

- Test commands in console before putting them in scripts
- Use comments in scripts (lines starting with # or ;)
- Scripts run line by line
- Variables persist during engine session
- Press `` ` `` to toggle console on/off

## Advanced

### Creating New Commands

Developers can add custom commands in `main.cpp`:

```cpp
scriptCompiler.RegisterFunction("mycommand", [](const std::vector<std::string>& args) {
    std::cout << "My custom command!" << std::endl;
});
```

### Extending the Build Script

Modify `OneClickBuild.bat` to add custom build steps:

```batch
REM Add your custom step here
echo Running custom build step...
```

## What's Next?

1. Try the example scripts in `scripts/` directory
2. Create your own scripts
3. Explore engine features (press F for wireframe, C for cell shading)
4. Check out [ENGINE_SYSTEMS.md](ENGINE_SYSTEMS.md) for more features
5. Join the community and share your creations!

---

**Quick Links:**
- 📖 [Full Script Guide](SCRIPT_COMPILER.md)
- 🚀 [Build Guide](ONECLICK_BUILD.md)
- 🎮 [Main README](README.md)
- 🛠️ [Troubleshooting](TROUBLESHOOTING.md)
