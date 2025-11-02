# Game Engine Assets Directory

This directory contains 3D models and textures for the game engine.

## Directory Structure

```
assets/
├── models/          # 3D model files
│   └── nature/      # Nature-themed models (trees, rocks, grass, etc.)
└── textures/        # Texture files
    └── nature/      # Nature-themed textures
```

## Supported Formats

### 3D Models
- `.obj` - Wavefront OBJ
- `.fbx` - Autodesk FBX
- `.gltf` / `.glb` - GL Transmission Format
- `.dae` - Collada
- `.blend` - Blender files

### Textures
- `.png` - PNG images (recommended)
- `.jpg` / `.jpeg` - JPEG images
- `.tga` - Targa images
- `.bmp` - Bitmap images

## Adding Assets

1. Place your 3D models in the appropriate `models/` subdirectory
2. Place corresponding textures in the `textures/` subdirectory
3. Use the Asset Manager in code to load them:

```cpp
#include "AssetManager.h"

AssetManager* assetMgr = AssetManager::GetInstance();
auto model = assetMgr->LoadModel("assets/models/nature/tree.fbx", "oak_tree");
```

## Stylized Nature Kit Integration

To use a Stylized Nature Kit:

1. Export models from Unity/Unreal or use source files
2. Place models in `assets/models/nature/`
3. Place textures in `assets/textures/nature/`
4. See `ASSET_PIPELINE.md` for detailed integration guide

## Example Asset Organization

```
assets/
├── models/
│   ├── nature/
│   │   ├── trees/
│   │   │   ├── oak_01.fbx
│   │   │   ├── pine_01.fbx
│   │   │   └── palm_01.fbx
│   │   ├── rocks/
│   │   │   ├── boulder_01.obj
│   │   │   └── cliff_01.obj
│   │   ├── vegetation/
│   │   │   ├── grass_patch.fbx
│   │   │   ├── bush_01.fbx
│   │   │   └── flower_01.fbx
│   │   └── water/
│   │       └── water_plane.obj
│   └── structures/
│       ├── windmill.fbx
│       └── house_01.fbx
└── textures/
    ├── nature/
    │   ├── bark_diffuse.png
    │   ├── bark_normal.png
    │   ├── leaves_diffuse.png
    │   ├── grass_diffuse.png
    │   ├── rock_diffuse.png
    │   └── water_diffuse.png
    └── structures/
        ├── wood_planks.png
        └── stone_brick.png
```

## Notes

- Keep texture sizes reasonable (1024x1024 or 2048x2048 for quality assets)
- Use power-of-2 texture dimensions for best compatibility
- Place textures in the same directory as models or name them consistently
- The Asset Manager automatically caches loaded assets for efficiency
