#ifndef MATERIALSYSTEM_H
#define MATERIALSYSTEM_H

#include <glm/glm.hpp>
#include <string>
#include <map>
#include <memory>
#include "Shader.h"

// Material class with PBR (Physically Based Rendering) properties
class Material {
public:
    Material(const std::string& name = "Default")
        : m_Name(name),
          m_Albedo(1.0f, 1.0f, 1.0f),
          m_Metallic(0.0f),
          m_Roughness(0.5f),
          m_AO(1.0f),
          m_Emission(0.0f, 0.0f, 0.0f),
          m_EmissionStrength(0.0f),
          m_AlbedoMap(0),
          m_NormalMap(0),
          m_MetallicMap(0),
          m_RoughnessMap(0),
          m_AOMap(0),
          m_EmissionMap(0),
          m_UseAlbedoMap(false),
          m_UseNormalMap(false),
          m_UseMetallicMap(false),
          m_UseRoughnessMap(false),
          m_UseAOMap(false),
          m_UseEmissionMap(false),
          m_TwoSided(false),
          m_Transparent(false),
          m_AlphaBlend(1.0f) {}

    // Setters for properties
    void SetAlbedo(const glm::vec3& albedo) { m_Albedo = albedo; }
    void SetMetallic(float metallic) { m_Metallic = glm::clamp(metallic, 0.0f, 1.0f); }
    void SetRoughness(float roughness) { m_Roughness = glm::clamp(roughness, 0.0f, 1.0f); }
    void SetAO(float ao) { m_AO = ao; }
    void SetEmission(const glm::vec3& emission) { m_Emission = emission; }
    void SetEmissionStrength(float strength) { m_EmissionStrength = strength; }
    void SetTwoSided(bool twoSided) { m_TwoSided = twoSided; }
    void SetTransparent(bool transparent) { m_Transparent = transparent; }
    void SetAlphaBlend(float alpha) { m_AlphaBlend = glm::clamp(alpha, 0.0f, 1.0f); }

    // Setters for texture maps
    void SetAlbedoMap(unsigned int texture) { m_AlbedoMap = texture; m_UseAlbedoMap = true; }
    void SetNormalMap(unsigned int texture) { m_NormalMap = texture; m_UseNormalMap = true; }
    void SetMetallicMap(unsigned int texture) { m_MetallicMap = texture; m_UseMetallicMap = true; }
    void SetRoughnessMap(unsigned int texture) { m_RoughnessMap = texture; m_UseRoughnessMap = true; }
    void SetAOMap(unsigned int texture) { m_AOMap = texture; m_UseAOMap = true; }
    void SetEmissionMap(unsigned int texture) { m_EmissionMap = texture; m_UseEmissionMap = true; }

    // Getters
    std::string GetName() const { return m_Name; }
    glm::vec3 GetAlbedo() const { return m_Albedo; }
    float GetMetallic() const { return m_Metallic; }
    float GetRoughness() const { return m_Roughness; }
    float GetAO() const { return m_AO; }
    glm::vec3 GetEmission() const { return m_Emission; }
    float GetEmissionStrength() const { return m_EmissionStrength; }
    bool IsTwoSided() const { return m_TwoSided; }
    bool IsTransparent() const { return m_Transparent; }
    float GetAlphaBlend() const { return m_AlphaBlend; }

    // Apply material to shader
    void ApplyToShader(Shader& shader, const std::string& uniformPrefix = "material") const {
        shader.use();

        // Base properties
        shader.setVec3(uniformPrefix + ".albedo", m_Albedo);
        shader.setFloat(uniformPrefix + ".metallic", m_Metallic);
        shader.setFloat(uniformPrefix + ".roughness", m_Roughness);
        shader.setFloat(uniformPrefix + ".ao", m_AO);
        shader.setVec3(uniformPrefix + ".emission", m_Emission * m_EmissionStrength);
        shader.setFloat(uniformPrefix + ".alpha", m_AlphaBlend);

        // Texture maps
        int textureUnit = 0;

        if (m_UseAlbedoMap && m_AlbedoMap != 0) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, m_AlbedoMap);
            shader.setInt(uniformPrefix + ".albedoMap", textureUnit);
            shader.setBool(uniformPrefix + ".useAlbedoMap", true);
            textureUnit++;
        } else {
            shader.setBool(uniformPrefix + ".useAlbedoMap", false);
        }

        if (m_UseNormalMap && m_NormalMap != 0) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, m_NormalMap);
            shader.setInt(uniformPrefix + ".normalMap", textureUnit);
            shader.setBool(uniformPrefix + ".useNormalMap", true);
            textureUnit++;
        } else {
            shader.setBool(uniformPrefix + ".useNormalMap", false);
        }

        if (m_UseMetallicMap && m_MetallicMap != 0) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, m_MetallicMap);
            shader.setInt(uniformPrefix + ".metallicMap", textureUnit);
            shader.setBool(uniformPrefix + ".useMetallicMap", true);
            textureUnit++;
        } else {
            shader.setBool(uniformPrefix + ".useMetallicMap", false);
        }

        if (m_UseRoughnessMap && m_RoughnessMap != 0) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, m_RoughnessMap);
            shader.setInt(uniformPrefix + ".roughnessMap", textureUnit);
            shader.setBool(uniformPrefix + ".useRoughnessMap", true);
            textureUnit++;
        } else {
            shader.setBool(uniformPrefix + ".useRoughnessMap", false);
        }

        if (m_UseAOMap && m_AOMap != 0) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, m_AOMap);
            shader.setInt(uniformPrefix + ".aoMap", textureUnit);
            shader.setBool(uniformPrefix + ".useAOMap", true);
            textureUnit++;
        } else {
            shader.setBool(uniformPrefix + ".useAOMap", false);
        }

        if (m_UseEmissionMap && m_EmissionMap != 0) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, m_EmissionMap);
            shader.setInt(uniformPrefix + ".emissionMap", textureUnit);
            shader.setBool(uniformPrefix + ".useEmissionMap", true);
            textureUnit++;
        } else {
            shader.setBool(uniformPrefix + ".useEmissionMap", false);
        }
    }

