#include "EngineCore.h"
#include "VoxelEngine.h"
#include "Renderer.h"
#include "Camera.h"
#include <memory>

namespace {
    std::unique_ptr<VoxelEngine> g_voxelEngine;
    std::unique_ptr<Renderer> g_renderer;
    std::unique_ptr<Camera> g_camera;
    bool g_editorMode = false;
}

extern "C" {

bool InitializeEngine(void* hwnd, int width, int height) {
    try {
        // Initialize renderer with DirectX 11
        g_renderer = std::make_unique<Renderer>();
        if (!g_renderer->Initialize(hwnd, width, height)) {
            return false;
        }
        
        // Initialize camera
        g_camera = std::make_unique<Camera>();
        g_camera->SetPosition(50.0f, 30.0f, 50.0f);
        g_camera->SetAspectRatio(static_cast<float>(width) / height);
        
        // Initialize voxel engine
        g_voxelEngine = std::make_unique<VoxelEngine>();
        g_voxelEngine->Initialize();
        
        return true;
    }
    catch (...) {
        return false;
    }
}

void ShutdownEngine() {
    g_voxelEngine.reset();
    g_camera.reset();
    g_renderer.reset();
}

void UpdateEngine(float deltaTime) {
    if (g_voxelEngine) {
        g_voxelEngine->Update(deltaTime);
    }
    if (g_camera) {
        g_camera->Update(deltaTime);
    }
}

void RenderEngine() {
    if (!g_renderer || !g_voxelEngine || !g_camera) {
        return;
    }
    
    g_renderer->BeginFrame();
    g_voxelEngine->Render(g_renderer.get(), g_camera.get());
    g_renderer->EndFrame();
}

void ResizeViewport(int width, int height) {
    if (g_renderer) {
        g_renderer->Resize(width, height);
    }
    if (g_camera) {
        g_camera->SetAspectRatio(static_cast<float>(width) / height);
    }
}

void SetCameraPosition(float x, float y, float z) {
    if (g_camera) {
        g_camera->SetPosition(x, y, z);
    }
}

void GetCameraPosition(float* x, float* y, float* z) {
    if (g_camera && x && y && z) {
        auto pos = g_camera->GetPosition();
        *x = pos.x;
        *y = pos.y;
        *z = pos.z;
    }
}

void SetCameraRotation(float pitch, float yaw) {
    if (g_camera) {
        g_camera->SetRotation(pitch, yaw);
    }
}

void MoveCameraForward(float distance) {
    if (g_camera) {
        g_camera->MoveForward(distance);
    }
}

void MoveCameraRight(float distance) {
    if (g_camera) {
        g_camera->MoveRight(distance);
    }
}

void MoveCameraUp(float distance) {
    if (g_camera) {
        g_camera->MoveUp(distance);
    }
}

void SetVoxel(int x, int y, int z, uint8_t blockType) {
    if (g_voxelEngine) {
        g_voxelEngine->SetVoxel(x, y, z, blockType);
    }
}

uint8_t GetVoxel(int x, int y, int z) {
    if (g_voxelEngine) {
        return g_voxelEngine->GetVoxel(x, y, z);
    }
    return 0;
}

void GenerateTerrain(int seed) {
    if (g_voxelEngine) {
        g_voxelEngine->GenerateTerrain(seed);
    }
}

void SetEditorMode(bool enabled) {
    g_editorMode = enabled;
}

bool IsEditorMode() {
    return g_editorMode;
}

void ProcessMouseMove(float deltaX, float deltaY) {
    if (g_camera && !g_editorMode) {
        g_camera->ProcessMouseMovement(deltaX, deltaY);
    }
}

void ProcessMouseWheel(float delta) {
    if (g_camera) {
        g_camera->ProcessMouseScroll(delta);
    }
}

void ProcessKeyInput(int keyCode, bool pressed) {
    // Handle keyboard input
    // This can be expanded based on game needs
}

} // extern "C"
