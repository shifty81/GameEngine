#ifndef UISYSTEM_H
#define UISYSTEM_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "Shader.h"

// UI Element base class
class UIElement {
public:
    UIElement(const std::string& name = "UIElement")
        : m_Name(name),
          m_Position(0.0f),
          m_Size(100.0f, 50.0f),
          m_Color(1.0f),
          m_Visible(true),
          m_Enabled(true),
          m_Hovered(false) {}

    virtual ~UIElement() = default;

    virtual void Render(Shader& shader) = 0;
    virtual void Update(float deltaTime) {}
    virtual bool HandleMouseClick(float x, float y) { return false; }
    virtual bool HandleMouseMove(float x, float y) {
        m_Hovered = IsPointInside(x, y);
        return m_Hovered;
    }

    // Getters/Setters
    void SetPosition(const glm::vec2& pos) { m_Position = pos; }
    glm::vec2 GetPosition() const { return m_Position; }

    void SetSize(const glm::vec2& size) { m_Size = size; }
    glm::vec2 GetSize() const { return m_Size; }

    void SetColor(const glm::vec4& color) { m_Color = color; }
    glm::vec4 GetColor() const { return m_Color; }

    void SetVisible(bool visible) { m_Visible = visible; }
    bool IsVisible() const { return m_Visible; }

    void SetEnabled(bool enabled) { m_Enabled = enabled; }
    bool IsEnabled() const { return m_Enabled; }

    bool IsHovered() const { return m_Hovered; }

    std::string GetName() const { return m_Name; }

protected:
    bool IsPointInside(float x, float y) const {
        return x >= m_Position.x && x <= m_Position.x + m_Size.x &&
               y >= m_Position.y && y <= m_Position.y + m_Size.y;
    }

    std::string m_Name;
    glm::vec2 m_Position;
    glm::vec2 m_Size;
    glm::vec4 m_Color;
    bool m_Visible;
    bool m_Enabled;
    bool m_Hovered;
};

// UI Panel - Container for other UI elements
class UIPanel : public UIElement {
public:
    UIPanel(const std::string& name = "Panel")
        : UIElement(name), m_VAO(0), m_VBO(0) {
        SetupMesh();
    }

    ~UIPanel() {
        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO) glDeleteBuffers(1, &m_VBO);
    }

    void Render(Shader& shader) override {
        if (!m_Visible) return;

        shader.use();
        shader.setVec2("position", m_Position);
        shader.setVec2("size", m_Size);
        shader.setVec4("color", m_Color);

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // Render children
        for (auto& child : m_Children) {
            if (child) child->Render(shader);
        }
    }

    void AddChild(std::shared_ptr<UIElement> child) {
        m_Children.push_back(child);
    }

    void RemoveChild(const std::string& name) {
        m_Children.erase(
            std::remove_if(m_Children.begin(), m_Children.end(),
                [&name](const std::shared_ptr<UIElement>& child) {
                    return child->GetName() == name;
                }),
            m_Children.end()
        );
    }

private:
    void SetupMesh() {
        float vertices[] = {
            0.0f, 1.0f,  // Top-left
            0.0f, 0.0f,  // Bottom-left
            1.0f, 0.0f,  // Bottom-right
            0.0f, 1.0f,  // Top-left
            1.0f, 0.0f,  // Bottom-right
            1.0f, 1.0f   // Top-right
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

        glBindVertexArray(0);
    }

    unsigned int m_VAO, m_VBO;
    std::vector<std::shared_ptr<UIElement>> m_Children;
};

// UI Button
class UIButton : public UIElement {
public:
    using ButtonCallback = std::function<void()>;

    UIButton(const std::string& name = "Button", const std::string& text = "Click")
        : UIElement(name), m_Text(text), m_VAO(0), m_VBO(0) {
        SetupMesh();
        m_NormalColor = glm::vec4(0.3f, 0.3f, 0.3f, 0.8f);
        m_HoverColor = glm::vec4(0.4f, 0.4f, 0.4f, 0.9f);
        m_ClickColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
        m_Color = m_NormalColor;
    }

    ~UIButton() {
        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO) glDeleteBuffers(1, &m_VBO);
    }

    void Render(Shader& shader) override {
        if (!m_Visible) return;

        // Update color based on state
        if (m_Hovered) {
            m_Color = m_HoverColor;
        } else {
            m_Color = m_NormalColor;
        }

        shader.use();
        shader.setVec2("position", m_Position);
        shader.setVec2("size", m_Size);
        shader.setVec4("color", m_Color);

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // TODO: Render text (requires text rendering system)
    }

    bool HandleMouseClick(float x, float y) override {
        if (!m_Enabled || !m_Visible) return false;

        if (IsPointInside(x, y)) {
            m_Color = m_ClickColor;
            if (m_OnClick) {
                m_OnClick();
            }
            return true;
        }
        return false;
    }

    void SetOnClick(ButtonCallback callback) {
        m_OnClick = callback;
    }

    void SetText(const std::string& text) { m_Text = text; }
    std::string GetText() const { return m_Text; }

