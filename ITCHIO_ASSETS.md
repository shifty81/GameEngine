# Integrating itch.io Assets

This guide shows you how to easily drop in assets and content downloaded from itch.io into your game engine.

## Overview

itch.io is a popular marketplace for game assets, including:
- 3D models and characters
- Textures and materials
- Sound effects and music
- Sprite packs and UI elements
- Complete asset packs (like nature kits, dungeon packs, etc.)

This engine makes it easy to integrate these assets directly into your projects.

## Quick Start - 3 Simple Steps

### 1. Download Assets from itch.io

1. Browse [itch.io](https://itch.io/game-assets) for assets
2. Download your chosen asset pack
3. Extract the downloaded files to a temporary location

### 2. Organize Assets in Your Project

Copy assets to the appropriate directories:

```
GameEngine/
├── assets/
│   ├── models/          # Place .obj, .fbx, .gltf files here
│   │   ├── characters/
│   │   ├── environment/
│   │   └── props/
│   └── textures/        # Place .png, .jpg texture files here
│       ├── diffuse/
│       ├── normal/
│       └── materials/
```

**Example:**
```bash
# After downloading a "Fantasy Forest Pack" from itch.io
GameEngine/
├── assets/
│   ├── models/
│   │   └── environment/
│   │       ├── tree_pine.fbx
│   │       ├── tree_oak.fbx
│   │       ├── rock_01.obj
│   │       └── bush_01.fbx
│   └── textures/
│       └── forest/
│           ├── tree_bark.png
│           ├── leaves_green.png
│           └── rock_moss.png
```

### 3. Load Assets in Your Game

Use the engine's AssetManager to load the assets:

```cpp
#include "AssetManager.h"

// Get asset manager instance
AssetManager* assets = AssetManager::GetInstance();

// Load models from itch.io asset pack
auto pine_tree = assets->LoadModel("assets/models/environment/tree_pine.fbx", "pine_tree");
auto oak_tree = assets->LoadModel("assets/models/environment/tree_oak.fbx", "oak_tree");
auto rock = assets->LoadModel("assets/models/environment/rock_01.obj", "rock");

// Load textures
auto bark_texture = assets->LoadTexture("assets/textures/forest/tree_bark.png", "bark");
auto leaves_texture = assets->LoadTexture("assets/textures/forest/leaves_green.png", "leaves");

// Use in your game loop
shader.use();
shader.setMat4("model", modelMatrix);
pine_tree->Draw(shader);
```

## Common itch.io Asset Types

### 3D Models

**Supported Formats:**
- `.fbx` (Autodesk FBX) - Most common on itch.io
- `.obj` (Wavefront OBJ) - Simple, widely supported
- `.gltf` / `.glb` (GL Transmission Format) - Modern standard
- `.dae` (Collada)
- `.blend` (Blender files)

**Where to place:** `assets/models/`

**How to load:**
```cpp
auto model = AssetManager::GetInstance()->LoadModel(
    "assets/models/your_model.fbx", 
    "unique_name"
);
```

### Textures & Materials

**Supported Formats:**
- `.png` (with transparency)
- `.jpg` / `.jpeg`
- `.tga` (Targa)
- `.bmp`

**Common texture types from itch.io:**
- Albedo/Diffuse maps (color)
- Normal maps (surface detail)
- Roughness maps (material shine)
- Metallic maps (metallic surfaces)
- Height/Displacement maps

**Where to place:** `assets/textures/`

**How to load:**
```cpp
auto texture = AssetManager::GetInstance()->LoadTexture(
    "assets/textures/my_texture.png", 
    "texture_name"
);
```

### Audio Files

**Supported Formats:**
- `.wav`
- `.ogg`
- `.mp3`

**Where to place:** `assets/audio/`

**How to use:**
```cpp
// Audio system integration example
// (Requires audio system implementation)
```

## Example: Integrating a Complete Asset Pack

Let's say you downloaded a "Low Poly Nature Pack" from itch.io.

### Step 1: Extract the Pack

After downloading, you might have:
```
LowPolyNaturePack/
├── Models/
│   ├── Trees/
│   │   ├── Pine_Tree_01.fbx
│   │   └── Oak_Tree_01.fbx
│   └── Rocks/
│       └── Rock_Moss_01.fbx
└── Textures/
    ├── Tree_Diffuse.png
    └── Rock_Diffuse.png
```

### Step 2: Copy to Engine

```bash
# Copy models
cp -r LowPolyNaturePack/Models/* GameEngine/assets/models/nature/

# Copy textures
cp -r LowPolyNaturePack/Textures/* GameEngine/assets/textures/nature/
```

### Step 3: Create a Scene with Assets

```cpp
#include "AssetManager.h"
#include <vector>

class NatureScene {
private:
    AssetManager* assets;
    std::vector<std::shared_ptr<Model>> trees;
    std::vector<std::shared_ptr<Model>> rocks;
    
public:
    void LoadAssets() {
        assets = AssetManager::GetInstance();
        
        // Load all nature models
        auto pine = assets->LoadModel("assets/models/nature/Pine_Tree_01.fbx", "pine");
        auto oak = assets->LoadModel("assets/models/nature/Oak_Tree_01.fbx", "oak");
        auto rock = assets->LoadModel("assets/models/nature/Rock_Moss_01.fbx", "rock");
        
        trees.push_back(pine);
        trees.push_back(oak);
        rocks.push_back(rock);
    }
    
    void PlaceInWorld() {
        // Place trees randomly in the world
        for (int i = 0; i < 20; i++) {
            float x = (rand() % 100) - 50.0f;
            float z = (rand() % 100) - 50.0f;
            
            // Alternate between tree types
            auto tree = trees[i % trees.size()];
            
            // Create transform and render
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0, z));
            shader.setMat4("model", model);
            tree->Draw(shader);
        }
    }
};

// Usage in main.cpp
NatureScene scene;
scene.LoadAssets();

// In render loop:
scene.PlaceInWorld();
```

## Asset Coordinate Systems

Some itch.io assets may use different coordinate systems than this engine:

**Engine Uses:** Y-up (OpenGL standard)
- X: Right
- Y: Up  
- Z: Forward (toward camera)

**If assets appear rotated or upside-down:**

```cpp
// Rotate model to match engine coordinate system
glm::mat4 model = glm::mat4(1.0f);
model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0)); // Rotate around X
model = glm::translate(model, position);

shader.setMat4("model", model);
modelFromItchio->Draw(shader);
```

## Asset Scale Issues

Some itch.io assets may be scaled differently:

```cpp
// Scale asset to fit your world
glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, position);
model = glm::scale(model, glm::vec3(0.1f)); // Scale down by 10x

shader.setMat4("model", model);
oversizedModel->Draw(shader);
```

## Organizing Large Asset Collections

For large projects with many itch.io packs:

```
assets/
├── models/
│   ├── nature_pack_01/      # First itch.io pack
│   │   ├── trees/
│   │   ├── rocks/
│   │   └── plants/
│   ├── dungeon_pack_01/     # Second itch.io pack
│   │   ├── walls/
│   │   ├── props/
│   │   └── enemies/
│   └── character_pack_01/   # Third itch.io pack
│       ├── heroes/
│       └── villains/
└── textures/
    ├── nature_pack_01/
    ├── dungeon_pack_01/
    └── character_pack_01/
```

**Create a helper class:**

```cpp
class AssetPackLoader {
public:
    static void LoadNaturePack(AssetManager* assets) {
        assets->LoadModel("assets/models/nature_pack_01/trees/pine.fbx", "pine");
        assets->LoadModel("assets/models/nature_pack_01/trees/oak.fbx", "oak");
        assets->LoadTexture("assets/textures/nature_pack_01/bark.png", "bark");
        // ... load all assets from pack
    }
    
    static void LoadDungeonPack(AssetManager* assets) {
        assets->LoadModel("assets/models/dungeon_pack_01/walls/wall_01.fbx", "wall");
        // ... load all assets from pack
    }
};

// Usage
AssetManager* assets = AssetManager::GetInstance();
AssetPackLoader::LoadNaturePack(assets);
AssetPackLoader::LoadDungeonPack(assets);
```

## Performance Tips

### 1. Asset Caching

The AssetManager automatically caches loaded assets:

```cpp
// First load - reads from disk
auto tree = assets->LoadModel("assets/models/tree.fbx", "tree");

// Second load with same name - returns cached version (instant)
auto tree2 = assets->LoadModel("assets/models/tree.fbx", "tree");

// tree and tree2 point to the same cached asset
```

### 2. Batch Loading

Load all assets during a loading screen:

```cpp
void LoadingScreen::LoadAllAssets() {
    AssetManager* assets = AssetManager::GetInstance();
    
    // Show loading bar
    UpdateProgress(0.0f, "Loading nature assets...");
    LoadNaturePack();
    
    UpdateProgress(0.3f, "Loading dungeon assets...");
    LoadDungeonPack();
    
    UpdateProgress(0.6f, "Loading character assets...");
    LoadCharacterPack();
    
    UpdateProgress(1.0f, "Ready!");
}
```

### 3. Unload Unused Assets

Free memory when switching levels:

```cpp
// Unload specific asset
assets->UnloadModel("old_level_prop");

// Clear all assets (do this between levels)
assets->ClearAll();
```

## Popular itch.io Asset Types for This Engine

### Recommended Asset Packs

1. **Low Poly Asset Packs**
   - Perform well in real-time
   - Look great with the engine's cell shading
   - Easy to integrate

2. **Nature Kits**
   - Trees, rocks, plants
   - Work well with engine's terrain system
   - Combine with procedural generation

3. **Modular Building Sets**
   - Dungeons, castles, sci-fi bases
   - Snap together naturally
   - Reuse with different textures

4. **Character Models**
   - Humanoid rigs
   - Animated (when animation system added)
   - With texture variations

### Search Tips on itch.io

When browsing itch.io for compatible assets:
- Filter by "3D models" or "Game assets"
- Look for "FBX", "OBJ", or "GLTF" formats
- Check license (free, commercial use, etc.)
- Read comments for quality feedback
- Download preview/demo versions first

## Troubleshooting

### Asset Not Loading

**Problem:** `LoadModel()` returns nullptr

**Solutions:**
1. Check file path is correct (relative to project root)
2. Verify file format is supported
3. Check console for error messages
4. Try absolute path first for testing

```cpp
auto model = assets->LoadModel("assets/models/tree.fbx", "tree");
if (!model) {
    std::cerr << "Failed to load tree model!" << std::endl;
    std::cerr << "Check: assets/models/tree.fbx exists" << std::endl;
}
```

### Textures Look Wrong

**Problem:** Textures appear black, white, or missing

**Solutions:**
1. Ensure texture files are in `assets/textures/`
2. Check texture paths in the model file
3. May need to rebake textures in Blender
4. Use absolute paths for testing

### Model Appears Tiny or Huge

**Problem:** Asset is wrong size

**Solution:** Scale the model

```cpp
glm::mat4 model = glm::mat4(1.0f);
model = glm::scale(model, glm::vec3(10.0f)); // Scale up 10x
// or
model = glm::scale(model, glm::vec3(0.01f)); // Scale down 100x
```

### Model is Rotated Wrong

**Problem:** Asset is upside-down or sideways

**Solution:** Rotate during rendering

```cpp
// Common fixes for different coordinate systems:

// Blender export (Z-up to Y-up)
model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));

// Some Unity exports
model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
```

## Best Practices

1. **Always organize by pack/source**
   - Keep each itch.io pack in its own folder
   - Easier to credit creators
   - Simpler to update or remove

2. **Name assets descriptively**
   ```cpp
   // Good
   assets->LoadModel("assets/models/nature/oak_tree_large.fbx", "oak_large");
   
   // Bad
   assets->LoadModel("assets/models/tree1.fbx", "t1");
   ```

3. **Check licenses**
   - Many itch.io assets are free for personal use
   - Some require attribution
   - Some allow commercial use
   - Always read the license.txt included with assets

4. **Test assets individually first**
   ```cpp
   // Load one asset at a time to test
   auto test = assets->LoadModel("new_asset.fbx", "test");
   if (test) {
       std::cout << "Asset loads successfully!" << std::endl;
   }
   ```

5. **Keep a credits file**
   ```
   CREDITS.txt:
   
   Nature Pack - by ArtistName (itch.io/artist)
   - License: CC-BY 4.0
   - Files: assets/models/nature/*
   
   Dungeon Pack - by AnotherArtist (itch.io/artist2)  
   - License: Free for commercial use
   - Files: assets/models/dungeon/*
   ```

## Example Projects

### Forest Scene
```cpp
// forest_scene.cpp
void CreateForestScene() {
    AssetManager* assets = AssetManager::GetInstance();
    
    // Load itch.io nature pack
    auto pine = assets->LoadModel("assets/models/nature/pine.fbx", "pine");
    auto oak = assets->LoadModel("assets/models/nature/oak.fbx", "oak");
    auto grass = assets->LoadTexture("assets/textures/nature/grass.png", "grass");
    
    // Create forest
    for (int x = -50; x < 50; x += 5) {
        for (int z = -50; z < 50; z += 5) {
            glm::vec3 pos(x + (rand() % 3), 0, z + (rand() % 3));
            glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
            
            shader.setMat4("model", model);
            (rand() % 2 ? pine : oak)->Draw(shader);
        }
    }
}
```

## Resources

- **itch.io Game Assets**: https://itch.io/game-assets
- **Free Asset Collections**: https://itch.io/game-assets/free
- **3D Models**: https://itch.io/game-assets/tag-3d
- **This Engine's Asset Pipeline Guide**: See `ASSET_PIPELINE.md`

## Summary

Integrating itch.io assets is simple:
1. Download and extract
2. Copy to `assets/` folders
3. Load with AssetManager
4. Adjust scale/rotation if needed
5. Enjoy!

The engine's AssetManager handles all the complexity - you just provide the file paths.
