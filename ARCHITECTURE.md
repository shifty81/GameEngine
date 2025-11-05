# Architecture Overview

## System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                     Game Engine Editor (.NET 9)                  │
│                                                                   │
│  ┌───────────────────────────────────────────────────────────┐  │
│  │                    WPF Main Window                         │  │
│  │  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐  │  │
│  │  │Hierarchy │  │ Viewport │  │Properties│  │ Console  │  │  │
│  │  │  Panel   │  │  (3D)    │  │  Panel   │  │  Panel   │  │  │
│  │  └──────────┘  └──────────┘  └──────────┘  └──────────┘  │  │
│  │                AvalonDock Dockable UI                      │  │
│  └───────────────────────────────────────────────────────────┘  │
│                                                                   │
│  ┌───────────────────────────────────────────────────────────┐  │
│  │              High-Level Game Systems (C#)                  │  │
│  │  • UI/UX Management        • Quest System                  │  │
│  │  • Inventory & Crafting    • Skill Trees                   │  │
│  │  • Save/Load System        • Mod Loading                   │  │
│  │  • Scripting Engine        • Networking Client             │  │
│  │  • Editor Tools            • Asset Management              │  │
│  └───────────────────────────────────────────────────────────┘  │
│                                                                   │
│  ┌───────────────────────────────────────────────────────────┐  │
│  │            C++/C# Interop Layer (P/Invoke)                 │  │
│  │  EngineInterop.cs ←→ EngineCore.h (DLL Exports)          │  │
│  └───────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
                               ↕ P/Invoke Calls
┌─────────────────────────────────────────────────────────────────┐
│                  Core Engine (C++20 DLL)                         │
│                                                                   │
│  ┌───────────────────────────────────────────────────────────┐  │
│  │              Performance-Critical Systems                  │  │
│  │                                                            │  │
│  │  ┌────────────────┐  ┌────────────────┐  ┌─────────────┐ │  │
│  │  │ Voxel Engine   │  │  Renderer      │  │   Camera    │ │  │
│  │  │                │  │  (DirectX 11)  │  │   System    │ │  │
│  │  │ • Chunks       │  │                │  │             │ │  │
│  │  │ • Mesh Gen     │  │ • Shaders      │  │ • Transform │ │  │
│  │  │ • Culling      │  │ • Textures     │  │ • Controls  │ │  │
│  │  │ • LOD          │  │ • Lighting     │  │ • View/Proj │ │  │
│  │  └────────────────┘  └────────────────┘  └─────────────┘ │  │
│  │                                                            │  │
│  │  ┌────────────────┐  ┌────────────────┐  ┌─────────────┐ │  │
│  │  │ Procedural Gen │  │ Physics Engine │  │  AI System  │ │  │
│  │  │                │  │                │  │             │ │  │
│  │  │ • Noise        │  │ • Collision    │  │ • Pathfind  │ │  │
│  │  │ • L-Systems    │  │ • Dynamics     │  │ • Behaviors │ │  │
│  │  │ • Cellular Auto│  │ • Forces       │  │ • States    │ │  │
│  │  │ • Biomes       │  │ • Constraints  │  │ • Navigation│ │  │
│  │  └────────────────┘  └────────────────┘  └─────────────┘ │  │
│  └───────────────────────────────────────────────────────────┘  │
│                                                                   │
│  ┌───────────────────────────────────────────────────────────┐  │
│  │                  DirectX 11 API                            │  │
│  │  • Device & Context      • Shaders (HLSL)                  │  │
│  │  • Swap Chain            • Vertex/Index Buffers            │  │
│  │  • Render Targets        • Textures & Samplers             │  │
│  └───────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
                               ↕
┌─────────────────────────────────────────────────────────────────┐
│                      Windows OS / GPU                            │
│  • DirectX 11 Runtime     • Windows SDK                          │
│  • GPU Drivers            • Hardware Acceleration                │
└─────────────────────────────────────────────────────────────────┘
```

## Data Flow

### Initialization Flow
```
1. C# Editor Launches (App.xaml.cs)
   ↓
2. MainWindow Created (MainWindow.xaml)
   ↓
3. EnginePanel (WinForms) Embedded in WPF
   ↓
4. C# Calls: EngineInterop.InitializeEngine(hwnd, width, height)
   ↓
5. C++ Creates: Renderer (DirectX 11)
   ↓
6. C++ Creates: VoxelEngine, Camera
   ↓
7. C++ Generates: Initial Terrain
   ↓
8. C# Starts: Update Timer (60 FPS)
```

### Update Loop Flow
```
Every Frame (~16ms):

1. C# Timer Tick
   ↓
2. C# Calculates Delta Time
   ↓
3. C# Processes Input (WASD, Mouse)
   ↓
4. C# Calls: EngineInterop.UpdateEngine(deltaTime)
   ↓
5. C++ Updates: Camera, VoxelEngine, Physics
   ↓
6. C# Calls: EngineInterop.RenderEngine()
   ↓
7. C++ Renders: All visible chunks
   ↓
8. DirectX 11 Presents: Frame to screen
   ↓
9. C# Updates: UI (FPS, Camera position)
```

### User Interaction Flow
```
User Input:
  Keyboard → C# KeyDown Event → EngineInterop → C++ Camera Movement
  Mouse → C# MouseMove Event → EngineInterop → C++ Camera Rotation
  Console → C# Command Parser → EngineInterop → C++ Actions
  UI Button → C# Event Handler → EngineInterop → C++ Operations
```

## Component Responsibilities

### C# Layer (.NET 9)

**Purpose**: High-level logic, UI, tools, and rapid iteration

**Responsibilities**:
- User interface (WPF with AvalonDock)
- Editor tools and inspectors
- Game logic (inventory, crafting, quests)
- Save/load system
- Mod loading and scripting
- Networking (client-side)
- Asset management (file I/O)
- Scene management
- Event handling

**Why C#**:
- Fast development iteration
- Rich UI libraries (WPF)
- Easy reflection and serialization
- Strong typing with modern features
- Garbage collection for UI objects
- Easy debugging

### C++ Layer (C++20)

**Purpose**: Performance-critical core systems

**Responsibilities**:
- Voxel chunk management
- Mesh generation and optimization
- DirectX 11 rendering
- Physics simulation
- Camera mathematics
- Procedural generation algorithms
- AI pathfinding
- Collision detection
- Memory-intensive operations

**Why C++**:
- Direct hardware access
- Manual memory management
- SIMD optimizations
- Zero-cost abstractions
- Direct GPU API access
- Multi-threading control
- Minimal runtime overhead

## Memory Management

### C++ Memory
- Manual allocation/deallocation
- Object pools for chunks
- Stack-based allocations where possible
- Custom allocators for performance

### C# Memory
- Garbage collected
- Structs for small data (Vector3, Color)
- Reference types for UI elements
- Weak references for large objects

### Shared Memory
- C++ owns all voxel data
- C# passes pointers via IntPtr
- No garbage collection of C++ objects
- Explicit lifetime management

## Threading Model

### C# Threads
- **UI Thread**: WPF main thread
- **Update Thread**: Timer-based game loop
- **Asset Thread**: Background asset loading

### C++ Threads
- **Main Thread**: Rendering and core update
- **Chunk Threads**: Mesh generation (pool)
- **Physics Thread**: Simulation step
- **Gen Thread**: Procedural generation

### Synchronization
- C++ uses mutex/locks for chunk access
- C# marshals calls to C++ on update thread
- No direct C# ↔ C++ thread communication

## File Structure

```
GameEngine/
├── GameEngine.sln                   # Visual Studio Solution
│
├── src/
│   ├── GameEngine.Core/             # C++ DLL Project
│   │   ├── EngineCore.h/.cpp        # DLL API Exports
│   │   ├── VoxelEngine.h/.cpp       # Voxel World Manager
│   │   ├── VoxelChunk.h/.cpp        # 16³ Chunk + Mesh
│   │   ├── Renderer.h/.cpp          # DirectX 11 Wrapper
│   │   ├── Camera.h/.cpp            # Camera System
│   │   └── GameEngine.Core.vcxproj  # VS C++ Project
│   │
│   └── GameEngine.Editor/           # C# WPF Project
│       ├── MainWindow.xaml/.cs      # Main Editor Window
│       ├── EngineInterop.cs         # P/Invoke Definitions
│       ├── EnginePanel.cs           # WinForms Render Panel
│       ├── App.xaml/.cs             # Application Entry
│       └── GameEngine.Editor.csproj # .NET 9 Project
│
├── bin/                             # Build Output
│   ├── Debug/x64/                   # Debug builds
│   │   ├── GameEngine.Core.dll      # C++ DLL
│   │   └── GameEngine.Core.pdb      # Debug symbols
│   └── Release/x64/                 # Release builds
│
├── obj/                             # Intermediate Files
├── external/                        # Third-party (future)
├── assets/                          # Game Assets (future)
│
├── README_WINDOWS.md                # Main Documentation
├── QUICKSTART.md                    # Quick Start Guide
├── ROADMAP.md                       # Feature Roadmap
├── ARCHITECTURE.md                  # This File
│
├── build.ps1                        # Build Script
└── run.ps1                          # Run Script
```

## Design Patterns Used

### C++ Patterns
- **Singleton**: VoxelEngine, Renderer (via static)
- **Object Pool**: Chunk allocation
- **Factory**: Block type creation
- **Component**: Camera, Transform
- **Observer**: Event system (future)

### C# Patterns
- **MVVM**: WPF ViewModels for UI
- **Command**: UI commands
- **Facade**: EngineInterop simplifies C++ access
- **Strategy**: Different editor tools
- **Repository**: Asset management (future)

## Communication Protocol

### C# → C++
```csharp
// C# Code
[DllImport("GameEngine.Core.dll")]
public static extern void SetVoxel(int x, int y, int z, byte type);

// Usage
EngineInterop.SetVoxel(10, 5, 10, 1);
```

### C++ API
```cpp
// C++ Code
extern "C" {
    ENGINECORE_API void SetVoxel(int x, int y, int z, uint8_t type) {
        if (g_voxelEngine) {
            g_voxelEngine->SetVoxel(x, y, z, type);
        }
    }
}
```

### Data Marshaling
- **Primitives**: Direct pass (int, float, bool)
- **Strings**: Marshal as const char*
- **Arrays**: Pass pointer + length
- **Structs**: Align with [StructLayout]
- **Pointers**: Use IntPtr in C#

## Performance Considerations

### Hot Paths (C++)
- Voxel mesh generation
- Rendering loop
- Physics update
- Collision detection
- Chunk updates

### Cold Paths (C#)
- UI updates
- File I/O
- Serialization
- Mod loading
- Networking (non-critical)

### Optimization Strategies
1. **Batch C++ calls**: Group operations
2. **Cache frequently used data**: Avoid repeated P/Invoke
3. **Use structs**: Reduce GC pressure in C#
4. **Pool allocations**: Reuse objects
5. **Multi-thread**: Separate threads for independent work

## Extension Points

### Adding C++ Features
1. Add function to EngineCore.h
2. Implement in EngineCore.cpp
3. Export with `extern "C"`
4. Add P/Invoke in EngineInterop.cs
5. Call from C# code

### Adding C# Features
1. Create new XAML window/control
2. Add event handlers
3. Call EngineInterop as needed
4. Update UI based on responses

### Adding Dockable Panels
1. Create new UserControl (XAML)
2. Add to MainWindow DockingManager
3. Register in AvalonDock layout
4. Wire up data binding

## Security Considerations

### C++ DLL
- Exported functions should validate all inputs
- Bounds checking on all array access
- Null pointer checks
- Safe string handling

### C# Interop
- Validate all data before passing to C++
- Handle exceptions from P/Invoke
- Use safe handles for resources
- Dispose pattern for cleanup

## Future Architecture Evolution

### Phase 1: Current (Complete)
- Basic C++/C# hybrid
- DirectX 11 rendering
- Simple voxel system
- Editor UI

### Phase 2: Enhanced (Next)
- Multi-threaded chunk generation
- Advanced rendering (LOD, culling)
- Save/load system
- More complex editor tools

### Phase 3: Advanced (Future)
- C++ plugin system
- Lua scripting integration
- Networked multiplayer
- VR support

### Phase 4: Ecosystem (Long-term)
- Mod marketplace
- Asset store integration
- Cloud saves
- Cross-platform (Linux/Mac)

---

This architecture provides:
- **Performance**: C++ for critical systems
- **Productivity**: C# for everything else
- **Flexibility**: Easy to extend both layers
- **Maintainability**: Clear separation of concerns
- **Scalability**: Can grow to AAA-level features
