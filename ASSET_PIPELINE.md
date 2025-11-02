# Asset Pipeline Guide - Stylized Nature Kit Integration

This guide explains how to integrate a Stylized Nature Kit (or any 3D asset pack) into the game engine.

## Overview

The engine now includes a complete asset pipeline with:
- **Assimp**: Loads .obj, .fbx, .gltf, and other 3D model formats
- **stb_image**: Loads .png, .tga, .jpg, and other texture formats  
- **Asset Manager**: Centralized asset loading and caching system
- **Model/Mesh System**: Efficient rendering with proper material support

## Step 1: Prepare Your Assets

### Export from Unity/Unreal

If your Stylized Nature Kit is in a Unity or Unreal project:

1. **From Unity:**
   - Select asset in Project window
   - Right-click → Export Package (uncheck dependencies)
   - Or find the source files in `Assets/YourKit/Models/` and `Assets/YourKit/Textures/`

2. **From Unreal:**
   - Content Browser → Right-click asset → Asset Actions → Export
   - Choose FBX format for models

### Supported Formats

**3D Models:**
- `.obj` - Wavefront OBJ (recommended for static objects)
- `.fbx` - Autodesk FBX (recommended for animated objects)
- `.gltf` / `.glb` - GL Transmission Format (modern standard)
- `.dae` - Collada
- `.blend` - Blender files

**Textures:**
- `.png` - PNG images (recommended, supports transparency)
- `.jpg` / `.jpeg` - JPEG images
- `.tga` - Targa images
- `.bmp` - Bitmap images
- `.psd` - Photoshop files

## Step 2: Organize Your Assets

Create an `assets/` directory structure:

```
GameEngine/
├── assets/
│   ├── models/
│   │   ├── nature/
│   │   │   ├── tree_01.fbx
│   │   │   ├── tree_02.fbx
│   │   │   ├── rock_01.obj
│   │   │   ├── grass_patch.fbx
│   │   │   └── bush_01.fbx
│   │   └── structures/
│   │       └── windmill.fbx
│   └── textures/
│       ├── nature/
│       │   ├── tree_bark_diffuse.png
│       │   ├── tree_bark_normal.png
│       │   ├── leaves_diffuse.png
│       │   ├── grass_diffuse.png
│       │   └── rock_diffuse.png
│       └── wind/
│           └── windmill_diffuse.png
```

## Step 3: Loading Assets in C++

### Basic Model Loading

```cpp
#include "AssetManager.h"

// Get the asset manager instance
AssetManager* assetMgr = AssetManager::GetInstance();

// Load a tree model
auto treeModel = assetMgr->LoadModel("assets/models/nature/tree_01.fbx", "tree_oak");

// Load a rock model  
auto rockModel = assetMgr->LoadModel("assets/models/nature/rock_01.obj", "rock_granite");

// Load windmill for wind system
auto windmillModel = assetMgr->LoadModel("assets/models/structures/windmill.fbx", "windmill");
```

### Loading with Textures

```cpp
// The model loader automatically finds textures in the same directory
// Or you can load textures separately:

unsigned int barkTexture = assetMgr->LoadTexture(
    "tree_bark_diffuse.png", 
    "assets/textures/nature",
    "bark_texture"
);

unsigned int grassTexture = assetMgr->LoadTexture(
    "grass_diffuse.png",
    "assets/textures/nature", 
    "grass_texture"
);
```

### Rendering Models

```cpp
// In your render loop:
shader.use();
shader.setMat4("projection", projection);
shader.setMat4("view", view);

// Set model transform
glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, glm::vec3(10.0f, 0.0f, 10.0f));
model = glm::scale(model, glm::vec3(2.0f));
shader.setMat4("model", model);

// Draw the model
treeModel->Draw(shader);
```

## Step 4: Vertex Animation for Wind

### Tree/Grass Swaying Shader

Update your vertex shader to add wind animation:

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Wind uniforms
uniform float time;
uniform vec3 windDirection;
uniform float windStrength;

