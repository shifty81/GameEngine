using System;
using System.Runtime.InteropServices;

namespace GameEngine.Editor
{
    public static class EngineInterop
    {
        private const string DllName = "GameEngine.Core.dll";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool InitializeEngine(IntPtr hwnd, int width, int height);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ShutdownEngine();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void UpdateEngine(float deltaTime);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void RenderEngine();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ResizeViewport(int width, int height);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetCameraPosition(float x, float y, float z);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetCameraPosition(out float x, out float y, out float z);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetCameraRotation(float pitch, float yaw);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void MoveCameraForward(float distance);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void MoveCameraRight(float distance);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void MoveCameraUp(float distance);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetVoxel(int x, int y, int z, byte blockType);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte GetVoxel(int x, int y, int z);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void GenerateTerrain(int seed);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetEditorMode(bool enabled);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool IsEditorMode();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ProcessMouseMove(float deltaX, float deltaY);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ProcessMouseWheel(float delta);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ProcessKeyInput(int keyCode, bool pressed);
    }
}
