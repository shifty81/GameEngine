#include "Camera.h"
#include <algorithm>

using namespace DirectX;

Camera::Camera()
    : m_position(0.0f, 0.0f, 0.0f)
    , m_worldUp(0.0f, 1.0f, 0.0f)
    , m_yaw(-90.0f)
    , m_pitch(0.0f)
    , m_movementSpeed(10.0f)
    , m_mouseSensitivity(0.1f)
    , m_zoom(45.0f)
    , m_aspectRatio(16.0f / 9.0f)
    , m_nearPlane(0.1f)
    , m_farPlane(1000.0f)
{
    UpdateCameraVectors();
}

void Camera::Update(float deltaTime) {
    // Camera update logic if needed
}

void Camera::SetPosition(float x, float y, float z) {
    m_position = XMFLOAT3(x, y, z);
}

void Camera::SetRotation(float pitch, float yaw) {
    m_pitch = pitch;
    m_yaw = yaw;
    UpdateCameraVectors();
}

void Camera::SetAspectRatio(float aspectRatio) {
    m_aspectRatio = aspectRatio;
}

void Camera::MoveForward(float distance) {
    XMVECTOR pos = XMLoadFloat3(&m_position);
    XMVECTOR forward = XMLoadFloat3(&m_forward);
    pos = XMVectorAdd(pos, XMVectorScale(forward, distance));
    XMStoreFloat3(&m_position, pos);
}

void Camera::MoveRight(float distance) {
    XMVECTOR pos = XMLoadFloat3(&m_position);
    XMVECTOR right = XMLoadFloat3(&m_right);
    pos = XMVectorAdd(pos, XMVectorScale(right, distance));
    XMStoreFloat3(&m_position, pos);
}

void Camera::MoveUp(float distance) {
    XMVECTOR pos = XMLoadFloat3(&m_position);
    XMVECTOR up = XMLoadFloat3(&m_worldUp);
    pos = XMVectorAdd(pos, XMVectorScale(up, distance));
    XMStoreFloat3(&m_position, pos);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (constrainPitch) {
        m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
    }

    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    m_zoom -= yoffset;
    m_zoom = std::clamp(m_zoom, 1.0f, 120.0f);
}

XMMATRIX Camera::GetViewMatrix() const {
    XMVECTOR pos = XMLoadFloat3(&m_position);
    XMVECTOR forward = XMLoadFloat3(&m_forward);
    XMVECTOR up = XMLoadFloat3(&m_up);
    XMVECTOR target = XMVectorAdd(pos, forward);
    return XMMatrixLookAtLH(pos, target, up);
}

XMMATRIX Camera::GetProjectionMatrix() const {
    return XMMatrixPerspectiveFovLH(
        XMConvertToRadians(m_zoom),
        m_aspectRatio,
        m_nearPlane,
        m_farPlane
    );
}

void Camera::UpdateCameraVectors() {
    // Calculate the new Forward vector
    XMFLOAT3 forward;
    float yawRad = XMConvertToRadians(m_yaw);
    float pitchRad = XMConvertToRadians(m_pitch);
    
    forward.x = cosf(yawRad) * cosf(pitchRad);
    forward.y = sinf(pitchRad);
    forward.z = sinf(yawRad) * cosf(pitchRad);
    
    XMVECTOR forwardVec = XMLoadFloat3(&forward);
    forwardVec = XMVector3Normalize(forwardVec);
    XMStoreFloat3(&m_forward, forwardVec);
    
    // Calculate Right and Up vectors
    XMVECTOR worldUp = XMLoadFloat3(&m_worldUp);
    XMVECTOR rightVec = XMVector3Normalize(XMVector3Cross(forwardVec, worldUp));
    XMStoreFloat3(&m_right, rightVec);
    
    XMVECTOR upVec = XMVector3Normalize(XMVector3Cross(rightVec, forwardVec));
    XMStoreFloat3(&m_up, upVec);
}
