#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>
#include <memory>
#include <map>

// Transform component
class Transform {
public:
    Transform(const glm::vec3& position = glm::vec3(0.0f),
              const glm::vec3& rotation = glm::vec3(0.0f),
              const glm::vec3& scale = glm::vec3(1.0f))
        : m_Position(position),
          m_Rotation(rotation),
          m_Scale(scale),
          m_Parent(nullptr) {
        UpdateMatrix();
    }

    // Getters
    glm::vec3 GetPosition() const { return m_Position; }
    glm::vec3 GetRotation() const { return m_Rotation; }
    glm::vec3 GetScale() const { return m_Scale; }
    glm::mat4 GetLocalMatrix() const { return m_LocalMatrix; }
    glm::mat4 GetWorldMatrix() const {
        if (m_Parent) {
            return m_Parent->GetWorldMatrix() * m_LocalMatrix;
        }
        return m_LocalMatrix;
    }

    // Setters
    void SetPosition(const glm::vec3& position) {
        m_Position = position;
        UpdateMatrix();
    }

    void SetRotation(const glm::vec3& rotation) {
        m_Rotation = rotation;
        UpdateMatrix();
    }

    void SetScale(const glm::vec3& scale) {
        m_Scale = scale;
        UpdateMatrix();
    }

    // Transform operations
    void Translate(const glm::vec3& offset) {
        m_Position += offset;
        UpdateMatrix();
    }

    void Rotate(const glm::vec3& eulerAngles) {
        m_Rotation += eulerAngles;
        UpdateMatrix();
    }

    void Scale(const glm::vec3& scale) {
        m_Scale *= scale;
        UpdateMatrix();
    }

    // Get direction vectors
    glm::vec3 GetForward() const {
        glm::mat4 worldMatrix = GetWorldMatrix();
        return glm::normalize(glm::vec3(worldMatrix[2]));
    }

    glm::vec3 GetRight() const {
        glm::mat4 worldMatrix = GetWorldMatrix();
        return glm::normalize(glm::vec3(worldMatrix[0]));
    }

    glm::vec3 GetUp() const {
        glm::mat4 worldMatrix = GetWorldMatrix();
        return glm::normalize(glm::vec3(worldMatrix[1]));
    }

    // Hierarchy
    void SetParent(Transform* parent) {
        m_Parent = parent;
    }

    Transform* GetParent() const {
        return m_Parent;
    }

private:
    void UpdateMatrix() {
        m_LocalMatrix = glm::mat4(1.0f);
        m_LocalMatrix = glm::translate(m_LocalMatrix, m_Position);
        m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
        m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
        m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
        m_LocalMatrix = glm::scale(m_LocalMatrix, m_Scale);
    }

    glm::vec3 m_Position;
    glm::vec3 m_Rotation;  // Euler angles in degrees
    glm::vec3 m_Scale;
    glm::mat4 m_LocalMatrix;
    Transform* m_Parent;
};

// Entity - Represents a game object in the scene
class Entity {
public:
    Entity(const std::string& name = "Entity", unsigned int id = 0)
        : m_Name(name), m_ID(id), m_Active(true) {
        m_Transform = std::make_shared<Transform>();
    }

    virtual ~Entity() = default;

    // Core interface
    virtual void Update(float deltaTime) {}
    virtual void Render() {}

    // Getters/Setters
    std::string GetName() const { return m_Name; }
    void SetName(const std::string& name) { m_Name = name; }

    unsigned int GetID() const { return m_ID; }

    bool IsActive() const { return m_Active; }
    void SetActive(bool active) { m_Active = active; }

    std::shared_ptr<Transform> GetTransform() { return m_Transform; }

    // Hierarchy
    void AddChild(std::shared_ptr<Entity> child) {
        child->GetTransform()->SetParent(m_Transform.get());
        m_Children.push_back(child);
    }

    void RemoveChild(unsigned int id) {
        m_Children.erase(
            std::remove_if(m_Children.begin(), m_Children.end(),
                [id](const std::shared_ptr<Entity>& child) {
                    return child->GetID() == id;
                }),
            m_Children.end()
        );
    }

    const std::vector<std::shared_ptr<Entity>>& GetChildren() const {
        return m_Children;
    }

    // Update all children
    void UpdateHierarchy(float deltaTime) {
        if (!m_Active) return;
        
        Update(deltaTime);
        
        for (auto& child : m_Children) {
            if (child) {
                child->UpdateHierarchy(deltaTime);
            }
        }
    }

    // Render all children
    void RenderHierarchy() {
        if (!m_Active) return;
        
        Render();
        
        for (auto& child : m_Children) {
            if (child) {
                child->RenderHierarchy();
            }
        }
    }

protected:
    std::string m_Name;
    unsigned int m_ID;
    bool m_Active;
    std::shared_ptr<Transform> m_Transform;
    std::vector<std::shared_ptr<Entity>> m_Children;
};

