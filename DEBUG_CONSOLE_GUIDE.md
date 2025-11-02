# Debug Console Usage Guide

This guide demonstrates how to use the powerful debug console system in the game engine.

## Overview

The Debug Console is a runtime command system that allows developers to:
- Execute commands while the game is running
- Inspect and modify game state
- Toggle rendering modes
- Control camera position
- Set and get variables
- Create custom commands for debugging

## Accessing the Console

### Opening/Closing
- Press **` (Grave/Tilde key)** to toggle the console on/off
- When open, the console captures keyboard input
- Camera and game controls are disabled while console is active
- Press ` again to close and resume normal gameplay

### Console State
- Console output appears in the terminal/command window
- Recent log entries (last 10) are displayed
- Current input line is shown with a cursor
- Command history is preserved across sessions

## Built-in Commands

### Help System
```
> help
```
Lists all available commands with descriptions.

```
> help <command>
```
Shows detailed help for a specific command.

### Console Management
```
> clear
```
Clears the console output buffer.

```
> exit
> quit
```
Closes the console (same as pressing ` key).

### Variable System
```
> set <name> <value>
```
Sets a console variable to a value.
Example:
```
> set debug_level 3
> set player_name "John Doe"
> set gravity 9.81
```

```
> get <name>
```
Retrieves and displays a variable value.
Example:
```
> get debug_level
debug_level = 3
```

```
> listvars
```
Lists all currently defined console variables.

### Output
```
> echo <text>
```
Prints text to the console.
Example:
```
> echo Hello World
Hello World
```

## Game-Specific Commands

### Graphics Controls

#### Wireframe Mode
```
> wireframe
```
Toggles wireframe rendering on/off.

#### Cell Shading
```
> cellshading
```
Toggles cell shading (toon rendering) on/off.

```
> cellshading on
> cellshading off
```
Explicitly enables or disables cell shading.

### Camera Controls

#### Set Camera Position
```
> setcam <x> <y> <z>
```
Moves the camera to the specified world coordinates.
Example:
```
> setcam 100 50 100
[Console] Camera moved to (100, 50, 100)
```

#### Get Camera Position
```
> getcam
```
Displays the current camera position.
Example:
```
> getcam
[Console] Camera position: (100.000000, 50.000000, 100.000000)
```

### System Information
```
> sysinfo
```
Displays system and engine information.

## Command History

### Navigation
- Press **Up Arrow** to cycle through previous commands
- Press **Down Arrow** to cycle forward through command history
- History stores up to 50 commands
- Duplicate consecutive commands are not stored

### Usage Example
```
> setcam 50 30 50
> getcam
> [Press Up Arrow]  # Shows: getcam
> [Press Up Arrow]  # Shows: setcam 50 30 50
> [Press Down Arrow] # Shows: getcam
```

## Creating Custom Commands

### Basic Command Registration

In your game code, register custom commands:

```cpp
#include "DebugConsole.h"

// Get console instance
auto console = DebugConsole::GetInstance();

// Register a simple command
console->RegisterCommand("spawn", "Spawn an entity at camera position",
    [](const std::vector<std::string>& args) {
        // Your spawn logic here
        glm::vec3 pos = camera.Position;
        SpawnEntity(pos);
        DebugConsole::GetInstance()->Log("[Game] Entity spawned at camera position");
    });
```

### Command with Arguments

```cpp
console->RegisterCommand("spawn_at", "Spawn entity at position (x y z)",
    [](const std::vector<std::string>& args) {
        if (args.size() < 3) {
            DebugConsole::GetInstance()->Log("[Error] Usage: spawn_at <x> <y> <z>");
            return;
        }
        
        float x = std::stof(args[0]);
        float y = std::stof(args[1]);
        float z = std::stof(args[2]);
        
        SpawnEntity(glm::vec3(x, y, z));
        DebugConsole::GetInstance()->Log("[Game] Entity spawned at (" + 
            args[0] + ", " + args[1] + ", " + args[2] + ")");
    });
```

### Command with Game State Access

```cpp
// Global or static access to game state
extern Player* g_Player;
extern GameWorld* g_World;

console->RegisterCommand("godmode", "Toggle invincibility",
    [](const std::vector<std::string>& args) {
        if (g_Player) {
            g_Player->invincible = !g_Player->invincible;
            DebugConsole::GetInstance()->Log("[Game] God mode: " + 
                std::string(g_Player->invincible ? "ON" : "OFF"));
        }
    });

console->RegisterCommand("teleport", "Teleport to a waypoint",
    [](const std::vector<std::string>& args) {
        if (args.empty()) {
            DebugConsole::GetInstance()->Log("[Error] Usage: teleport <waypoint_name>");
            return;
        }
        
        if (g_World && g_Player) {
            auto waypoint = g_World->GetWaypoint(args[0]);
            if (waypoint) {
                g_Player->SetPosition(waypoint->position);
                DebugConsole::GetInstance()->Log("[Game] Teleported to " + args[0]);
            } else {
                DebugConsole::GetInstance()->Log("[Error] Waypoint not found: " + args[0]);
            }
        }
    });
```

## Practical Use Cases

### Debugging Gameplay

```
# Test different camera angles
> setcam 0 100 0
> getcam

# Toggle rendering modes to see geometry
> wireframe
> cellshading off

# Set game variables for testing
> set enemy_health 1
> set player_speed 20
> set time_scale 0.5
```

### Performance Testing

```
# Display system info
> sysinfo

# Set target FPS for testing
> set target_fps 30
> set vsync 0

# Enable profiling
> set show_profiler 1
```

### Level Design

```
# Quick camera positioning
> setcam 250 75 300
> getcam

# Test different visual styles
> cellshading on
> wireframe
```

## Advanced Features

### Variable Persistence

Variables set through the console persist for the entire session:

```cpp
// In your game loop
auto console = DebugConsole::GetInstance();

if (console->HasVariable("player_speed")) {
    float speed = std::stof(console->GetVariable("player_speed", "10.0"));
    player.SetSpeed(speed);
}

if (console->HasVariable("debug_mode")) {
    bool debugMode = console->GetVariable("debug_mode") == "true";
    renderer.SetDebugMode(debugMode);
}
```

### Command Categories

Organize commands by category using naming conventions:

```cpp
// Debug commands
console->RegisterCommand("debug_lines", "Toggle debug line rendering", ...);
console->RegisterCommand("debug_physics", "Show physics debug info", ...);
console->RegisterCommand("debug_ai", "Show AI state info", ...);

// Graphics commands
console->RegisterCommand("gfx_shadows", "Toggle shadow rendering", ...);
console->RegisterCommand("gfx_fov", "Set field of view", ...);
console->RegisterCommand("gfx_quality", "Set graphics quality", ...);

// Game commands
console->RegisterCommand("game_save", "Save current game state", ...);
console->RegisterCommand("game_load", "Load saved game state", ...);
console->RegisterCommand("game_reset", "Reset to initial state", ...);
```

## Best Practices

### 1. Descriptive Command Names
Use clear, lowercase command names:
- Good: `spawn_enemy`, `toggle_debug`, `reload_shaders`
- Avoid: `se`, `td`, `rs`

### 2. Provide Help Text
Always include descriptive help text:
```cpp
console->RegisterCommand("spawn", 
    "Spawn an enemy at camera position. Usage: spawn [type]",
    callback);
```

### 3. Validate Arguments
Check argument counts and types:
```cpp
if (args.size() < 2) {
    console->Log("[Error] Usage: command <arg1> <arg2>");
    return;
}
```

### 4. Error Handling
Use try-catch for type conversions:
```cpp
try {
    float value = std::stof(args[0]);
} catch (const std::exception& e) {
    console->Log("[Error] Invalid number: " + args[0]);
    return;
}
```

### 5. Log Actions
Always log what your command does:
```cpp
console->Log("[Game] Action completed successfully");
console->Log("[Warning] This might cause issues");
console->Log("[Error] Operation failed");
```

## Console API Reference

### DebugConsole Class

#### Singleton Access
```cpp
DebugConsole* console = DebugConsole::GetInstance();
```

#### State Management
```cpp
void SetEnabled(bool enabled);
bool IsEnabled() const;
void Toggle();
```

#### Command Registration
```cpp
void RegisterCommand(const std::string& name, 
                    const std::string& description, 
                    CommandCallback callback);
void UnregisterCommand(const std::string& name);
```

#### Command Execution
```cpp
void ExecuteCommand(const std::string& commandLine);
```

#### Variable System
```cpp
void SetVariable(const std::string& name, const std::string& value);
std::string GetVariable(const std::string& name, const std::string& defaultValue = "");
bool HasVariable(const std::string& name);
const std::map<std::string, std::string>& GetVariables();
```

#### Logging
```cpp
void Log(const std::string& message);
void Clear();
const std::vector<std::string>& GetOutputLog();
```

#### History Navigation
```cpp
std::string GetPreviousCommand();
std::string GetNextCommand();
```

#### Auto-completion
```cpp
std::vector<std::string> GetCommandSuggestions(const std::string& prefix);
```

## Tips and Tricks

### Batch Commands
Execute multiple commands quickly by using command history:
```
> setcam 100 50 100
> wireframe
> cellshading off
[Press Up] [Press Enter]  # Re-execute cellshading off
[Press Up] [Press Up] [Press Enter]  # Re-execute setcam
```

### Quick Testing
Use console variables for rapid iteration:
```
> set speed 20
# Test gameplay
> set speed 30
# Test again
> set speed 15
# Find optimal value
```

### Save Favorite Commands
Keep a text file with commonly used commands:
```
setcam 100 50 100
cellshading on
set time_scale 1.0
wireframe
```

## Troubleshooting

### Console Won't Open
- Ensure you're pressing the correct key (` grave/tilde)
- Check if another system is capturing the key
- Verify console is initialized in main.cpp

