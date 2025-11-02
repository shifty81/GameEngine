#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <ctime>

#include "Shader.h"
#include "Camera.h"
#include "Terrain.h"
#include "Water.h"
#include "Wind.h"
#include "TextureGenerator.h"
#include "DebugConsole.h"
#include "ScriptCompiler.h"

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

// Debug console state
bool consoleActive = false;
std::string consoleInput = "";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void character_callback(GLFWwindow* window, unsigned int codepoint);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main() {
    // Set GLFW error callback before initialization
    glfwSetErrorCallback(glfw_error_callback);
    
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        std::cerr << "\nTroubleshooting:" << std::endl;
        std::cerr << "1. Ensure you have a display available (check $DISPLAY environment variable)" << std::endl;
        std::cerr << "2. Install required X11 libraries: sudo apt-get install libx11-dev libxrandr-dev" << std::endl;
        std::cerr << "3. If running headless, use Xvfb: Xvfb :99 -screen 0 1024x768x24 &" << std::endl;
        std::cerr << "   Then export DISPLAY=:99 before running" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Game Engine - Procedural World", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        std::cerr << "\nPossible causes:" << std::endl;
        std::cerr << "1. OpenGL 3.3 not supported by your GPU/driver" << std::endl;
        std::cerr << "2. No display server running (X11/Wayland)" << std::endl;
        std::cerr << "3. Insufficient graphics driver installation" << std::endl;
        std::cerr << "\nTroubleshooting steps:" << std::endl;
        std::cerr << "1. Check OpenGL support: glxinfo | grep \"OpenGL version\"" << std::endl;
        std::cerr << "2. Update GPU drivers: sudo ubuntu-drivers autoinstall (for NVIDIA)" << std::endl;
        std::cerr << "3. For headless systems, use Xvfb (virtual framebuffer)" << std::endl;
        std::cerr << "4. See UBUNTU_24_04.md for detailed setup instructions" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCharCallback(window, character_callback);
    glfwSetKeyCallback(window, key_callback);

    // Capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        std::cerr << "This indicates OpenGL function loading failed." << std::endl;
        std::cerr << "Ensure your GPU drivers are properly installed." << std::endl;
        glfwTerminate();
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

    // Initialize Script Compiler System
    auto& scriptManager = ScriptCompilerManager::GetInstance();
    scriptManager.Initialize();
    auto& scriptCompiler = scriptManager.GetCompiler();
    
    // Register engine functions for scripts
    scriptCompiler.RegisterFunction("wireframe", [](const std::vector<std::string>& args) {
        wireframeMode = !wireframeMode;
        glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
        std::cout << "[Script] Wireframe mode: " << (wireframeMode ? "ON" : "OFF") << std::endl;
    });
    
    scriptCompiler.RegisterFunction("cellshading", [](const std::vector<std::string>& args) {
        if (args.empty()) {
            cellShadingEnabled = !cellShadingEnabled;
        } else {
            cellShadingEnabled = (args[0] == "on" || args[0] == "1" || args[0] == "true");
        }
        std::cout << "[Script] Cell shading: " << (cellShadingEnabled ? "ON" : "OFF") << std::endl;
    });
    
    scriptCompiler.RegisterFunction("setcam", [](const std::vector<std::string>& args) {
        if (args.size() >= 3) {
            try {
                float x = std::stof(args[0]);
                float y = std::stof(args[1]);
                float z = std::stof(args[2]);
                camera.Position = glm::vec3(x, y, z);
                std::cout << "[Script] Camera moved to (" << x << ", " << y << ", " << z << ")" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "[Script Error] Invalid number format" << std::endl;
            }
        } else {
            std::cerr << "[Script Error] Usage: setcam <x> <y> <z>" << std::endl;
        }
    });
    
    scriptCompiler.RegisterFunction("getcam", [](const std::vector<std::string>& args) {
        std::cout << "[Script] Camera position: (" 
                  << camera.Position.x << ", "
                  << camera.Position.y << ", "
                  << camera.Position.z << ")" << std::endl;
    });
    
    // Load startup scripts if they exist
    std::ifstream startupCheck("scripts/startup.script");
    if (startupCheck.good()) {
        startupCheck.close();
        std::cout << "[ScriptCompiler] Loading startup script..." << std::endl;
        if (!scriptManager.LoadScript("scripts/startup.script")) {
            std::cerr << "[ScriptCompiler] Warning: Failed to load startup script" << std::endl;
        }
    }
    
    // Initialize Debug Console
    auto debugConsole = DebugConsole::GetInstance();
    
    // Register custom game-specific console commands
    debugConsole->RegisterCommand("wireframe", "Toggle wireframe mode",
        [](const std::vector<std::string>& args) {
            wireframeMode = !wireframeMode;
            glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
            DebugConsole::GetInstance()->Log("[Console] Wireframe mode: " + 
                std::string(wireframeMode ? "ON" : "OFF"));
        });
    
    debugConsole->RegisterCommand("cellshading", "Toggle or set cell shading (usage: cellshading [on|off])",
        [](const std::vector<std::string>& args) {
            if (args.empty()) {
                cellShadingEnabled = !cellShadingEnabled;
            } else {
                cellShadingEnabled = (args[0] == "on" || args[0] == "1" || args[0] == "true");
            }
            DebugConsole::GetInstance()->Log("[Console] Cell shading: " + 
                std::string(cellShadingEnabled ? "ON" : "OFF"));
        });
    
    debugConsole->RegisterCommand("setcam", "Set camera position (usage: setcam <x> <y> <z>)",
        [](const std::vector<std::string>& args) {
            if (args.size() >= 3) {
                try {
                    float x = std::stof(args[0]);
                    float y = std::stof(args[1]);
                    float z = std::stof(args[2]);
                    camera.Position = glm::vec3(x, y, z);
                    DebugConsole::GetInstance()->Log("[Console] Camera moved to (" + 
                        args[0] + ", " + args[1] + ", " + args[2] + ")");
                } catch (const std::exception& e) {
                    DebugConsole::GetInstance()->Log("[Error] Invalid number format. Usage: setcam <x> <y> <z>");
                }
            } else {
                DebugConsole::GetInstance()->Log("[Error] Usage: setcam <x> <y> <z>");
            }
        });
    
    debugConsole->RegisterCommand("getcam", "Get current camera position",
        [](const std::vector<std::string>& args) {
            auto console = DebugConsole::GetInstance();
            console->Log("[Console] Camera position: (" + 
                std::to_string(camera.Position.x) + ", " +
                std::to_string(camera.Position.y) + ", " +
                std::to_string(camera.Position.z) + ")");
        });
    
    // Register script execution command in debug console
    debugConsole->RegisterCommand("script", "Execute a script command (usage: script <command>)",
        [](const std::vector<std::string>& args) {
            if (args.empty()) {
                DebugConsole::GetInstance()->Log("[Error] Usage: script <command>");
                return;
            }
            
            // Reconstruct the command from args
            std::string command = args[0];
            for (size_t i = 1; i < args.size(); i++) {
                command += " " + args[i];
            }
            
            auto& scriptManager = ScriptCompilerManager::GetInstance();
            if (scriptManager.ExecuteCommand(command)) {
                DebugConsole::GetInstance()->Log("[Script] Command executed: " + command);
            } else {
                DebugConsole::GetInstance()->Log("[Script Error] Failed to execute: " + command);
            }
        });
    
    debugConsole->RegisterCommand("loadscript", "Load and execute a script file (usage: loadscript <filename>)",
        [](const std::vector<std::string>& args) {
            if (args.empty()) {
                DebugConsole::GetInstance()->Log("[Error] Usage: loadscript <filename>");
                return;
            }
            
            auto& scriptManager = ScriptCompilerManager::GetInstance();
            if (scriptManager.LoadScript(args[0])) {
                DebugConsole::GetInstance()->Log("[Script] Loaded and executed: " + args[0]);
            } else {
                DebugConsole::GetInstance()->Log("[Script Error] Failed to load: " + args[0]);
            }
        });

    std::cout << "\n=== 3D Game Engine Started ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD - Move camera" << std::endl;
    std::cout << "  Space/Shift - Move up/down" << std::endl;
    std::cout << "  Mouse - Look around" << std::endl;
    std::cout << "  F - Toggle wireframe mode" << std::endl;
    std::cout << "  C - Toggle cell shading" << std::endl;
    std::cout << "  ` (Grave/Tilde) - Toggle debug console" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << "\nFeatures:" << std::endl;
    std::cout << "  ✓ Procedural terrain generation" << std::endl;
    std::cout << "  ✓ Cell shading (toon rendering)" << std::endl;
    std::cout << "  ✓ Cave systems (underground caverns)" << std::endl;
    std::cout << "  ✓ Flowing water system" << std::endl;
    std::cout << "  ✓ Wind system (for windmills)" << std::endl;
    std::cout << "  ✓ Procedural texture generation" << std::endl;
    std::cout << "  ✓ Debug console (press ` to open)" << std::endl;
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

        // Display console output if active
        if (consoleActive) {
            auto console = DebugConsole::GetInstance();
            const auto& log = console->GetOutputLog();
            
            // Print console header
            std::cout << "\n========== Debug Console ==========\n";
            
            // Print recent log entries (last 10)
            size_t startIdx = log.size() > 10 ? log.size() - 10 : 0;
            for (size_t i = startIdx; i < log.size(); i++) {
                std::cout << log[i] << std::endl;
            }
            
            // Print current input
            std::cout << "\n> " << consoleInput << "_\n";
            std::cout << "===================================\n" << std::endl;
        }

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

    // Don't process camera/game input when console is active
    if (consoleActive) {
        return;
    }

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
    // Don't process mouse movement when console is active
    if (consoleActive) {
        return;
    }
    
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

void character_callback(GLFWwindow* window, unsigned int codepoint) {
    if (consoleActive) {
        // Add character to console input
        if (codepoint >= 32 && codepoint <= 126) {  // Printable ASCII
            consoleInput += static_cast<char>(codepoint);
        }
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        // Toggle console with grave accent/tilde key
        if (key == GLFW_KEY_GRAVE_ACCENT) {
            consoleActive = !consoleActive;
            auto console = DebugConsole::GetInstance();
            
            if (consoleActive) {
                console->SetEnabled(true);
                console->Log("\n=== Debug Console ===");
                console->Log("Type 'help' for available commands");
                console->Log("Press ` again to close console\n");
                // Release mouse cursor when console is active
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                console->SetEnabled(false);
                consoleInput.clear();
                // Capture mouse cursor when console is closed
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                firstMouse = true;  // Reset mouse to prevent camera jump
            }
        }
        
        if (consoleActive) {
            auto console = DebugConsole::GetInstance();
            
            // Handle Enter key - execute command
            if (key == GLFW_KEY_ENTER) {
                if (!consoleInput.empty()) {
                    console->Log("> " + consoleInput);
                    console->ExecuteCommand(consoleInput);
                    consoleInput.clear();
                }
            }
            // Handle Backspace
            else if (key == GLFW_KEY_BACKSPACE) {
                if (!consoleInput.empty()) {
                    consoleInput.pop_back();
                }
            }
            // Handle Up arrow - previous command
            else if (key == GLFW_KEY_UP) {
                consoleInput = console->GetPreviousCommand();
            }
            // Handle Down arrow - next command
            else if (key == GLFW_KEY_DOWN) {
                consoleInput = console->GetNextCommand();
            }
        }
    }
}
