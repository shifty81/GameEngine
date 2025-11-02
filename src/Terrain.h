#ifndef TERRAIN_H
#define TERRAIN_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include "Shader.h"

class PerlinNoise {
private:
    std::vector<int> p;

public:
    PerlinNoise(unsigned int seed = 0) {
        p.resize(256);
        for (int i = 0; i < 256; i++) {
            p[i] = i;
        }
        
        std::default_random_engine engine(seed);
        std::shuffle(p.begin(), p.end(), engine);
        p.insert(p.end(), p.begin(), p.end());
    }

    double noise(double x, double y, double z) {
        int X = (int)floor(x) & 255;
        int Y = (int)floor(y) & 255;
        int Z = (int)floor(z) & 255;

        x -= floor(x);
        y -= floor(y);
        z -= floor(z);

        double u = fade(x);
        double v = fade(y);
        double w = fade(z);

        int A = p[X] + Y;
        int AA = p[A] + Z;
        int AB = p[A + 1] + Z;
        int B = p[X + 1] + Y;
        int BA = p[B] + Z;
        int BB = p[B + 1] + Z;

        return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
                                       grad(p[BA], x - 1, y, z)),
                               lerp(u, grad(p[AB], x, y - 1, z),
                                       grad(p[BB], x - 1, y - 1, z))),
                       lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
                                       grad(p[BA + 1], x - 1, y, z - 1)),
                               lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                                       grad(p[BB + 1], x - 1, y - 1, z - 1))));
    }

private:
    double fade(double t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    double lerp(double t, double a, double b) {
        return a + t * (b - a);
    }

    double grad(int hash, double x, double y, double z) {
        int h = hash & 15;
        double u = h < 8 ? x : y;
        double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
};

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Terrain {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;
    int width, height;
    float scale;
    PerlinNoise perlin;

    Terrain(int w, int h, float s, unsigned int seed = 0) 
        : width(w), height(h), scale(s), perlin(seed) {
        generateTerrain();
        setupMesh();
    }

    void Draw(Shader& shader) {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    float getHeight(float x, float z) {
        // Multi-octave Perlin noise for more interesting terrain
        float height = 0.0f;
        float amplitude = 1.0f;
        float frequency = 1.0f;
        
        for (int i = 0; i < 4; i++) {
            height += amplitude * (float)perlin.noise(x * frequency * 0.01, 0, z * frequency * 0.01);
            amplitude *= 0.5f;
            frequency *= 2.0f;
        }
        
        return height * scale;
    }

    bool isCave(float x, float y, float z) {
        // 3D Perlin noise for cave generation
        double noise1 = perlin.noise(x * 0.05, y * 0.05, z * 0.05);
        double noise2 = perlin.noise(x * 0.03 + 100, y * 0.03 + 100, z * 0.03 + 100);
        
        // Caves exist where both noise values are within a threshold
        return (noise1 > -0.2 && noise1 < 0.2) && (noise2 > -0.15 && noise2 < 0.15);
    }

    ~Terrain() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

private:
    void generateTerrain() {
        vertices.clear();
        indices.clear();

        // Generate vertices
        for (int z = 0; z <= height; z++) {
            for (int x = 0; x <= width; x++) {
                Vertex vertex;
                vertex.Position.x = (float)x;
                vertex.Position.z = (float)z;
                vertex.Position.y = getHeight((float)x, (float)z);
                
                // Calculate normal (simplified)
                glm::vec3 normal(0.0f, 1.0f, 0.0f);
                if (x > 0 && x < width && z > 0 && z < height) {
                    float hL = getHeight((float)(x - 1), (float)z);
                    float hR = getHeight((float)(x + 1), (float)z);
                    float hD = getHeight((float)x, (float)(z - 1));
                    float hU = getHeight((float)x, (float)(z + 1));
                    normal = glm::normalize(glm::vec3(hL - hR, 2.0f, hD - hU));
                }
                vertex.Normal = normal;
                
                vertex.TexCoords = glm::vec2((float)x / width, (float)z / height);
                vertices.push_back(vertex);
            }
        }

        // Generate indices
        for (int z = 0; z < height; z++) {
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
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // Normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        // TexCoords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }
};

#endif