private:
    std::string m_Name;

    // PBR properties
    glm::vec3 m_Albedo;          // Base color
    float m_Metallic;            // Metallic factor (0-1)
    float m_Roughness;           // Roughness factor (0-1)
    float m_AO;                  // Ambient occlusion (0-1)
    glm::vec3 m_Emission;        // Emission color
    float m_EmissionStrength;    // Emission strength multiplier

    // Texture maps
    unsigned int m_AlbedoMap;
    unsigned int m_NormalMap;
    unsigned int m_MetallicMap;
    unsigned int m_RoughnessMap;
    unsigned int m_AOMap;
    unsigned int m_EmissionMap;

    // Flags
    bool m_UseAlbedoMap;
    bool m_UseNormalMap;
    bool m_UseMetallicMap;
    bool m_UseRoughnessMap;
    bool m_UseAOMap;
    bool m_UseEmissionMap;
    bool m_TwoSided;
    bool m_Transparent;
    float m_AlphaBlend;
};

// Material Manager
class MaterialManager {
public:
    static MaterialManager* GetInstance() {
        static MaterialManager instance;
        return &instance;
    }

    // Create and register a material
    std::shared_ptr<Material> CreateMaterial(const std::string& name) {
        auto material = std::make_shared<Material>(name);
        m_Materials[name] = material;
        return material;
    }

    // Get a material by name
    std::shared_ptr<Material> GetMaterial(const std::string& name) {
        auto it = m_Materials.find(name);
        if (it != m_Materials.end()) {
            return it->second;
        }
        return nullptr;
    }

    // Register an existing material
    void RegisterMaterial(const std::string& name, std::shared_ptr<Material> material) {
        m_Materials[name] = material;
    }

    // Remove a material
    void RemoveMaterial(const std::string& name) {
        m_Materials.erase(name);
    }

    // Clear all materials
    void ClearAllMaterials() {
        m_Materials.clear();
    }

    // Get material count
    size_t GetMaterialCount() const {
        return m_Materials.size();
    }

    // Create some preset materials
    void CreatePresetMaterials() {
        // Glossy plastic
        auto plastic = CreateMaterial("Plastic");
        plastic->SetAlbedo(glm::vec3(0.8f, 0.2f, 0.2f));
        plastic->SetMetallic(0.0f);
        plastic->SetRoughness(0.2f);

        // Rough metal
        auto metal = CreateMaterial("Metal");
        metal->SetAlbedo(glm::vec3(0.7f, 0.7f, 0.7f));
        metal->SetMetallic(1.0f);
        metal->SetRoughness(0.3f);

        // Gold
        auto gold = CreateMaterial("Gold");
        gold->SetAlbedo(glm::vec3(1.0f, 0.86f, 0.57f));
        gold->SetMetallic(1.0f);
        gold->SetRoughness(0.1f);

        // Wood
        auto wood = CreateMaterial("Wood");
        wood->SetAlbedo(glm::vec3(0.6f, 0.3f, 0.1f));
        wood->SetMetallic(0.0f);
        wood->SetRoughness(0.8f);

        // Glass
        auto glass = CreateMaterial("Glass");
        glass->SetAlbedo(glm::vec3(0.9f, 0.9f, 0.9f));
        glass->SetMetallic(0.0f);
        glass->SetRoughness(0.05f);
        glass->SetTransparent(true);
        glass->SetAlphaBlend(0.3f);

        // Emissive
        auto emissive = CreateMaterial("Emissive");
        emissive->SetAlbedo(glm::vec3(1.0f, 1.0f, 1.0f));
        emissive->SetEmission(glm::vec3(1.0f, 0.5f, 0.0f));
        emissive->SetEmissionStrength(2.0f);
    }

private:
    MaterialManager() {}
    ~MaterialManager() {}

    MaterialManager(const MaterialManager&) = delete;
    MaterialManager& operator=(const MaterialManager&) = delete;

    std::map<std::string, std::shared_ptr<Material>> m_Materials;
};

#endif // MATERIALSYSTEM_H
