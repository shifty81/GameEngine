#ifndef WIND_H
#define WIND_H

#include <glm/glm.hpp>
#include <cmath>

class Wind {
public:
    glm::vec3 direction;
    float strength;
    float time;
    float baseStrength;
    float gustFrequency;

    Wind(glm::vec3 dir = glm::vec3(1.0f, 0.0f, 0.0f), float str = 1.0f) 
        : direction(glm::normalize(dir)), baseStrength(str), strength(str), time(0.0f), gustFrequency(0.5f) {
    }

    void Update(float deltaTime) {
        time += deltaTime;
        
        // Simulate wind gusts using sine waves
        float gust = sin(time * gustFrequency) * 0.3f + sin(time * gustFrequency * 2.0f) * 0.2f;
        strength = baseStrength + gust * baseStrength;
        
        // Slight direction variation
        float dirVariation = sin(time * 0.3f) * 0.2f;
        direction.x = cos(dirVariation);
        direction.z = sin(dirVariation);
        direction = glm::normalize(direction);
    }

    glm::vec3 getForce() const {
        return direction * strength;
    }

    float getStrength() const {
        return strength;
    }

    glm::vec3 getDirection() const {
        return direction;
    }

    // Calculate rotation for windmill based on wind
    float getWindmillRotation() const {
        return time * strength * 2.0f; // Rotation speed based on wind strength
    }
};

#endif
