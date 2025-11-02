# Engine Systems Documentation

This document provides a comprehensive overview of all game engine systems implemented in the GameEngine project.

## Table of Contents

1. [Graphics and Rendering](#graphics-and-rendering)
2. [Input and Event Handling](#input-and-event-handling)
3. [Core Engine Systems](#core-engine-systems)
4. [Optional Features](#optional-features)
5. [Integration Examples](#integration-examples)

---

## Graphics and Rendering

### Window and Context Management (GLFW)
**Status**: ✅ Fully Implemented

The engine uses GLFW for cross-platform window creation and OpenGL context management.

**Features**:
- Window creation with customizable size
- OpenGL 3.3+ core profile context
- Framebuffer resize callbacks
- Input event handling integration
- V-Sync control

**Files**: `main.cpp`

**Usage**:
```cpp
glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

GLFWwindow* window = glfwCreateWindow(1280, 720, "Game Engine", NULL, NULL);
glfwMakeContextCurrent(window);
```

### Shader Management
**Status**: ✅ Fully Implemented

Complete shader system for loading, compiling, and managing GLSL shaders.

**Features**:
- Vertex and fragment shader compilation
- Error checking and reporting
- Uniform variable setting (int, float, vec2, vec3, vec4, mat4, bool)
- Support for inline GLSL code or file-based shaders
- Shader program linking

**Files**: `Shader.h`

**Usage**:
```cpp
Shader shader("vertex.glsl", "fragment.glsl", true);  // Load from files
// or
Shader shader(vertexSource, fragmentSource, false);    // Inline source

shader.use();
shader.setMat4("projection", projectionMatrix);
shader.setVec3("lightPos", glm::vec3(1.0f, 1.0f, 1.0f));
shader.setBool("useTexture", true);
```

### Mesh and Model Loading (Assimp)
**Status**: ✅ Fully Implemented

3D model loading system using Assimp library for multiple formats.

**Supported Formats**:
- OBJ (Wavefront)
- FBX (Autodesk)
- GLTF/GLB (GL Transmission Format)
- DAE (Collada)
- BLEND (Blender)

**Features**:
- Automatic mesh processing and triangulation
- Normal generation
- Tangent/bitangent calculation
- Multi-mesh model support
- Material and texture extraction

**Files**: `Model.h`, `AssetManager.h`

**Usage**:
```cpp
AssetManager* assetMgr = AssetManager::GetInstance();
auto model = assetMgr->LoadModel("assets/models/tree.fbx", "tree");

// In render loop
shader.use();
shader.setMat4("model", transform);
model->Draw(shader);
```

### Texture Management (stb_image)
**Status**: ✅ Fully Implemented

Texture loading system supporting multiple image formats.

**Supported Formats**:
- PNG (with alpha)
- JPEG/JPG
- TGA (Targa)
- BMP (Bitmap)
- PSD (Photoshop)

**Features**:
- Automatic format detection
- Mipmap generation
- Texture parameter configuration
- Procedural texture generation

**Files**: `TextureLoader.h`, `TextureGenerator.h`, `AssetManager.h`

**Usage**:
```cpp
AssetManager* assetMgr = AssetManager::GetInstance();
unsigned int texture = assetMgr->LoadTexture("diffuse.png", "assets/textures", "tree_diffuse");

// Bind texture
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, texture);
shader.setInt("texture_diffuse1", 0);
```

### Camera System
**Status**: ✅ Fully Implemented

First-person camera system with full 3D movement.

**Features**:
- WASD movement
- Mouse look (pitch/yaw)
- Vertical movement (Space/Shift)
- Adjustable movement speed and mouse sensitivity
- FOV (zoom) control
- View matrix generation

**Files**: `Camera.h`

**Usage**:
```cpp
Camera camera(glm::vec3(0.0f, 5.0f, 10.0f));

// Process input
camera.ProcessKeyboard(FORWARD, deltaTime);
camera.ProcessMouseMovement(xoffset, yoffset);
camera.ProcessMouseScroll(yoffset);

// Get view matrix
glm::mat4 view = camera.GetViewMatrix();
shader.setMat4("view", view);
```

### Lighting System
**Status**: ✅ Newly Implemented

Advanced lighting system with multiple light types.

**Light Types**:
- **Directional Light**: Sun-like infinite light (e.g., sunlight)
- **Point Light**: Omnidirectional light with attenuation (e.g., bulb)
- **Spot Light**: Cone-shaped light (e.g., flashlight)

**Features**:
- Multiple lights support (arrays in shaders)
- Attenuation for point lights (constant, linear, quadratic)
- Spot light cutoff angles (inner and outer)
- Per-light color and intensity
- Enable/disable individual lights
- Global ambient lighting

**Files**: `LightSystem.h`

**Usage**:
```cpp
LightManager* lightMgr = LightManager::GetInstance();

// Add directional light (sun)
auto sun = std::make_shared<DirectionalLight>(
    glm::vec3(0.0f, -1.0f, -0.5f),  // Direction
    glm::vec3(1.0f, 1.0f, 0.9f),    // Color
    1.0f                             // Intensity
);
lightMgr->AddDirectionalLight(sun);

// Add point light
auto light = std::make_shared<PointLight>(
    glm::vec3(5.0f, 3.0f, 5.0f),    // Position
    glm::vec3(1.0f, 0.8f, 0.6f),    // Color
    5.0f                             // Intensity
);
lightMgr->AddPointLight(light);

// Apply to shader
lightMgr->ApplyToShader(shader);
```

### Material System
**Status**: ✅ Newly Implemented

PBR (Physically Based Rendering) material system.

**Properties**:
- **Albedo**: Base color
- **Metallic**: Metallic factor (0-1)
- **Roughness**: Surface roughness (0-1)
- **AO**: Ambient occlusion
- **Emission**: Self-illumination color and strength

**Texture Maps**:
- Albedo/Diffuse map
- Normal map (bump mapping)
- Metallic map
- Roughness map
- Ambient occlusion map
- Emission map

**Files**: `MaterialSystem.h`

**Usage**:
```cpp
MaterialManager* matMgr = MaterialManager::GetInstance();

// Create material
auto metal = matMgr->CreateMaterial("Metal");
metal->SetAlbedo(glm::vec3(0.7f, 0.7f, 0.7f));
metal->SetMetallic(1.0f);
metal->SetRoughness(0.3f);

// Set textures
metal->SetAlbedoMap(textureID);
metal->SetNormalMap(normalMapID);

// Apply to shader
metal->ApplyToShader(shader);
```

### Post-Processing Effects
**Status**: ✅ Newly Implemented

Framework for screen-space post-processing effects.

**Implemented Effects**:
- **FXAA**: Fast Approximate Anti-Aliasing
- **Tone Mapping**: HDR to LDR conversion (Reinhard, Exposure, ACES)
- **Bloom**: Glow effect (framework ready)

**Features**:
- Framebuffer abstraction
- Effect chaining/stacking
- HDR rendering support
- Multisampling support

**Files**: `PostProcessing.h`

**Usage**:
```cpp
PostProcessingManager* ppMgr = PostProcessingManager::GetInstance();
ppMgr->Initialize(screenWidth, screenHeight);

// Add effects
auto fxaa = std::make_shared<FXAAEffect>();
ppMgr->AddEffect(fxaa);

auto toneMap = std::make_shared<ToneMappingEffect>(ToneMappingEffect::Mode::ACES, 1.0f);
ppMgr->AddEffect(toneMap);

// In render loop
ppMgr->BeginScene();
// Render scene here
ppMgr->EndScene();
ppMgr->ApplyEffects(postProcessShader);
```

---

## Input and Event Handling

### Input Management
**Status**: ✅ Newly Implemented

Centralized input system for keyboard, mouse, and gamepad.

**Features**:
- **Keyboard**: Key press, release, just pressed/released detection
- **Mouse**: Button press, position, delta, scroll
- **Gamepad**: Button and axis input for up to 16 gamepads
- **Action Mapping**: Map actions to keys
- **Event Callbacks**: Custom event handling

**Files**: `InputManager.h`

**Usage**:
```cpp
InputManager* input = InputManager::GetInstance();
input->Initialize(window);

// In update loop
input->Update();

// Check input
if (input->IsKeyPressed(GLFW_KEY_W)) {
    // Move forward
}

if (input->IsKeyJustPressed(GLFW_KEY_SPACE)) {
    // Jump
}

glm::vec2 mousePos = input->GetMousePosition();
glm::vec2 mouseDelta = input->GetMouseDelta();

// Gamepad
if (input->IsGamepadConnected(0)) {
    glm::vec2 leftStick = input->GetGamepadLeftStick(0);
    if (input->IsGamepadButtonPressed(GLFW_GAMEPAD_BUTTON_A, 0)) {
        // A button pressed
    }
}

// Action mapping
input->MapAction("Jump", GLFW_KEY_SPACE);
if (input->IsActionJustPressed("Jump")) {
    // Jump
}
```

### Event System
**Status**: ✅ Newly Implemented

Event bus for decoupled communication between systems.

**Event Types**:
- Window events (resize, close)
- Input events (key, mouse button, mouse move, scroll)
- Custom game events

**Features**:
- Type-safe event dispatching
- Subscribe to specific event types
- Subscribe to all events
- Event queuing for deferred processing
- Event handling/propagation control

**Files**: `EventSystem.h`

**Usage**:
```cpp
EventSystem* eventSys = EventSystem::GetInstance();

// Subscribe to events
eventSys->Subscribe("WindowResize", [](Event& e) {
    auto& resizeEvent = static_cast<WindowResizeEvent&>(e);
    std::cout << "Window resized: " << resizeEvent.Width << "x" << resizeEvent.Height << std::endl;
});

// Publish event immediately
WindowResizeEvent event(1920, 1080);
eventSys->PublishImmediate(event);

// Queue event for later
auto keyEvent = std::make_unique<KeyPressedEvent>(GLFW_KEY_W);
eventSys->QueueEvent(std::move(keyEvent));

// Process queued events (e.g., end of frame)
eventSys->ProcessEvents();
```

---

## Core Engine Systems

### Game Loop
**Status**: ✅ Fully Implemented

Main game loop with fixed timestep and delta time.

**Features**:
- Frame timing calculation
- Delta time for frame-rate independent movement
- Input processing
- System updates
- Rendering

**Files**: `main.cpp`

**Structure**:
```cpp
while (!glfwWindowShouldClose(window)) {
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Process input
    processInput(window);

    // Update systems
    water.Update(deltaTime);
    wind.Update(deltaTime);

    // Render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Render objects...

    // Swap buffers and poll events
    glfwSwapBuffers(window);
    glfwPollEvents();
}
```

### Scene Graph / Entity-Component System
**Status**: ✅ Newly Implemented

Hierarchical scene graph with entity management.

**Components**:
- **Transform**: Position, rotation, scale with parent-child hierarchy
- **Entity**: Base game object with update/render interface
- **Scene**: Container for entities
- **SceneManager**: Manages multiple scenes

**Features**:
- Parent-child transform hierarchy
- Local and world space transformations
- Entity lifecycle management (create, destroy, find)
- Active/inactive entities
- Multiple scene support with scene switching

**Files**: `SceneGraph.h`

**Usage**:
```cpp
SceneManager* sceneMgr = SceneManager::GetInstance();

// Create scene
auto mainScene = sceneMgr->CreateScene("MainScene");

// Create entities
auto player = mainScene->CreateEntity("Player");
player->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

auto weapon = mainScene->CreateEntity("Weapon");
weapon->GetTransform()->SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));

// Create hierarchy
player->AddChild(weapon);  // Weapon follows player

// In game loop
sceneMgr->Update(deltaTime);
sceneMgr->Render();
```

### Resource Management
**Status**: ✅ Fully Implemented

Centralized asset loading and caching system.

**Managed Resources**:
- 3D Models
- Textures
- Materials (via MaterialManager)
- Audio (via AudioManager - stub)

**Features**:
- Automatic caching (no duplicate loading)
- Reference counting with shared pointers
- Resource unloading
- Statistics and debugging

**Files**: `AssetManager.h`, `MaterialSystem.h`, `AudioSystem.h`

**Usage**:
```cpp
AssetManager* assetMgr = AssetManager::GetInstance();

// Load and cache automatically
auto model = assetMgr->LoadModel("assets/tree.fbx", "tree");
auto texture = assetMgr->LoadTexture("bark.png", "assets/textures", "bark");

// Get cached resource (doesn't reload)
auto cachedModel = assetMgr->GetModel("tree");

// Unload when done
assetMgr->UnloadModel("tree");
assetMgr->UnloadTexture("bark");

// Statistics
assetMgr->PrintStats();
```

### Math Library (GLM)
**Status**: ✅ Fully Implemented

Complete mathematics library for 3D transformations.

**Features**:
- Vector math (vec2, vec3, vec4)
- Matrix operations (mat3, mat4)
- Quaternions for rotations
- Transformations (translate, rotate, scale)
- Projection matrices (perspective, orthographic)
- View matrices (lookAt)
- Common math functions

**Files**: External library (GLM), used throughout

**Usage**:
```cpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Vectors
glm::vec3 position(0.0f, 1.0f, 0.0f);
glm::vec3 velocity(1.0f, 0.0f, 0.0f);
position += velocity * deltaTime;

// Matrices
glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, position);
model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(2.0f));

// Projection
glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

// View
glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
```

---

## Optional Features

### Physics Engine
**Status**: ⚠️ Stub Implementation

Basic physics framework with collision detection primitives.

**Implemented**:
- AABB (Axis-Aligned Bounding Box) collision detection
- RigidBody component with velocity, mass, forces
- Collider components (Box, Sphere)
- Gravity simulation
- PhysicsWorld for raycasting (stub)

**For Production**: Integrate a full physics library:
- **Bullet Physics**: https://github.com/bulletphysics/bullet3
- **PhysX** (NVIDIA): https://github.com/NVIDIAGameWorks/PhysX
- **Jolt Physics**: https://github.com/jrouwe/JoltPhysics

**Files**: `PhysicsSystem.h`

**Usage**:
```cpp
// Create rigid body
RigidBody rb(10.0f);  // 10kg mass
rb.SetUseGravity(true);
rb.ApplyForce(glm::vec3(0.0f, 100.0f, 0.0f));  // Apply upward force

// Update physics
rb.Update(deltaTime);
glm::vec3 velocity = rb.GetVelocity();

// Collision detection
BoxCollider box(glm::vec3(2.0f, 2.0f, 2.0f));
AABB aabb = box.GetAABB(position);

BoxCollider other(glm::vec3(1.0f, 1.0f, 1.0f));
AABB otherAABB = other.GetAABB(otherPosition);

if (aabb.Intersects(otherAABB)) {
    // Collision!
}
```

### Audio System
**Status**: ⚠️ Stub Implementation

Audio framework for sound effects and music.

**Implemented**:
- AudioSource with playback control (play, pause, stop)
- 3D spatial audio support (position, min/max distance)
- AudioListener (attached to camera)
- AudioManager for centralized audio control
- Volume control (master, music, SFX)

**For Production**: Integrate an audio library:
- **OpenAL**: https://github.com/kcat/openal-soft (3D audio)
- **miniaudio**: https://github.com/mackron/miniaudio (simple, no dependencies)
- **FMOD**: https://www.fmod.com/ (professional, commercial)
- **SoLoud**: https://github.com/jarikomppa/soloud (easy to use)

**Files**: `AudioSystem.h`

**Usage**:
```cpp
AudioManager* audioMgr = AudioManager::GetInstance();
audioMgr->Initialize();

// Load audio
auto bgMusic = audioMgr->LoadAudio("bgm", "assets/audio/background.mp3");
bgMusic->SetLoop(true);
bgMusic->SetVolume(0.7f);

auto footstep = audioMgr->LoadAudio("footstep", "assets/audio/step.wav");

// Play audio
audioMgr->PlaySound("bgm");
audioMgr->PlaySoundAtPosition("footstep", playerPosition);

// Update listener position (camera)
audioMgr->GetListener().SetPosition(camera.Position);
audioMgr->GetListener().SetOrientation(camera.Front, camera.Up);

audioMgr->Update(deltaTime);
```

### UI System
**Status**: ✅ Newly Implemented

In-game UI system for panels, buttons, and labels.

**Implemented**:
- UIElement base class
- UIPanel (container)
- UIButton (with click callbacks)
- UILabel (text - stub)
- UIManager for centralized UI management
- Mouse interaction (hover, click)

**For Production**: For advanced UI, integrate:
- **Dear ImGui**: https://github.com/ocornut/imgui (immediate mode, great for tools/debug)
- **RmlUi**: https://github.com/mikke89/RmlUi (HTML/CSS-like)
- **Nuklear**: https://github.com/Immediate-Mode-UI/Nuklear

**Files**: `UISystem.h`

**Usage**:
```cpp
UIManager* uiMgr = UIManager::GetInstance();
uiMgr->Initialize(screenWidth, screenHeight);

// Create panel
auto panel = std::make_shared<UIPanel>("MainPanel");
panel->SetPosition(glm::vec2(10.0f, 10.0f));
panel->SetSize(glm::vec2(300.0f, 200.0f));
panel->SetColor(glm::vec4(0.2f, 0.2f, 0.2f, 0.8f));

// Create button
auto button = std::make_shared<UIButton>("PlayButton", "Play");
button->SetPosition(glm::vec2(100.0f, 100.0f));
button->SetSize(glm::vec2(150.0f, 50.0f));
button->SetOnClick([]() {
    std::cout << "Play button clicked!" << std::endl;
});

uiMgr->AddElement(panel);
uiMgr->AddElement(button);

// In game loop
uiMgr->Update(deltaTime);
uiMgr->Render();

// Handle input
uiMgr->HandleMouseMove(mouseX, mouseY);
uiMgr->HandleMouseClick(mouseX, mouseY);
```

### Scripting System
**Status**: ❌ Not Implemented

**Recommended Integration**:
- **Lua**: https://www.lua.org/ (lightweight, easy to embed)
- **ChaiScript**: https://github.com/ChaiScript/ChaiScript (C++ scripting)
- **AngelScript**: https://www.angelcode.com/angelscript/ (C++-like syntax)
- **Python** (embedded): https://docs.python.org/3/extending/embedding.html

**Typical Usage Pattern**:
```cpp
// Pseudocode for Lua integration
LuaEngine* lua = LuaEngine::GetInstance();
lua->Initialize();

// Register C++ functions
lua->RegisterFunction("GetEntityPosition", [](int entityID) {
    return GetEntity(entityID)->GetTransform()->GetPosition();
});

// Run scripts
lua->ExecuteFile("scripts/player.lua");

// Call Lua functions
lua->CallFunction("OnPlayerDamage", playerID, damage);
```

### Debugging Tools
**Status**: ✅ Newly Implemented

Comprehensive debugging and profiling tools.

**Implemented**:
- **Profiler**: Frame timing, FPS counter, section profiling
- **GLDebug**: OpenGL debug message callback
- **MemoryTracker**: GPU memory tracking (textures, buffers)
- **DebugRenderer**: Draw debug shapes (lines, boxes, spheres)
- **Timer**: High-resolution performance timing
- **ScopedTimer**: RAII-style profiling

**Files**: `DebugTools.h`

**Usage**:
```cpp
// Enable OpenGL debug output
GLDebug::EnableDebugOutput();

// Profiler
Profiler* profiler = Profiler::GetInstance();

// In game loop
profiler->BeginFrame();

profiler->BeginSection("Physics");
// Physics code...
profiler->EndSection("Physics");

profiler->BeginSection("Rendering");
// Rendering code...
profiler->EndSection("Rendering");

profiler->EndFrame();

// Print stats
std::cout << profiler->GetStatsString() << std::endl;  // "FPS: 60.0 | Frame: 16.67ms"
profiler->PrintStats();  // Detailed breakdown

// Scoped profiling
{
    PROFILE_SCOPE("ExpensiveFunction");
    // Code to profile...
}  // Automatically prints elapsed time

// Memory tracking
MemoryTracker* memTracker = MemoryTracker::GetInstance();
memTracker->AllocateTexture(textureID, width * height * 4, "DiffuseMap");
memTracker->PrintMemoryStats();

// Debug rendering
DebugRenderer* debugRender = DebugRenderer::GetInstance();
debugRender->DrawBox(position, size, glm::vec3(1.0f, 0.0f, 0.0f));  // Red box
debugRender->DrawLine(start, end, glm::vec3(0.0f, 1.0f, 0.0f));     // Green line
debugRender->DrawSphere(center, radius, glm::vec3(0.0f, 0.0f, 1.0f)); // Blue sphere
```

---

## Integration Examples

### Complete Game Object Example

```cpp
// Create a game entity with multiple components
class Player : public Entity {
public:
    Player() : Entity("Player") {
        // Transform
        GetTransform()->SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));
        
        // Model
        AssetManager* assets = AssetManager::GetInstance();
        m_Model = assets->LoadModel("assets/player.fbx", "player_model");
        
        // Material
        MaterialManager* matMgr = MaterialManager::GetInstance();
        m_Material = matMgr->CreateMaterial("PlayerMaterial");
        m_Material->SetAlbedo(glm::vec3(0.8f, 0.3f, 0.3f));
        m_Material->SetMetallic(0.0f);
        m_Material->SetRoughness(0.6f);
        
        // Physics
        m_RigidBody = std::make_unique<RigidBody>(75.0f);  // 75kg
        m_Collider = std::make_unique<BoxCollider>(glm::vec3(1.0f, 2.0f, 1.0f));
        
        // Audio
        AudioManager* audio = AudioManager::GetInstance();
        m_FootstepSound = audio->LoadAudio("footstep", "assets/audio/step.wav");
    }
    
    void Update(float deltaTime) override {
        // Input
        InputManager* input = InputManager::GetInstance();
        
        glm::vec3 movement(0.0f);
        if (input->IsKeyPressed(GLFW_KEY_W)) movement.z -= 1.0f;
        if (input->IsKeyPressed(GLFW_KEY_S)) movement.z += 1.0f;
        if (input->IsKeyPressed(GLFW_KEY_A)) movement.x -= 1.0f;
        if (input->IsKeyPressed(GLFW_KEY_D)) movement.x += 1.0f;
        
        if (glm::length(movement) > 0.0f) {
            movement = glm::normalize(movement) * m_MoveSpeed * deltaTime;
            GetTransform()->Translate(movement);
            
            // Play footstep sound
            if (m_FootstepTimer <= 0.0f) {
                AudioManager::GetInstance()->PlaySoundAtPosition(
                    "footstep", 
                    GetTransform()->GetPosition()
                );
                m_FootstepTimer = 0.5f;
            }
        }
        
        m_FootstepTimer -= deltaTime;
        
        // Physics
        m_RigidBody->Update(deltaTime);
        glm::vec3 velocity = m_RigidBody->GetVelocity();
        GetTransform()->Translate(velocity * deltaTime);
    }
    
    void Render() override {
        // Setup shader
        Shader* shader = GetShader();  // Assume we have a shader
        shader->use();
        
        // Apply material
        m_Material->ApplyToShader(*shader);
        
        // Apply lighting
        LightManager::GetInstance()->ApplyToShader(*shader);
        
        // Set transform
        shader->setMat4("model", GetTransform()->GetWorldMatrix());
        
        // Draw model
        m_Model->Draw(*shader);
        
        // Debug rendering
        #ifdef _DEBUG
        DebugRenderer* debug = DebugRenderer::GetInstance();
        AABB aabb = m_Collider->GetAABB(GetTransform()->GetPosition());
        debug->DrawBox(aabb.GetCenter(), aabb.GetSize(), glm::vec3(0.0f, 1.0f, 0.0f));
        #endif
    }
    
private:
    std::shared_ptr<Model> m_Model;
    std::shared_ptr<Material> m_Material;
    std::unique_ptr<RigidBody> m_RigidBody;
    std::unique_ptr<BoxCollider> m_Collider;
    std::shared_ptr<AudioSource> m_FootstepSound;
    float m_MoveSpeed = 5.0f;
    float m_FootstepTimer = 0.0f;
};
```

### Complete Game Loop Example

```cpp
int main() {
    // Initialize engine systems
    GLFWwindow* window = InitializeWindow();
    
    InputManager::GetInstance()->Initialize(window);
    EventSystem::GetInstance()->SetEventLogging(true);
    AudioManager::GetInstance()->Initialize();
    UIManager::GetInstance()->Initialize(1280, 720);
    PostProcessingManager::GetInstance()->Initialize(1280, 720);
    MaterialManager::GetInstance()->CreatePresetMaterials();
    GLDebug::EnableDebugOutput();
    
    // Create scene
    SceneManager* sceneMgr = SceneManager::GetInstance();
    auto mainScene = sceneMgr->CreateScene("MainScene");
    sceneMgr->SetActiveScene("MainScene");
    
    // Create player
    auto player = std::make_shared<Player>();
    mainScene->AddEntity(player);
    
    // Setup lighting
    LightManager* lightMgr = LightManager::GetInstance();
    auto sun = std::make_shared<DirectionalLight>(
        glm::vec3(0.5f, -1.0f, -0.3f),
        glm::vec3(1.0f, 1.0f, 0.9f),
        1.2f
    );
    lightMgr->AddDirectionalLight(sun);
    
    // Setup post-processing
    auto fxaa = std::make_shared<FXAAEffect>();
    PostProcessingManager::GetInstance()->AddEffect(fxaa);
    
    // Game loop
    Profiler* profiler = Profiler::GetInstance();
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    while (!glfwWindowShouldClose(window)) {
        // Timing
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        profiler->BeginFrame();
        
        // Input
        profiler->BeginSection("Input");
        InputManager::GetInstance()->Update();
        glfwPollEvents();
        profiler->EndSection("Input");
        
        // Events
        profiler->BeginSection("Events");
        EventSystem::GetInstance()->ProcessEvents();
        profiler->EndSection("Events");
        
        // Update
        profiler->BeginSection("Update");
        sceneMgr->Update(deltaTime);
        AudioManager::GetInstance()->Update(deltaTime);
        UIManager::GetInstance()->Update(deltaTime);
        profiler->EndSection("Update");
        
        // Render
        profiler->BeginSection("Render");
        PostProcessingManager::GetInstance()->BeginScene();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        sceneMgr->Render();
        
        PostProcessingManager::GetInstance()->EndScene();
        PostProcessingManager::GetInstance()->ApplyEffects(postProcessShader);
        
        UIManager::GetInstance()->Render();
        profiler->EndSection("Render");
        
        glfwSwapBuffers(window);
        
        profiler->EndFrame();
        
        // Print stats every second
        if (profiler->GetFrameCount() % 60 == 0) {
            std::cout << profiler->GetStatsString() << std::endl;
        }
    }
    
    // Cleanup
    AudioManager::GetInstance()->Shutdown();
    SceneManager::GetInstance()->ClearAll();
    
    glfwTerminate();
    return 0;
}
```

---

## File Structure

```
GameEngine/
├── src/
│   ├── main.cpp                  # Entry point and game loop
│   ├── Camera.h                  # ✅ Camera system
│   ├── Shader.h                  # ✅ Shader management
│   ├── Model.h                   # ✅ 3D model loading
│   ├── TextureLoader.h           # ✅ Texture loading
│   ├── TextureGenerator.h        # ✅ Procedural textures
│   ├── AssetManager.h            # ✅ Resource management
│   ├── Terrain.h                 # ✅ Procedural terrain
│   ├── Water.h                   # ✅ Water system
│   ├── Wind.h                    # ✅ Wind system
│   ├── InputManager.h            # ✅ NEW - Input handling
│   ├── EventSystem.h             # ✅ NEW - Event bus
│   ├── LightSystem.h             # ✅ NEW - Advanced lighting
│   ├── MaterialSystem.h          # ✅ NEW - PBR materials
│   ├── PostProcessing.h          # ✅ NEW - Post-processing
│   ├── PhysicsSystem.h           # ⚠️ NEW - Physics stub
│   ├── AudioSystem.h             # ⚠️ NEW - Audio stub
│   ├── UISystem.h                # ✅ NEW - UI system
│   ├── SceneGraph.h              # ✅ NEW - Scene/Entity system
│   └── DebugTools.h              # ✅ NEW - Debug/profiling
├── external/                     # Third-party libraries
│   ├── glfw/                     # Window management
│   ├── glm/                      # Math library
│   ├── assimp/                   # Model loading
│   ├── glad/                     # OpenGL loader
│   └── stb/                      # Image loading
└── CMakeLists.txt               # Build configuration
```

---

## Summary

### ✅ Fully Implemented Systems
1. Window and Context Management (GLFW)
2. Shader Management
3. Mesh and Model Loading (Assimp)
4. Texture Management (stb_image)
5. Camera System
6. Lighting System (NEW)
7. Material System (NEW)
8. Post-Processing Framework (NEW)
9. Input Management (NEW)
10. Event System (NEW)
11. Game Loop
12. Scene Graph / ECS (NEW)
13. Resource Management
14. Math Library (GLM)
15. UI System (NEW)
16. Debugging Tools (NEW)

### ⚠️ Stub/Partial Implementations
1. Physics System - Basic framework, integrate Bullet/PhysX for production
2. Audio System - Basic framework, integrate OpenAL/FMOD for production

### ❌ Not Implemented (Recommended)
1. Scripting System - Integrate Lua/ChaiScript for game logic

---

## Next Steps

1. **Testing**: Test all new systems with example code
2. **Integration**: Integrate physics library (Bullet recommended)
3. **Integration**: Integrate audio library (OpenAL recommended)
4. **Documentation**: Create API reference documentation
5. **Examples**: Create sample projects demonstrating each system
6. **Scripting**: Add Lua scripting support
7. **Advanced Features**: Shadows, deferred rendering, particles, etc.