// Scene - Container for entities
class Scene {
public:
    Scene(const std::string& name = "Scene") : m_Name(name), m_NextEntityID(1) {}

    std::string GetName() const { return m_Name; }
    void SetName(const std::string& name) { m_Name = name; }

    // Entity management
    std::shared_ptr<Entity> CreateEntity(const std::string& name = "Entity") {
        auto entity = std::make_shared<Entity>(name, m_NextEntityID++);
        m_Entities[entity->GetID()] = entity;
        return entity;
    }

    void AddEntity(std::shared_ptr<Entity> entity) {
        if (entity->GetID() == 0) {
            // Assign new ID if not set
            entity = std::make_shared<Entity>(entity->GetName(), m_NextEntityID++);
        }
        m_Entities[entity->GetID()] = entity;
    }

    void RemoveEntity(unsigned int id) {
        m_Entities.erase(id);
    }

    std::shared_ptr<Entity> GetEntity(unsigned int id) {
        auto it = m_Entities.find(id);
        if (it != m_Entities.end()) {
            return it->second;
        }
        return nullptr;
    }

    std::shared_ptr<Entity> FindEntityByName(const std::string& name) {
        for (auto& pair : m_Entities) {
            if (pair.second->GetName() == name) {
                return pair.second;
            }
        }
        return nullptr;
    }

    // Get all entities
    std::vector<std::shared_ptr<Entity>> GetAllEntities() {
        std::vector<std::shared_ptr<Entity>> entities;
        for (auto& pair : m_Entities) {
            entities.push_back(pair.second);
        }
        return entities;
    }

    // Scene update
    void Update(float deltaTime) {
        for (auto& pair : m_Entities) {
            if (pair.second && pair.second->IsActive()) {
                pair.second->UpdateHierarchy(deltaTime);
            }
        }
    }

    // Scene render
    void Render() {
        for (auto& pair : m_Entities) {
            if (pair.second && pair.second->IsActive()) {
                pair.second->RenderHierarchy();
            }
        }
    }

    // Clear scene
    void Clear() {
        m_Entities.clear();
        m_NextEntityID = 1;
    }

    size_t GetEntityCount() const {
        return m_Entities.size();
    }

private:
    std::string m_Name;
    std::map<unsigned int, std::shared_ptr<Entity>> m_Entities;
    unsigned int m_NextEntityID;
};

// Scene Manager - Manages multiple scenes
class SceneManager {
public:
    static SceneManager* GetInstance() {
        static SceneManager instance;
        return &instance;
    }

    // Scene management
    std::shared_ptr<Scene> CreateScene(const std::string& name) {
        auto scene = std::make_shared<Scene>(name);
        m_Scenes[name] = scene;
        
        if (!m_ActiveScene) {
            m_ActiveScene = scene;
        }
        
        return scene;
    }

    void AddScene(std::shared_ptr<Scene> scene) {
        m_Scenes[scene->GetName()] = scene;
        
        if (!m_ActiveScene) {
            m_ActiveScene = scene;
        }
    }

    void RemoveScene(const std::string& name) {
        auto it = m_Scenes.find(name);
        if (it != m_Scenes.end()) {
            if (m_ActiveScene == it->second) {
                m_ActiveScene = nullptr;
            }
            m_Scenes.erase(it);
        }
    }

    std::shared_ptr<Scene> GetScene(const std::string& name) {
        auto it = m_Scenes.find(name);
        if (it != m_Scenes.end()) {
            return it->second;
        }
        return nullptr;
    }

    // Active scene
    void SetActiveScene(const std::string& name) {
        auto scene = GetScene(name);
        if (scene) {
            m_ActiveScene = scene;
        }
    }

    std::shared_ptr<Scene> GetActiveScene() {
        return m_ActiveScene;
    }

    // Update/Render active scene
    void Update(float deltaTime) {
        if (m_ActiveScene) {
            m_ActiveScene->Update(deltaTime);
        }
    }

    void Render() {
        if (m_ActiveScene) {
            m_ActiveScene->Render();
        }
    }

    // Clear all scenes
    void ClearAll() {
        m_Scenes.clear();
        m_ActiveScene = nullptr;
    }

private:
    SceneManager() {}
    ~SceneManager() {}

    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    std::map<std::string, std::shared_ptr<Scene>> m_Scenes;
    std::shared_ptr<Scene> m_ActiveScene;
};

// For more advanced ECS, consider:
// - EnTT: https://github.com/skypjack/entt
// - flecs: https://github.com/SanderMertens/flecs
// - EntityX: https://github.com/alecthomas/entityx

#endif // SCENEGRAPH_H
