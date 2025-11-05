#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    bool Initialize(void* hwnd, int width, int height);
    void Shutdown();
    void Resize(int width, int height);
    
    void BeginFrame();
    void EndFrame();
    
    void Clear(float r, float g, float b, float a);
    
    ID3D11Device* GetDevice() { return m_device.Get(); }
    ID3D11DeviceContext* GetContext() { return m_context.Get(); }
    
private:
    bool CreateDeviceAndSwapChain(void* hwnd, int width, int height);
    bool CreateRenderTargets();
    void CleanupRenderTargets();
    
    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;
    ComPtr<IDXGISwapChain> m_swapChain;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
    ComPtr<ID3D11DepthStencilView> m_depthStencilView;
    ComPtr<ID3D11DepthStencilState> m_depthStencilState;
    ComPtr<ID3D11RasterizerState> m_rasterizerState;
    
    int m_width;
    int m_height;
};
