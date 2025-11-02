#ifndef WATER_H
#define WATER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"

class Water {
public:
    unsigned int VAO, VBO, EBO;
    float waterLevel;
    int width, depth;
    float time;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    Water(float level, int w, int d) : waterLevel(level), width(w), depth(d), time(0.0f) {
        generateWaterPlane();
        setupMesh();
    }

    void Update(float deltaTime) {
        time += deltaTime;
    }

    void Draw(Shader& shader) {
        shader.setFloat("time", time);
        shader.setFloat("waterLevel", waterLevel);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    glm::vec3 getFlowDirection(float x, float z) {
        // Simple flow pattern - can be made more complex
        float flowX = sin(x * 0.1f + time * 0.5f) * 0.5f;
        float flowZ = cos(z * 0.1f + time * 0.5f) * 0.5f;
        return glm::normalize(glm::vec3(flowX, 0.0f, flowZ));
    }

    ~Water() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

private:
    void generateWaterPlane() {
        vertices.clear();
        indices.clear();

        float tileSize = 1.0f;
        
        // Generate water plane vertices
        for (int z = 0; z <= depth; z++) {
            for (int x = 0; x <= width; x++) {
                // Position
                vertices.push_back((float)x * tileSize);
                vertices.push_back(waterLevel);
                vertices.push_back((float)z * tileSize);
                
                // Normal
                vertices.push_back(0.0f);
                vertices.push_back(1.0f);
                vertices.push_back(0.0f);
                
                // Texture coordinates
                vertices.push_back((float)x / width);
                vertices.push_back((float)z / depth);
            }
        }

        // Generate indices
        for (int z = 0; z < depth; z++) {
            for (int x = 0; x < width; x++) {
                int topLeft = z * (width + 1) + x;
                int topRight = topLeft + 1;
                int bottomLeft = (z + 1) * (width + 1) + x;
                int bottomRight = bottomLeft + 1;

                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }
    }

    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

        // Normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

        // TexCoords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

        glBindVertexArray(0);
    }
};

#endif
