#ifndef LIGHTSYSTEM_H
#define LIGHTSYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>
#include "Shader.h"

// Base Light class
class Light {
public:
    enum class Type {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    Light(Type type, const glm::vec3& color = glm::vec3(1.0f), float intensity = 1.0f)
        : m_Type(type), m_Color(color), m_Intensity(intensity), m_Enabled(true) {}

    virtual ~Light() = default;

    Type GetType() const { return m_Type; }
    glm::vec3 GetColor() const { return m_Color; }
    float GetIntensity() const { return m_Intensity; }
    bool IsEnabled() const { return m_Enabled; }

    void SetColor(const glm::vec3& color) { m_Color = color; }
    void SetIntensity(float intensity) { m_Intensity = intensity; }
    void SetEnabled(bool enabled) { m_Enabled = enabled; }

    virtual void ApplyToShader(Shader& shader, int index) const = 0;

protected:
    Type m_Type;
    glm::vec3 m_Color;
    float m_Intensity;
    bool m_Enabled;
};

// Directional Light (like sun)
class DirectionalLight : public Light {
public:
    DirectionalLight(const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f),
                     const glm::vec3& color = glm::vec3(1.0f),
                     float intensity = 1.0f)
        : Light(Type::DIRECTIONAL, color, intensity), m_Direction(glm::normalize(direction)) {}

    glm::vec3 GetDirection() const { return m_Direction; }
    void SetDirection(const glm::vec3& direction) { m_Direction = glm::normalize(direction); }

    void ApplyToShader(Shader& shader, int index) const override {
        std::string base = "dirLights[" + std::to_string(index) + "]";
        shader.setVec3(base + ".direction", m_Direction);
        shader.setVec3(base + ".color", m_Color * m_Intensity);
        shader.setBool(base + ".enabled", m_Enabled);
    }

private:
    glm::vec3 m_Direction;
};

// Point Light (like bulb)
class PointLight : public Light {
public:
    PointLight(const glm::vec3& position = glm::vec3(0.0f),
               const glm::vec3& color = glm::vec3(1.0f),
               float intensity = 1.0f,
               float constant = 1.0f,
               float linear = 0.09f,
               float quadratic = 0.032f)
        : Light(Type::POINT, color, intensity),
          m_Position(position),
          m_Constant(constant),
          m_Linear(linear),
          m_Quadratic(quadratic) {}

    glm::vec3 GetPosition() const { return m_Position; }
    void SetPosition(const glm::vec3& position) { m_Position = position; }

    float GetConstant() const { return m_Constant; }
    float GetLinear() const { return m_Linear; }
    float GetQuadratic() const { return m_Quadratic; }

    void SetAttenuation(float constant, float linear, float quadratic) {
        m_Constant = constant;
        m_Linear = linear;
        m_Quadratic = quadratic;
    }

    // Calculate effective radius based on attenuation
    float GetRadius(float threshold = 0.01f) const {
        return (-m_Linear + std::sqrt(m_Linear * m_Linear - 4 * m_Quadratic * (m_Constant - (256.0f / threshold) * m_Intensity))) / (2 * m_Quadratic);
    }

    void ApplyToShader(Shader& shader, int index) const override {
        std::string base = "pointLights[" + std::to_string(index) + "]";
        shader.setVec3(base + ".position", m_Position);
        shader.setVec3(base + ".color", m_Color * m_Intensity);
        shader.setFloat(base + ".constant", m_Constant);
        shader.setFloat(base + ".linear", m_Linear);
        shader.setFloat(base + ".quadratic", m_Quadratic);
        shader.setBool(base + ".enabled", m_Enabled);
    }

private:
    glm::vec3 m_Position;
    float m_Constant;
    float m_Linear;
    float m_Quadratic;
};

