#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include "Model.h"
#include "TextureLoader.h"

class AssetManager {
private:
    std::unordered_map<std::string, std::shared_ptr<Model>> models;
    std::unordered_map<std::string, unsigned int> textures;
    
    static AssetManager* instance;
    
    AssetManager() {}

public:
    static AssetManager* GetInstance() {
        if (instance == nullptr) {
            instance = new AssetManager();
        }
        return instance;
    }

    // Load a 3D model from file (.obj, .fbx, .gltf)
    std::shared_ptr<Model> LoadModel(const std::string& path, const std::string& name = "") {
        std::string modelName = name.empty() ? path : name;
        
        // Check if model is already loaded
        auto it = models.find(modelName);
        if (it != models.end()) {
            std::cout << "Model already loaded: " << modelName << std::endl;
            return it->second;
        }
        
        // Load the model
        std::cout << "Loading model: " << path << std::endl;
        auto model = std::make_shared<Model>(path);
        models[modelName] = model;
        return model;
    }

    // Get a loaded model by name
    std::shared_ptr<Model> GetModel(const std::string& name) {
        auto it = models.find(name);
        if (it != models.end()) {
            return it->second;
        }
        std::cout << "Model not found: " << name << std::endl;
        return nullptr;
    }

    // Load a texture from file
    unsigned int LoadTexture(const std::string& path, const std::string& directory = "", const std::string& name = "") {
        std::string textureName = name.empty() ? path : name;
        
        // Check if texture is already loaded
        auto it = textures.find(textureName);
        if (it != textures.end()) {
            std::cout << "Texture already loaded: " << textureName << std::endl;
            return it->second;
        }
        
        // Load the texture
        std::cout << "Loading texture: " << path << std::endl;
        unsigned int textureID = TextureFromFile(path.c_str(), directory);
        textures[textureName] = textureID;
        return textureID;
    }

    // Get a loaded texture by name
    unsigned int GetTexture(const std::string& name) {
        auto it = textures.find(name);
        if (it != textures.end()) {
            return it->second;
        }
        std::cout << "Texture not found: " << name << std::endl;
        return 0;
    }

    // Unload a model
    void UnloadModel(const std::string& name) {
        auto it = models.find(name);
        if (it != models.end()) {
            models.erase(it);
            std::cout << "Model unloaded: " << name << std::endl;
        }
    }

    // Unload a texture
    void UnloadTexture(const std::string& name) {
        auto it = textures.find(name);
        if (it != textures.end()) {
            glDeleteTextures(1, &it->second);
            textures.erase(it);
            std::cout << "Texture unloaded: " << name << std::endl;
        }
    }

    // Clear all assets
    void ClearAll() {
        models.clear();
        for (auto& tex : textures) {
            glDeleteTextures(1, &tex.second);
        }
        textures.clear();
        std::cout << "All assets cleared" << std::endl;
    }

    // Get stats
    void PrintStats() {
        std::cout << "=== Asset Manager Stats ===" << std::endl;
        std::cout << "Models loaded: " << models.size() << std::endl;
        std::cout << "Textures loaded: " << textures.size() << std::endl;
        std::cout << "===========================" << std::endl;
    }
};

// Initialize static instance
AssetManager* AssetManager::instance = nullptr;

#endif