private:
    void SetupMesh() {
        float vertices[] = {
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 1.0f
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

        glBindVertexArray(0);
    }

    std::string m_Text;
    unsigned int m_VAO, m_VBO;
    ButtonCallback m_OnClick;
    glm::vec4 m_NormalColor;
    glm::vec4 m_HoverColor;
    glm::vec4 m_ClickColor;
};

// UI Text Label (stub - requires font rendering)
class UILabel : public UIElement {
public:
    UILabel(const std::string& name = "Label", const std::string& text = "")
        : UIElement(name), m_Text(text), m_FontSize(16.0f) {}

    void Render(Shader& shader) override {
        if (!m_Visible) return;
        // TODO: Implement text rendering
        // For full text rendering, integrate:
        // - FreeType: https://www.freetype.org/
        // - stb_truetype: https://github.com/nothings/stb
    }

    void SetText(const std::string& text) { m_Text = text; }
    std::string GetText() const { return m_Text; }

    void SetFontSize(float size) { m_FontSize = size; }
    float GetFontSize() const { return m_FontSize; }

private:
    std::string m_Text;
    float m_FontSize;
};

// UI Manager
class UIManager {
public:
    static UIManager* GetInstance() {
        static UIManager instance;
        return &instance;
    }

    void Initialize(int screenWidth, int screenHeight) {
        m_ScreenWidth = screenWidth;
        m_ScreenHeight = screenHeight;
        CreateDefaultShader();
        std::cout << "[UI] UI System Initialized" << std::endl;
    }

    void Resize(int width, int height) {
        m_ScreenWidth = width;
        m_ScreenHeight = height;
    }

    void AddElement(std::shared_ptr<UIElement> element) {
        m_Elements.push_back(element);
    }

    void RemoveElement(const std::string& name) {
        m_Elements.erase(
            std::remove_if(m_Elements.begin(), m_Elements.end(),
                [&name](const std::shared_ptr<UIElement>& elem) {
                    return elem->GetName() == name;
                }),
            m_Elements.end()
        );
    }

    void Update(float deltaTime) {
        for (auto& element : m_Elements) {
            if (element) element->Update(deltaTime);
        }
    }

    void Render() {
        if (!m_UIShader) return;

        // Disable depth test for UI
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_UIShader->use();
        m_UIShader->setVec2("screenSize", glm::vec2(m_ScreenWidth, m_ScreenHeight));

        for (auto& element : m_Elements) {
            if (element && element->IsVisible()) {
                element->Render(*m_UIShader);
            }
        }

        glEnable(GL_DEPTH_TEST);
    }

    void HandleMouseClick(float x, float y) {
        // Process in reverse order (top elements first)
        for (auto it = m_Elements.rbegin(); it != m_Elements.rend(); ++it) {
            if ((*it)->HandleMouseClick(x, y)) {
                break;  // Stop after first element handles the click
            }
        }
    }

    void HandleMouseMove(float x, float y) {
        for (auto& element : m_Elements) {
            if (element) element->HandleMouseMove(x, y);
        }
    }

    void ClearAll() {
        m_Elements.clear();
    }

private:
    UIManager() : m_ScreenWidth(1280), m_ScreenHeight(720) {}
    ~UIManager() {}

    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;

    void CreateDefaultShader() {
        const char* vertexShader = R"(
            #version 330 core
            layout (location = 0) in vec2 aPos;
            
            uniform vec2 position;
            uniform vec2 size;
            uniform vec2 screenSize;
            
            void main() {
                vec2 scaledPos = aPos * size + position;
                vec2 normalizedPos = (scaledPos / screenSize) * 2.0 - 1.0;
                normalizedPos.y = -normalizedPos.y;  // Flip Y for screen space
                gl_Position = vec4(normalizedPos, 0.0, 1.0);
            }
        )";

        const char* fragmentShader = R"(
            #version 330 core
            out vec4 FragColor;
            uniform vec4 color;
            
            void main() {
                FragColor = color;
            }
        )";

        m_UIShader = std::make_unique<Shader>(vertexShader, fragmentShader, false);
    }

    std::vector<std::shared_ptr<UIElement>> m_Elements;
    std::unique_ptr<Shader> m_UIShader;
    int m_ScreenWidth;
    int m_ScreenHeight;
};

// Note: For advanced UI features, consider integrating:
// - Dear ImGui: https://github.com/ocornut/imgui (for debug/tools UI)
// - Nuklear: https://github.com/Immediate-Mode-UI/Nuklear
// - RmlUi: https://github.com/mikke89/RmlUi (HTML/CSS-like UI)

#endif // UISYSTEM_H
