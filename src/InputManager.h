#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <functional>

// Input Manager - Centralized input handling for keyboard, mouse, and gamepad
class InputManager {
public:
    // Singleton access
    static InputManager* GetInstance() {
        static InputManager instance;
        return &instance;
    }

    // Initialize with GLFW window
    void Initialize(GLFWwindow* window) {
        m_Window = window;
        
        // Set up GLFW callbacks
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
        glfwSetCursorPosCallback(window, CursorPositionCallback);
        glfwSetScrollCallback(window, ScrollCallback);
        glfwSetJoystickCallback(JoystickCallback);
    }

    // Update input states (call once per frame)
    void Update() {
        // Update previous frame states
        m_PreviousKeys = m_Keys;
        m_PreviousMouseButtons = m_MouseButtons;
        
        // Poll gamepad states
        UpdateGamepadStates();
    }

    // Keyboard input
    bool IsKeyPressed(int key) const {
        auto it = m_Keys.find(key);
        return it != m_Keys.end() && it->second;
    }

    bool IsKeyJustPressed(int key) const {
        return IsKeyPressed(key) && !WasKeyPressed(key);
    }

    bool IsKeyJustReleased(int key) const {
        return !IsKeyPressed(key) && WasKeyPressed(key);
    }

    // Mouse input
    bool IsMouseButtonPressed(int button) const {
        auto it = m_MouseButtons.find(button);
        return it != m_MouseButtons.end() && it->second;
    }

    bool IsMouseButtonJustPressed(int button) const {
        return IsMouseButtonPressed(button) && !WasMouseButtonPressed(button);
    }

    glm::vec2 GetMousePosition() const { return m_MousePosition; }
    glm::vec2 GetMouseDelta() const { return m_MouseDelta; }
    float GetScrollDelta() const { return m_ScrollDelta; }

    // Mouse cursor control
    void SetCursorMode(int mode) {
        glfwSetInputMode(m_Window, GLFW_CURSOR, mode);
    }

    // Gamepad input
    bool IsGamepadConnected(int gamepadID = 0) const {
        return glfwJoystickPresent(gamepadID);
    }

    bool IsGamepadButtonPressed(int button, int gamepadID = 0) const {
        if (gamepadID >= m_GamepadStates.size()) return false;
        if (button >= m_GamepadStates[gamepadID].buttonCount) return false;
        return m_GamepadStates[gamepadID].buttons[button] == GLFW_PRESS;
    }

    float GetGamepadAxis(int axis, int gamepadID = 0) const {
        if (gamepadID >= m_GamepadStates.size()) return 0.0f;
        if (axis >= m_GamepadStates[gamepadID].axisCount) return 0.0f;
        return m_GamepadStates[gamepadID].axes[axis];
    }

    glm::vec2 GetGamepadLeftStick(int gamepadID = 0) const {
        return glm::vec2(GetGamepadAxis(0, gamepadID), GetGamepadAxis(1, gamepadID));
    }

    glm::vec2 GetGamepadRightStick(int gamepadID = 0) const {
        return glm::vec2(GetGamepadAxis(2, gamepadID), GetGamepadAxis(3, gamepadID));
    }

    // Input action mapping
    void MapAction(const std::string& actionName, int key) {
        m_ActionMappings[actionName] = key;
    }

    bool IsActionPressed(const std::string& actionName) const {
        auto it = m_ActionMappings.find(actionName);
        if (it != m_ActionMappings.end()) {
            return IsKeyPressed(it->second);
        }
        return false;
    }

    bool IsActionJustPressed(const std::string& actionName) const {
        auto it = m_ActionMappings.find(actionName);
        if (it != m_ActionMappings.end()) {
            return IsKeyJustPressed(it->second);
        }
        return false;
    }

    // Event callbacks (for custom handling)
    using KeyEventCallback = std::function<void(int key, int action)>;
    using MouseButtonCallback = std::function<void(int button, int action)>;
    using MouseMoveCallback = std::function<void(double x, double y)>;

    void AddKeyEventListener(KeyEventCallback callback) {
        m_KeyEventCallbacks.push_back(callback);
    }

    void AddMouseButtonListener(MouseButtonCallback callback) {
        m_MouseButtonCallbacks.push_back(callback);
    }

    void AddMouseMoveListener(MouseMoveCallback callback) {
        m_MouseMoveCallbacks.push_back(callback);
    }

private:
    InputManager() {}
    ~InputManager() {}

    // Prevent copying
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    bool WasKeyPressed(int key) const {
        auto it = m_PreviousKeys.find(key);
        return it != m_PreviousKeys.end() && it->second;
    }

    bool WasMouseButtonPressed(int button) const {
        auto it = m_PreviousMouseButtons.find(button);
        return it != m_PreviousMouseButtons.end() && it->second;
    }

    void UpdateGamepadStates() {
        m_GamepadStates.clear();
        for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; i++) {
            if (glfwJoystickPresent(i)) {
                GamepadState state;
                state.buttons = glfwGetJoystickButtons(i, &state.buttonCount);
                state.axes = glfwGetJoystickAxes(i, &state.axisCount);
                m_GamepadStates.push_back(state);
            }
        }
    }

    // Static callbacks that forward to instance
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        InputManager* instance = GetInstance();
        
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            instance->m_Keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            instance->m_Keys[key] = false;
        }

        // Notify listeners
        for (auto& callback : instance->m_KeyEventCallbacks) {
            callback(key, action);
        }
    }

    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        InputManager* instance = GetInstance();
        
        if (action == GLFW_PRESS) {
            instance->m_MouseButtons[button] = true;
        } else if (action == GLFW_RELEASE) {
            instance->m_MouseButtons[button] = false;
        }

        // Notify listeners
        for (auto& callback : instance->m_MouseButtonCallbacks) {
            callback(button, action);
        }
    }

    static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
        InputManager* instance = GetInstance();
        
        glm::vec2 newPos(xpos, ypos);
        instance->m_MouseDelta = newPos - instance->m_MousePosition;
        instance->m_MousePosition = newPos;

        // Notify listeners
        for (auto& callback : instance->m_MouseMoveCallbacks) {
            callback(xpos, ypos);
        }
    }

    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        InputManager* instance = GetInstance();
        instance->m_ScrollDelta = static_cast<float>(yoffset);
    }

    static void JoystickCallback(int jid, int event) {
        if (event == GLFW_CONNECTED) {
            std::cout << "Gamepad " << jid << " connected" << std::endl;
        } else if (event == GLFW_DISCONNECTED) {
            std::cout << "Gamepad " << jid << " disconnected" << std::endl;
        }
    }

    struct GamepadState {
        const unsigned char* buttons;
        int buttonCount;
        const float* axes;
        int axisCount;
    };

    GLFWwindow* m_Window = nullptr;
    
    // Current and previous frame states
    std::map<int, bool> m_Keys;
    std::map<int, bool> m_PreviousKeys;
    std::map<int, bool> m_MouseButtons;
    std::map<int, bool> m_PreviousMouseButtons;
    
    glm::vec2 m_MousePosition = glm::vec2(0.0f);
    glm::vec2 m_MouseDelta = glm::vec2(0.0f);
    float m_ScrollDelta = 0.0f;
    
    std::vector<GamepadState> m_GamepadStates;
    
    // Action mappings
    std::map<std::string, int> m_ActionMappings;
    
    // Event callbacks
    std::vector<KeyEventCallback> m_KeyEventCallbacks;
    std::vector<MouseButtonCallback> m_MouseButtonCallbacks;
    std::vector<MouseMoveCallback> m_MouseMoveCallbacks;
};

#endif // INPUTMANAGER_H
