# Scripts Directory

This directory contains script files for the GameEngine's custom script compiler system.

## Overview

The script compiler allows you to write and execute custom scripts at runtime without recompiling the C++ code. Scripts use a simple command-based syntax and can interact with engine systems.

## File Format

- Scripts should have a `.script` file extension
- Lines starting with `#` or `;` are comments
- Each line contains one command with arguments

## Included Scripts

### startup.script
Automatically executed when the engine starts (if it exists). Use this for initialization and configuration.

**Example:**
```bash
# Startup configuration
print Engine Started!
cellshading on
```

### demo.script
Demonstrates the script compiler features. Load with `loadscript scripts/demo.script` in the debug console.

**Example:**
```bash
print === Demo Script ===
setcam 50.0 30.0 50.0
wireframe
```

## Creating Your Own Scripts

1. Create a new `.script` file in this directory
2. Add commands (one per line)
3. Add comments for documentation
4. Load via debug console: `loadscript scripts/yourscript.script`

## Available Commands

### Basic Commands
- `print <text>` - Print message to console
- `echo <text>` - Same as print
- `set <name> <value>` - Set a variable
- `get <name>` - Display variable value

### Engine Commands
- `wireframe` - Toggle wireframe mode
- `cellshading [on|off]` - Toggle cell shading
- `setcam <x> <y> <z>` - Set camera position
- `getcam` - Display camera position

## Variable System

Variables can be stored and referenced:

```bash
# Set variables
set player_name "Hero"
set player_health 100

# Reference variables with $
set x 50.0
setcam $x 30.0 50.0
```

## Loading Scripts

### Method 1: Automatic Startup
Rename your script to `startup.script` and it will run automatically when the engine starts.

### Method 2: Debug Console
Press `` ` `` to open the console and type:
```
loadscript scripts/yourscript.script
```

### Method 3: Script Command
Execute individual commands:
```
script print Hello World
script setcam 75 40 75
```

## Tips

- Use comments to document your scripts
- Test commands individually in the console first
- Keep scripts simple - one command per line
- Check SCRIPT_COMPILER.md for complete documentation
- Variables persist during the engine session

## Examples

### Configuration Script
```bash
# config.script - Game configuration
print Loading configuration...
set quality ultra
set fps_target 60
cellshading on
print Configuration loaded
```

### Camera Tour Script
```bash
# tour.script - Camera tour
print Starting tour...
setcam 0 50 0
# Wait/pause would go here (not implemented yet)
setcam 100 50 100
print Tour complete
```

### Debug Script
```bash
# debug.script - Debug helpers
print Debug mode enabled
wireframe
setcam 50 100 50
set debug_mode true
```

## Documentation

For complete documentation, see:
- **[SCRIPT_COMPILER.md](../SCRIPT_COMPILER.md)** - Full scripting guide
- **[DEBUG_CONSOLE_GUIDE.md](../DEBUG_CONSOLE_GUIDE.md)** - Console commands
- **[README.md](../README.md)** - Main documentation

## Troubleshooting

**Script not loading:**
- Check file path is correct
- Ensure `.script` extension
- Verify commands are spelled correctly

**Command not found:**
- Check command spelling
- See available commands: `help` in console
- Refer to SCRIPT_COMPILER.md

**Variable not working:**
- Use `$` prefix when referencing: `$myvar`
- Variables are case-sensitive
- Set variable before using: `set myvar value`
