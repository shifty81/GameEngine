#pragma once

#ifdef GAMEENGINECORE_EXPORTS
#define ENGINECORE_API __declspec(dllexport)
#else
#define ENGINECORE_API __declspec(dllimport)
#endif

#include <cstdint>

extern "C" {
    // Engine initialization and shutdown
    ENGINECORE_API bool InitializeEngine(void* hwnd, int width, int height);
    ENGINECORE_API void ShutdownEngine();
    
    // Engine update and render
    ENGINECORE_API void UpdateEngine(float deltaTime);
    ENGINECORE_API void RenderEngine();
    ENGINECORE_API void ResizeViewport(int width, int height);
    
    // Camera controls
    ENGINECORE_API void SetCameraPosition(float x, float y, float z);
    ENGINECORE_API void GetCameraPosition(float* x, float* y, float* z);
    ENGINECORE_API void SetCameraRotation(float pitch, float yaw);
    ENGINECORE_API void MoveCameraForward(float distance);
    ENGINECORE_API void MoveCameraRight(float distance);
    ENGINECORE_API void MoveCameraUp(float distance);
    
    // Voxel operations
    ENGINECORE_API void SetVoxel(int x, int y, int z, uint8_t blockType);
    ENGINECORE_API uint8_t GetVoxel(int x, int y, int z);
    ENGINECORE_API void GenerateTerrain(int seed);
    
    // Editor mode
    ENGINECORE_API void SetEditorMode(bool enabled);
    ENGINECORE_API bool IsEditorMode();
    
    // Input handling
    ENGINECORE_API void ProcessMouseMove(float deltaX, float deltaY);
    ENGINECORE_API void ProcessMouseWheel(float delta);
    ENGINECORE_API void ProcessKeyInput(int keyCode, bool pressed);
}
