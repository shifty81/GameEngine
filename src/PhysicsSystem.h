#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <glm/glm.hpp>
#include <vector>
#include <memory>

// Simple AABB (Axis-Aligned Bounding Box) for collision detection
struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    AABB() : min(0.0f), max(0.0f) {}
    AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}

    bool Intersects(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }

    glm::vec3 GetCenter() const {
        return (min + max) * 0.5f;
    }

    glm::vec3 GetSize() const {
        return max - min;
    }
};

// Rigid Body component
class RigidBody {
public:
    RigidBody(float mass = 1.0f) 
        : m_Mass(mass), 
          m_InvMass(mass > 0.0f ? 1.0f / mass : 0.0f),
          m_Velocity(0.0f),
          m_Acceleration(0.0f),
          m_UseGravity(true),
          m_IsKinematic(false) {}

    void ApplyForce(const glm::vec3& force) {
        if (!m_IsKinematic && m_InvMass > 0.0f) {
            m_Acceleration += force * m_InvMass;
        }
    }

    void ApplyImpulse(const glm::vec3& impulse) {
        if (!m_IsKinematic && m_InvMass > 0.0f) {
            m_Velocity += impulse * m_InvMass;
        }
    }

    void Update(float deltaTime) {
        if (m_IsKinematic) return;

        // Apply gravity
        if (m_UseGravity && m_InvMass > 0.0f) {
            m_Acceleration += glm::vec3(0.0f, -9.81f, 0.0f);
        }

        // Integration (Euler)
        m_Velocity += m_Acceleration * deltaTime;
        
        // Apply damping
        m_Velocity *= (1.0f - m_Damping * deltaTime);
        
        // Reset acceleration for next frame
        m_Acceleration = glm::vec3(0.0f);
    }

    glm::vec3 GetVelocity() const { return m_Velocity; }
    void SetVelocity(const glm::vec3& velocity) { m_Velocity = velocity; }
    
    float GetMass() const { return m_Mass; }
    void SetMass(float mass) { 
        m_Mass = mass; 
        m_InvMass = mass > 0.0f ? 1.0f / mass : 0.0f;
    }
    
    void SetKinematic(bool kinematic) { m_IsKinematic = kinematic; }
    bool IsKinematic() const { return m_IsKinematic; }
    
    void SetUseGravity(bool useGravity) { m_UseGravity = useGravity; }
    bool UsesGravity() const { return m_UseGravity; }
    
    void SetDamping(float damping) { m_Damping = glm::clamp(damping, 0.0f, 1.0f); }
    float GetDamping() const { return m_Damping; }

private:
    float m_Mass;
    float m_InvMass;
    glm::vec3 m_Velocity;
    glm::vec3 m_Acceleration;
    float m_Damping = 0.05f;
    bool m_UseGravity;
    bool m_IsKinematic;
};

// Collider component
class Collider {
public:
    enum class Type {
        BOX,
        SPHERE,
        CAPSULE
    };

    Collider(Type type = Type::BOX) : m_Type(type), m_IsTrigger(false) {}

    virtual ~Collider() = default;
    virtual AABB GetAABB(const glm::vec3& position) const = 0;

    Type GetType() const { return m_Type; }
    bool IsTrigger() const { return m_IsTrigger; }
    void SetTrigger(bool trigger) { m_IsTrigger = trigger; }

protected:
    Type m_Type;
    bool m_IsTrigger;
};

// Box Collider
class BoxCollider : public Collider {
public:
    BoxCollider(const glm::vec3& size = glm::vec3(1.0f)) 
        : Collider(Type::BOX), m_Size(size) {}

    AABB GetAABB(const glm::vec3& position) const override {
        glm::vec3 halfSize = m_Size * 0.5f;
        return AABB(position - halfSize, position + halfSize);
    }

    glm::vec3 GetSize() const { return m_Size; }
    void SetSize(const glm::vec3& size) { m_Size = size; }

private:
    glm::vec3 m_Size;
};

// Sphere Collider
class SphereCollider : public Collider {
public:
    SphereCollider(float radius = 0.5f) 
        : Collider(Type::SPHERE), m_Radius(radius) {}

    AABB GetAABB(const glm::vec3& position) const override {
        glm::vec3 radiusVec(m_Radius);
        return AABB(position - radiusVec, position + radiusVec);
    }

    float GetRadius() const { return m_Radius; }
    void SetRadius(float radius) { m_Radius = radius; }

private:
    float m_Radius;
};

// Physics World - manages all physics objects
class PhysicsWorld {
public:
    static PhysicsWorld* GetInstance() {
        static PhysicsWorld instance;
        return &instance;
    }

    void SetGravity(const glm::vec3& gravity) { m_Gravity = gravity; }
    glm::vec3 GetGravity() const { return m_Gravity; }

    // Raycasting
    struct RaycastHit {
        bool hit;
        glm::vec3 point;
        glm::vec3 normal;
        float distance;
        void* object;  // Pointer to the hit object
    };

    RaycastHit Raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance = 1000.0f) {
        // Simple raycast implementation stub
        RaycastHit result;
        result.hit = false;
        result.distance = maxDistance;
        result.object = nullptr;
        
        // TODO: Implement actual raycasting against colliders
        
        return result;
    }

    // Check if point is inside any collider
    bool OverlapPoint(const glm::vec3& point) {
        // TODO: Implement point overlap check
        return false;
    }

    // Get all colliders overlapping with AABB
    std::vector<Collider*> OverlapBox(const AABB& box) {
        // TODO: Implement box overlap check
        return std::vector<Collider*>();
    }

private:
    PhysicsWorld() : m_Gravity(0.0f, -9.81f, 0.0f) {}
    ~PhysicsWorld() {}

    PhysicsWorld(const PhysicsWorld&) = delete;
    PhysicsWorld& operator=(const PhysicsWorld&) = delete;

    glm::vec3 m_Gravity;
};

// NOTE: For a full physics engine, consider integrating:
// - Bullet Physics: https://github.com/bulletphysics/bullet3
// - PhysX: https://github.com/NVIDIAGameWorks/PhysX
// - Box2D (2D): https://github.com/erincatto/box2d
// - Jolt Physics: https://github.com/jrouwe/JoltPhysics

#endif // PHYSICSSYSTEM_H