void main() {
    vec3 pos = aPos;
    
    // Apply wind effect based on vertex height (Y coordinate)
    // Only affect upper parts of trees/grass
    float windEffect = aPos.y * 0.1;  // Higher vertices move more
    
    // Create swaying motion
    float sway = sin(time * 2.0 + aPos.x * 0.5 + aPos.z * 0.5) * windStrength;
    pos.x += windDirection.x * sway * windEffect;
    pos.z += windDirection.z * sway * windEffect;
    
    FragPos = vec3(model * vec4(pos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
```

### Using Wind System

```cpp
#include "Wind.h"

// Create wind system
Wind wind(glm::vec3(1.0f, 0.0f, 0.5f), 2.0f);

// In render loop:
wind.Update(deltaTime);

// Pass wind data to shader
shader.setFloat("time", glfwGetTime());
shader.setVec3("windDirection", wind.getDirection());
shader.setFloat("windStrength", wind.getStrength());
```

## Step 5: Stylized Rendering

The engine includes cell shading for stylized looks. For nature assets:

### Cell-Shaded Nature Shader

```glsl
// Fragment shader for stylized nature
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool cellShading;

void main() {
    vec3 color = texture(texture_diffuse1, TexCoords).rgb;
    
    // Lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    
    // Cell shading quantization
    if (cellShading) {
        if (diff > 0.95) diff = 1.0;
        else if (diff > 0.7) diff = 0.8;
        else if (diff > 0.4) diff = 0.5;
        else diff = 0.3;
    }
    
    // Rim lighting for stylized effect
    vec3 viewDir = normalize(viewPos - FragPos);
    float rim = 1.0 - max(dot(viewDir, norm), 0.0);
    rim = smoothstep(0.6, 1.0, rim);
    vec3 rimColor = vec3(0.3) * rim;
    
    vec3 result = (0.3 + diff * 0.7) * color + rimColor;
    FragColor = vec4(result, 1.0);
}
```

## Step 6: Scene Integration

### Complete Example

```cpp
// In main.cpp or scene setup:

AssetManager* assetMgr = AssetManager::GetInstance();

// Load nature assets
auto tree = assetMgr->LoadModel("assets/models/nature/tree_01.fbx", "tree");
auto rock = assetMgr->LoadModel("assets/models/nature/rock_01.obj", "rock");
auto grass = assetMgr->LoadModel("assets/models/nature/grass_patch.fbx", "grass");

// Create instances with transforms
struct GameObject {
    std::shared_ptr<Model> model;
    glm::vec3 position;
    glm::vec3 scale;
    float rotation;
};

std::vector<GameObject> trees;
std::vector<GameObject> rocks;

// Populate scene
for (int i = 0; i < 50; i++) {
    GameObject obj;
    obj.model = tree;
    obj.position = glm::vec3(rand() % 100, 0, rand() % 100);
    obj.scale = glm::vec3(1.0f + (rand() % 50) / 100.0f);
    obj.rotation = (rand() % 360) * 3.14159f / 180.0f;
    trees.push_back(obj);
}

// In render loop:
for (auto& tree : trees) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, tree.position);
    model = glm::rotate(model, tree.rotation, glm::vec3(0, 1, 0));
    model = glm::scale(model, tree.scale);
    
    shader.setMat4("model", model);
    tree.model->Draw(shader);
}
```

## Step 7: Coordinate System Handling

Different tools use different coordinate systems:

```cpp
// If assets appear rotated or flipped:

// For assets from Blender (Z-up):
model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));

// For assets from 3DS Max (Z-up, different):
model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));

// Or configure import:
const aiScene* scene = importer.ReadFile(path, 
    aiProcess_Triangulate | 
    aiProcess_FlipUVs |
    aiProcess_ConvertToLeftHanded);  // Convert coordinate system
```

## Asset Manager Features

### Caching
Assets are automatically cached. Loading the same asset twice returns the cached version.

### Memory Management
```cpp
// Unload when not needed
assetMgr->UnloadModel("tree_oak");
assetMgr->UnloadTexture("bark_texture");

// Clear all assets
assetMgr->ClearAll();

// Check what's loaded
assetMgr->PrintStats();
```

### Reusable Assets
```cpp
// Same model, different locations
auto tree1 = assetMgr->GetModel("tree_oak");  // Returns cached model
auto tree2 = assetMgr->GetModel("tree_oak");  // Same instance
// Both point to the same model data - efficient!
```

## Performance Tips

1. **Use LOD (Level of Detail)**: Load multiple versions of models for different distances
2. **Instance Rendering**: Render many copies of the same model efficiently
3. **Texture Atlasing**: Combine multiple textures into one
4. **Occlusion Culling**: Don't render what's not visible
5. **Batch Similar Objects**: Group objects using the same shader/material

## Troubleshooting

### Models not appearing
- Check file paths are correct
- Verify models are in supported format
- Check console for Assimp error messages
- Ensure textures are in same directory as model or properly referenced

### Textures look wrong
- Verify UV coordinates in model
- Check texture format is supported
- Try flipping UVs: `aiProcess_FlipUVs`
- Ensure textures are power-of-2 dimensions for best compatibility

### Performance issues
- Reduce polygon count of models
- Use texture compression
- Implement frustum culling
- Use simpler shaders for distant objects

## Next Steps

- Add animation support for character models
- Implement particle systems for effects
- Add physics integration
- Create a scene editor
- Build prefab system for asset reuse
