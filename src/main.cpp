#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ctime>

#include "Shader.h"
#include "Camera.h"
#include "Terrain.h"
#include "Water.h"
#include "Wind.h"
#include "TextureGenerator.h"

// Settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// Camera
Camera camera(glm::vec3(50.0f, 30.0f, 50.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Input states
bool wireframeMode = false;
bool cellShadingEnabled = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

int main() {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Game Engine - Procedural World", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Cell shading vertex shader
    const char* cellVertexShader = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec2 aTexCoords;

        out vec3 FragPos;
        out vec3 Normal;
        out vec2 TexCoords;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            FragPos = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;
            TexCoords = aTexCoords;
            gl_Position = projection * view * vec4(FragPos, 1.0);
        }
    )";

    // Cell shading fragment shader with toon effect
    const char* cellFragmentShader = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 FragPos;
        in vec3 Normal;
        in vec2 TexCoords;

        uniform vec3 lightPos;
        uniform vec3 viewPos;
        uniform vec3 objectColor;
        uniform bool cellShading;

        void main() {
            // Ambient
            float ambientStrength = 0.3;
            vec3 ambient = ambientStrength * vec3(1.0);

            // Diffuse
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);

            // Cell shading quantization
            if (cellShading) {
                // Quantize lighting into discrete steps for toon effect
                if (diff > 0.95) diff = 1.0;
                else if (diff > 0.5) diff = 0.7;
                else if (diff > 0.25) diff = 0.4;
                else diff = 0.2;
            }

            vec3 diffuse = diff * vec3(1.0);

            // Specular with cell shading
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
            
            if (cellShading) {
                // Binary specular for cell shading
                spec = spec > 0.5 ? 1.0 : 0.0;
            }
            
            vec3 specular = spec * vec3(1.0);

            // Edge detection for outline effect
            float edge = 1.0;
            if (cellShading) {
                float edgeDot = dot(viewDir, norm);
                if (edgeDot < 0.3) {
                    edge = 0.0; // Black outline
                }
            }

            vec3 result = (ambient + diffuse + specular) * objectColor * edge;
            FragColor = vec4(result, 1.0);
        }
    )";

    // Water shader with animation
    const char* waterFragmentShader = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 FragPos;
        in vec3 Normal;
        in vec2 TexCoords;

        uniform float time;
        uniform vec3 viewPos;
        uniform bool cellShading;

        void main() {
            // Animated water color
            vec3 waterColor = vec3(0.1, 0.5, 0.8);
            
            // Simple wave effect
            float wave = sin(TexCoords.x * 10.0 + time) * cos(TexCoords.y * 10.0 + time) * 0.1;
            
            // Lighting
            vec3 lightDir = normalize(vec3(1.0, 1.0, 0.5));
            vec3 norm = normalize(Normal + vec3(0.0, wave, 0.0));
            float diff = max(dot(norm, lightDir), 0.0);
            
            if (cellShading) {
                if (diff > 0.8) diff = 1.0;
                else if (diff > 0.5) diff = 0.7;
                else diff = 0.4;
            }
            
            vec3 result = waterColor * (0.5 + diff * 0.5);
            FragColor = vec4(result, 0.7); // Semi-transparent
        }
    )";

    // Create shaders
    Shader terrainShader(cellVertexShader, cellFragmentShader, false);
    Shader waterShader(cellVertexShader, waterFragmentShader, false);

    // Seed random with time
    srand((unsigned int)time(0));

    // Create terrain with procedural generation
    std::cout << "Generating procedural terrain..." << std::endl;
    Terrain terrain(100, 100, 15.0f, 12345);
    
    // Create water system
    std::cout << "Creating water system..." << std::endl;
    Water water(5.0f, 100, 100);
    
    // Create wind system
    std::cout << "Initializing wind system..." << std::endl;
    Wind wind(glm::vec3(1.0f, 0.0f, 0.5f), 2.0f);

    // Generate procedural textures and export them
    std::cout << "Generating procedural textures..." << std::endl;
    // Create textures directory if it doesn't exist
    system("mkdir -p textures");
    TextureGenerator::exportGeneratedTextures();

    // Generate textures for use
    unsigned int grassTexture = TextureGenerator::generateGrassTexture();
    unsigned int waterTexture = TextureGenerator::generateWaterTexture();

    std::cout << "\n=== 3D Game Engine Started ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD - Move camera" << std::endl;
    std::cout << "  Space/Shift - Move up/down" << std::endl;
    std::cout << "  Mouse - Look around" << std::endl;
    std::cout << "  F - Toggle wireframe mode" << std::endl;
    std::cout << "  C - Toggle cell shading" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << "\nFeatures:" << std::endl;
    std::cout << "  ✓ Procedural terrain generation" << std::endl;
    std::cout << "  ✓ Cell shading (toon rendering)" << std::endl;
    std::cout << "  ✓ Cave systems (underground caverns)" << std::endl;
    std::cout << "  ✓ Flowing water system" << std::endl;
    std::cout << "  ✓ Wind system (for windmills)" << std::endl;
    std::cout << "  ✓ Procedural texture generation" << std::endl;
    std::cout << "==============================\n" << std::endl;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);

        // Update systems
        water.Update(deltaTime);
        wind.Update(deltaTime);

        // Render
        glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Sky blue
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // View/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 
                                               (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        // Render terrain with cell shading
        terrainShader.use();
        terrainShader.setMat4("projection", projection);
        terrainShader.setMat4("view", view);
        terrainShader.setMat4("model", model);
        terrainShader.setVec3("lightPos", glm::vec3(50.0f, 100.0f, 50.0f));
        terrainShader.setVec3("viewPos", camera.Position);
        terrainShader.setVec3("objectColor", glm::vec3(0.3f, 0.7f, 0.3f)); // Green
        terrainShader.setBool("cellShading", cellShadingEnabled);

        terrain.Draw(terrainShader);

        // Render water with transparency
        waterShader.use();
        waterShader.setMat4("projection", projection);
        waterShader.setMat4("view", view);
        waterShader.setMat4("model", model);
        waterShader.setVec3("viewPos", camera.Position);
        waterShader.setBool("cellShading", cellShadingEnabled);
        
        water.Draw(waterShader);

        // Display wind info (for debugging)
        if ((int)currentFrame % 5 == 0) {
            glm::vec3 windForce = wind.getForce();
            // Wind info could be displayed in UI
        }

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    // Toggle wireframe
    static bool fKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !fKeyPressed) {
        wireframeMode = !wireframeMode;
        glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
        fKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) {
        fKeyPressed = false;
    }

    // Toggle cell shading
    static bool cKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !cKeyPressed) {
        cellShadingEnabled = !cellShadingEnabled;
        std::cout << "Cell shading: " << (cellShadingEnabled ? "ON" : "OFF") << std::endl;
        cKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) {
        cKeyPressed = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}
