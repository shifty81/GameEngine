# Custom Script Compiler System

## Overview

The GameEngine now includes a **Custom Script Compiler** that allows you to write and execute scripts at runtime without recompiling the C++ code. This system enables modding, custom game logic, and rapid prototyping.

## Features

- ✨ **Runtime Compilation** - Compile and execute scripts on-the-fly
- 🔧 **Engine Integration** - Direct access to engine systems (camera, rendering, etc.)
- 📝 **Simple Syntax** - Easy-to-learn command-based scripting language
- 🎮 **Game Logic Extension** - Add custom behaviors without C++ knowledge
- 🔌 **Extensible** - Easy to add new commands and functions
- 💾 **Variable System** - Store and retrieve persistent values
- 📦 **File Support** - Load scripts from `.script` files

## Script Syntax

Scripts use a simple command-based syntax:

```
command argument1 argument2 argument3
```

### Comments

Lines starting with `#` or `;` are treated as comments:

```bash
# This is a comment
; This is also a comment
print Hello World  # This prints "Hello World"
```

### Variables

Variables can be set and retrieved using `set` and `get` commands:

```bash
# Set a variable
set player_health 100
set player_name "John Doe"

# Get a variable value
get player_health
```

Variables are stored as strings but can represent any value. You can reference variables in commands using the `$` prefix:

```bash
set x 50.0
setcam $x 30.0 50.0
```

## Built-in Commands

### General Commands

| Command | Description | Usage |
|---------|-------------|-------|
| `print` | Print text to console | `print Hello World` |
| `echo` | Same as print | `echo Message here` |
| `set` | Set a variable | `set varname value` |
| `get` | Display variable value | `get varname` |

### Engine Commands

| Command | Description | Usage |
|---------|-------------|-------|
| `wireframe` | Toggle wireframe rendering | `wireframe` |
| `cellshading` | Toggle cell shading | `cellshading [on\|off]` |
| `setcam` | Set camera position | `setcam <x> <y> <z>` |
| `getcam` | Display camera position | `getcam` |

## Script Files

### Creating a Script File

Create a text file with the `.script` extension in the `scripts/` directory:

**scripts/example.script:**
```bash
# Example Script
print Starting example script

# Set up variables
set quality high
set fps_target 60

# Control the camera
setcam 100.0 50.0 100.0
getcam

# Toggle rendering modes
cellshading on
wireframe

print Script completed!
```

### Loading Scripts

Scripts can be loaded in two ways:

1. **Automatic Startup Script**: Place a script at `scripts/startup.script` and it will run automatically when the engine starts.

