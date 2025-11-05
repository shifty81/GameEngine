#pragma once

#include <DirectXMath.h>

class Camera {
public:
    Camera();
    ~Camera() = default;
    
    void Update(float deltaTime);
    
    void SetPosition(float x, float y, float z);
    DirectX::XMFLOAT3 GetPosition() const { return m_position; }
    
    void SetRotation(float pitch, float yaw);
    void SetAspectRatio(float aspectRatio);
    
    void MoveForward(float distance);
    void MoveRight(float distance);
    void MoveUp(float distance);
    
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
    
    DirectX::XMMATRIX GetViewMatrix() const;
    DirectX::XMMATRIX GetProjectionMatrix() const;
    DirectX::XMFLOAT3 GetForward() const { return m_forward; }
    DirectX::XMFLOAT3 GetRight() const { return m_right; }
    DirectX::XMFLOAT3 GetUp() const { return m_up; }
    
private:
    void UpdateCameraVectors();
    
    DirectX::XMFLOAT3 m_position;
    DirectX::XMFLOAT3 m_forward;
    DirectX::XMFLOAT3 m_up;
    DirectX::XMFLOAT3 m_right;
    DirectX::XMFLOAT3 m_worldUp;
    
    float m_yaw;
    float m_pitch;
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoom;
    
    float m_aspectRatio;
    float m_nearPlane;
    float m_farPlane;
};