// Spot Light (like flashlight)
class SpotLight : public Light {
public:
    SpotLight(const glm::vec3& position = glm::vec3(0.0f),
              const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f),
              const glm::vec3& color = glm::vec3(1.0f),
              float intensity = 1.0f,
              float cutOff = glm::cos(glm::radians(12.5f)),
              float outerCutOff = glm::cos(glm::radians(17.5f)))
        : Light(Type::SPOT, color, intensity),
          m_Position(position),
          m_Direction(glm::normalize(direction)),
          m_CutOff(cutOff),
          m_OuterCutOff(outerCutOff) {}

    glm::vec3 GetPosition() const { return m_Position; }
    glm::vec3 GetDirection() const { return m_Direction; }
    float GetCutOff() const { return m_CutOff; }
    float GetOuterCutOff() const { return m_OuterCutOff; }

    void SetPosition(const glm::vec3& position) { m_Position = position; }
    void SetDirection(const glm::vec3& direction) { m_Direction = glm::normalize(direction); }
    void SetCutOff(float cutOff) { m_CutOff = cutOff; }
    void SetOuterCutOff(float outerCutOff) { m_OuterCutOff = outerCutOff; }

    void ApplyToShader(Shader& shader, int index) const override {
        std::string base = "spotLights[" + std::to_string(index) + "]";
        shader.setVec3(base + ".position", m_Position);
        shader.setVec3(base + ".direction", m_Direction);
        shader.setVec3(base + ".color", m_Color * m_Intensity);
        shader.setFloat(base + ".cutOff", m_CutOff);
        shader.setFloat(base + ".outerCutOff", m_OuterCutOff);
        shader.setBool(base + ".enabled", m_Enabled);
    }

private:
    glm::vec3 m_Position;
    glm::vec3 m_Direction;
    float m_CutOff;
    float m_OuterCutOff;
};

// Light Manager
class LightManager {
public:
    static LightManager* GetInstance() {
        static LightManager instance;
        return &instance;
    }

    // Add lights
    void AddDirectionalLight(std::shared_ptr<DirectionalLight> light) {
        m_DirectionalLights.push_back(light);
    }

    void AddPointLight(std::shared_ptr<PointLight> light) {
        m_PointLights.push_back(light);
    }

    void AddSpotLight(std::shared_ptr<SpotLight> light) {
        m_SpotLights.push_back(light);
    }

    // Remove lights
    void ClearAllLights() {
        m_DirectionalLights.clear();
        m_PointLights.clear();
        m_SpotLights.clear();
    }

    // Apply all lights to shader
    void ApplyToShader(Shader& shader) const {
        shader.use();
        
        // Apply directional lights
        shader.setInt("numDirLights", static_cast<int>(m_DirectionalLights.size()));
        for (size_t i = 0; i < m_DirectionalLights.size(); i++) {
            m_DirectionalLights[i]->ApplyToShader(shader, i);
        }

        // Apply point lights
        shader.setInt("numPointLights", static_cast<int>(m_PointLights.size()));
        for (size_t i = 0; i < m_PointLights.size(); i++) {
            m_PointLights[i]->ApplyToShader(shader, i);
        }

        // Apply spot lights
        shader.setInt("numSpotLights", static_cast<int>(m_SpotLights.size()));
        for (size_t i = 0; i < m_SpotLights.size(); i++) {
            m_SpotLights[i]->ApplyToShader(shader, i);
        }
    }

    // Getters
    const std::vector<std::shared_ptr<DirectionalLight>>& GetDirectionalLights() const { return m_DirectionalLights; }
    const std::vector<std::shared_ptr<PointLight>>& GetPointLights() const { return m_PointLights; }
    const std::vector<std::shared_ptr<SpotLight>>& GetSpotLights() const { return m_SpotLights; }

    // Global ambient light
    void SetAmbientLight(const glm::vec3& ambient) { m_AmbientLight = ambient; }
    glm::vec3 GetAmbientLight() const { return m_AmbientLight; }

private:
    LightManager() : m_AmbientLight(0.2f, 0.2f, 0.2f) {}
    ~LightManager() {}

    LightManager(const LightManager&) = delete;
    LightManager& operator=(const LightManager&) = delete;

    std::vector<std::shared_ptr<DirectionalLight>> m_DirectionalLights;
    std::vector<std::shared_ptr<PointLight>> m_PointLights;
    std::vector<std::shared_ptr<SpotLight>> m_SpotLights;
    glm::vec3 m_AmbientLight;
};

#endif // LIGHTSYSTEM_H