### Commands Not Working
- Check command name spelling (case-insensitive)
- Verify command is registered
- Use `help` to see available commands
- Check console log for error messages

### Input Not Appearing
- Ensure console is active (camera should stop moving)
- Check if character callback is registered
- Verify input is not being blocked

## Example Session

```
# Start game and press ` to open console

> help
=== Available Commands ===
  cellshading - Toggle cell shading
  clear - Clear the console output
  echo - Echo text to console (usage: echo <text>)
  exit - Close the console
  get - Get a console variable value (usage: get <name>)
  getcam - Get current camera position
  help - Display all available commands
  listvars - List all console variables
  quit - Close the console
  set - Set a console variable (usage: set <name> <value>)
  setcam - Set camera position (x y z)
  sysinfo - Display system information
  wireframe - Toggle wireframe mode
==========================

> getcam
[Console] Camera position: (50.000000, 30.000000, 50.000000)

> setcam 0 100 0
[Console] Camera moved to (0, 100, 0)

> wireframe
[Console] Wireframe mode: ON

> set test_var hello
[Console] Variable 'test_var' set to 'hello'

> get test_var
test_var = hello

> listvars
=== Console Variables ===
  test_var = hello
=========================

> echo Testing the console
Testing the console

> clear
[Console] Output cleared

> exit
# Console closes, resume gameplay
```

## See Also

- [README.md](README.md) - Main documentation
- [ENGINE_SYSTEMS.md](ENGINE_SYSTEMS.md) - Engine systems reference
- [VERSION_UPDATES.md](VERSION_UPDATES.md) - Architecture updates

---

For more information or to report issues, please visit the GitHub repository.