2. **Console Command**: Use the debug console (press `` ` `` key) and type:
   ```
   loadscript scripts/example.script
   ```

3. **Direct Command Execution**: Execute individual commands via the console:
   ```
   script print Hello from console!
   script setcam 75.0 40.0 75.0
   ```

## Using the Script System in Debug Console

Press the `` ` `` (grave/tilde) key to open the debug console.

### Execute a Single Script Command

```
script print Hello World
script setcam 50 30 50
script cellshading on
```

### Load and Run a Script File

```
loadscript scripts/demo.script
loadscript scripts/startup.script
```

## Examples

### Example 1: Camera Animation Script

**scripts/camera_tour.script:**
```bash
# Camera Tour Script
print Starting camera tour...

set tour_step 1

print Moving to position 1
setcam 0.0 50.0 0.0

print Moving to position 2
setcam 100.0 50.0 0.0

print Moving to position 3
setcam 100.0 50.0 100.0

print Tour complete!
```

### Example 2: Rendering Configuration

**scripts/graphics_quality.script:**
```bash
# Graphics Quality Configuration
print Configuring graphics settings...

# Set quality level
set quality_level ultra

# Enable effects
cellshading on

print Graphics configured for ultra quality
```

### Example 3: Development Helper

**scripts/dev_helper.script:**
```bash
# Development Helper Script
print Development mode activated

# Set to wireframe for debugging geometry
wireframe

# Position camera at dev view
setcam 50.0 100.0 50.0

# Store development variables
set debug_mode true
set show_colliders true

print Development helper loaded
```

## Extending the Script Compiler

### Adding Custom Commands from C++

To add new commands to the script system, register them in `main.cpp`:

```cpp
// Get the script compiler
auto& scriptCompiler = ScriptCompilerManager::GetInstance().GetCompiler();

// Register a custom function
scriptCompiler.RegisterFunction("mycommand", [](const std::vector<std::string>& args) {
    std::cout << "My custom command executed!" << std::endl;
    
    // Access arguments
    if (!args.empty()) {
        std::cout << "First argument: " << args[0] << std::endl;
    }
});
```

### Example: Adding a "spawn" Command

```cpp
scriptCompiler.RegisterFunction("spawn", [](const std::vector<std::string>& args) {
    if (args.size() >= 4) {
        std::string objectType = args[0];
        float x = std::stof(args[1]);
        float y = std::stof(args[2]);
        float z = std::stof(args[3]);
        
        // Your spawn logic here
        std::cout << "Spawning " << objectType << " at (" 
                  << x << ", " << y << ", " << z << ")" << std::endl;
    }
});
```

Now you can use it in scripts:
```bash
spawn tree 50.0 0.0 50.0
spawn rock 60.0 0.0 60.0
```

## Advanced Features

### Variable Substitution

Use `$variablename` to substitute variables in commands:

```bash
set x 100.0
set y 50.0
set z 100.0

setcam $x $y $z
```

### Script Chains

Execute multiple related commands:

```bash
# Setup script
set game_mode adventure
cellshading on
setcam 50 30 50
print Game initialized in $game_mode mode
```

## API Reference

### ScriptCompiler Class

The main compiler class. Access via `ScriptCompilerManager::GetInstance().GetCompiler()`.

**Methods:**

- `bool CompileAndExecute(const std::string& script)` - Compile and execute a script string
- `bool CompileFile(const std::string& filename)` - Load and execute a script file
- `void RegisterFunction(const std::string& name, ScriptFunction function)` - Register a new command
- `void SetVariable(const std::string& name, const std::string& value)` - Set a variable
- `std::string GetVariable(const std::string& name)` - Get a variable value
- `bool HasVariable(const std::string& name)` - Check if variable exists
- `void ClearVariables()` - Clear all variables
- `void SetVerbose(bool enabled)` - Enable verbose logging

### ScriptCompilerManager Class

Singleton manager for the script system.

**Methods:**

- `static ScriptCompilerManager& GetInstance()` - Get singleton instance
- `ScriptCompiler& GetCompiler()` - Get the compiler instance
- `void Initialize()` - Initialize the system
- `bool LoadScript(const std::string& filename)` - Load a script file
- `bool ExecuteCommand(const std::string& command)` - Execute a command

## Performance Considerations

- Scripts are interpreted, not compiled to machine code
- Suitable for configuration, setup, and event handling
- Not recommended for per-frame logic or intensive calculations
- Best used for initialization, level setup, and user commands

## Best Practices

1. **Use startup scripts** for game configuration
2. **Keep scripts simple** - one command per line
3. **Comment your scripts** for maintainability
4. **Test incrementally** - test each command before adding more
5. **Use meaningful variable names** - `player_health` not `ph`
6. **Organize scripts by purpose** - separate files for different features

## Troubleshooting

### Script not executing

- Check file path is correct (relative to executable)
- Ensure file has `.script` extension
- Verify commands are spelled correctly
- Check for syntax errors (unmatched quotes, etc.)

### Command not found

- Ensure the command is registered (check available commands)
- Verify spelling and case sensitivity
- Check if command requires specific arguments

### Variable not working

- Make sure variable is set before using
- Use `$` prefix when referencing variables: `$myvar`
- Variables are case-sensitive

## Future Enhancements

Potential future additions to the script system:

- Control flow (if/else statements)
- Loops (for/while)
- Functions/procedures
- Math expression evaluation
- Event system integration
- Networking commands
- File I/O operations

## See Also

- [DEBUG_CONSOLE_GUIDE.md](DEBUG_CONSOLE_GUIDE.md) - Debug console usage
- [ENGINE_SYSTEMS.md](ENGINE_SYSTEMS.md) - Complete engine documentation
- [README.md](README.md) - General engine documentation
