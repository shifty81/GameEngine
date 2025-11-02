#ifndef TEXTURE_GENERATOR_H
#define TEXTURE_GENERATOR_H

#include <glad/glad.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class TextureGenerator {
public:
    static unsigned int generateGrassTexture(int width = 256, int height = 256) {
        std::vector<unsigned char> data(width * height * 3);
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int index = (y * width + x) * 3;
                // Green with some variation
                data[index + 0] = 34 + (rand() % 20);  // R
                data[index + 1] = 139 + (rand() % 30); // G
                data[index + 2] = 34 + (rand() % 20);  // B
            }
        }
        
        return createTexture(data.data(), width, height);
    }

    static unsigned int generateStoneTexture(int width = 256, int height = 256) {
        std::vector<unsigned char> data(width * height * 3);
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int index = (y * width + x) * 3;
                // Gray with variation
                unsigned char gray = 100 + (rand() % 50);
                data[index + 0] = gray;
                data[index + 1] = gray;
                data[index + 2] = gray;
            }
        }
        
        return createTexture(data.data(), width, height);
    }

    static unsigned int generateSandTexture(int width = 256, int height = 256) {
        std::vector<unsigned char> data(width * height * 3);
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int index = (y * width + x) * 3;
                // Sandy color
                data[index + 0] = 194 + (rand() % 20); // R
                data[index + 1] = 178 + (rand() % 20); // G
                data[index + 2] = 128 + (rand() % 20); // B
            }
        }
        
        return createTexture(data.data(), width, height);
    }

    static unsigned int generateWaterTexture(int width = 256, int height = 256) {
        std::vector<unsigned char> data(width * height * 3);
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int index = (y * width + x) * 3;
                // Blue water
                data[index + 0] = 30 + (rand() % 20);  // R
                data[index + 1] = 144 + (rand() % 30); // G
                data[index + 2] = 255;                 // B
            }
        }
        
        return createTexture(data.data(), width, height);
    }

    static void saveTextureToFile(const std::string& filename, unsigned char* data, int width, int height) {
        // Save as PPM format (simple image format)
        std::ofstream file(filename, std::ios::binary);
        if (file.is_open()) {
            file << "P6\n" << width << " " << height << "\n255\n";
            file.write(reinterpret_cast<char*>(data), width * height * 3);
            file.close();
            std::cout << "Texture saved to: " << filename << std::endl;
        } else {
            std::cout << "Failed to save texture: " << filename << std::endl;
        }
    }

    static void exportGeneratedTextures() {
        int size = 256;
        std::vector<unsigned char> data(size * size * 3);
        
        // Generate and save grass texture
        for (int i = 0; i < size * size * 3; i += 3) {
            data[i + 0] = 34 + (rand() % 20);
            data[i + 1] = 139 + (rand() % 30);
            data[i + 2] = 34 + (rand() % 20);
        }
        saveTextureToFile("textures/grass.ppm", data.data(), size, size);
        
        // Generate and save stone texture
        for (int i = 0; i < size * size * 3; i += 3) {
            unsigned char gray = 100 + (rand() % 50);
            data[i + 0] = gray;
            data[i + 1] = gray;
            data[i + 2] = gray;
        }
        saveTextureToFile("textures/stone.ppm", data.data(), size, size);
        
        // Generate and save sand texture
        for (int i = 0; i < size * size * 3; i += 3) {
            data[i + 0] = 194 + (rand() % 20);
            data[i + 1] = 178 + (rand() % 20);
            data[i + 2] = 128 + (rand() % 20);
        }
        saveTextureToFile("textures/sand.ppm", data.data(), size, size);
        
        // Generate and save water texture
        for (int i = 0; i < size * size * 3; i += 3) {
            data[i + 0] = 30 + (rand() % 20);
            data[i + 1] = 144 + (rand() % 30);
            data[i + 2] = 255;
        }
        saveTextureToFile("textures/water.ppm", data.data(), size, size);
    }

private:
    static unsigned int createTexture(unsigned char* data, int width, int height) {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        
        return textureID;
    }
};

#endif
