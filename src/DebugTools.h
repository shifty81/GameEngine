#ifndef DEBUGTOOLS_H
#define DEBUGTOOLS_H

#include <glad/glad.h>
#include <string>
#include <chrono>
#include <map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

// Performance Timer
class Timer {
public:
    Timer() { Reset(); }

    void Reset() {
        m_StartTime = std::chrono::high_resolution_clock::now();
    }

    float GetElapsedSeconds() const {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<float>(now - m_StartTime).count();
    }

    float GetElapsedMilliseconds() const {
        return GetElapsedSeconds() * 1000.0f;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
};

// Scoped Performance Profiler
class ScopedTimer {
public:
    ScopedTimer(const std::string& name) : m_Name(name) {
        m_Timer.Reset();
    }

    ~ScopedTimer() {
        float elapsed = m_Timer.GetElapsedMilliseconds();
        std::cout << "[Profile] " << m_Name << ": " << elapsed << "ms" << std::endl;
    }

private:
    std::string m_Name;
    Timer m_Timer;
};

// Performance Profiler
class Profiler {
public:
    static Profiler* GetInstance() {
        static Profiler instance;
        return &instance;
    }

    void BeginFrame() {
        m_FrameTimer.Reset();
        m_SectionTimers.clear();
    }

    void EndFrame() {
        m_FrameTime = m_FrameTimer.GetElapsedMilliseconds();
        m_FrameCount++;
        
        // Calculate FPS
        m_FPSTimer += m_FrameTime;
        m_FPSFrameCount++;
        
        if (m_FPSTimer >= 1000.0f) {  // Update FPS every second
            m_FPS = m_FPSFrameCount / (m_FPSTimer / 1000.0f);
            m_FPSTimer = 0.0f;
            m_FPSFrameCount = 0;
        }
    }

    void BeginSection(const std::string& name) {
        if (m_SectionTimers.find(name) == m_SectionTimers.end()) {
            m_SectionTimers[name] = Timer();
        }
        m_SectionTimers[name].Reset();
    }

    void EndSection(const std::string& name) {
        auto it = m_SectionTimers.find(name);
        if (it != m_SectionTimers.end()) {
            float elapsed = it->second.GetElapsedMilliseconds();
            m_SectionTimes[name] = elapsed;
        }
    }

    // Getters
    float GetFrameTime() const { return m_FrameTime; }
    float GetFPS() const { return m_FPS; }
    unsigned long long GetFrameCount() const { return m_FrameCount; }
    
    float GetSectionTime(const std::string& name) const {
        auto it = m_SectionTimes.find(name);
        return it != m_SectionTimes.end() ? it->second : 0.0f;
    }

    // Print statistics
    void PrintStats() const {
        std::cout << "\n=== Performance Statistics ===" << std::endl;
        std::cout << "FPS: " << std::fixed << std::setprecision(1) << m_FPS << std::endl;
        std::cout << "Frame Time: " << std::setprecision(3) << m_FrameTime << "ms" << std::endl;
        std::cout << "Total Frames: " << m_FrameCount << std::endl;
        
        if (!m_SectionTimes.empty()) {
            std::cout << "\nSection Times:" << std::endl;
            for (const auto& pair : m_SectionTimes) {
                std::cout << "  " << pair.first << ": " << pair.second << "ms" << std::endl;
            }
        }
        std::cout << "============================\n" << std::endl;
    }

    std::string GetStatsString() const {
        std::stringstream ss;
        ss << "FPS: " << std::fixed << std::setprecision(1) << m_FPS
           << " | Frame: " << std::setprecision(2) << m_FrameTime << "ms";
        return ss.str();
    }

private:
    Profiler() 
        : m_FrameTime(0.0f), 
          m_FPS(0.0f), 
          m_FrameCount(0),
          m_FPSTimer(0.0f),
          m_FPSFrameCount(0) {}
    
    ~Profiler() {}

    Profiler(const Profiler&) = delete;
    Profiler& operator=(const Profiler&) = delete;

    Timer m_FrameTimer;
    std::map<std::string, Timer> m_SectionTimers;
    std::map<std::string, float> m_SectionTimes;
    
    float m_FrameTime;
    float m_FPS;
    unsigned long long m_FrameCount;
    
    float m_FPSTimer;
    int m_FPSFrameCount;
};

// OpenGL Debug Output
class GLDebug {
public:
    static void EnableDebugOutput() {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(DebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        std::cout << "[GLDebug] OpenGL debug output enabled" << std::endl;
    }

    static void DisableDebugOutput() {
        glDisable(GL_DEBUG_OUTPUT);
        std::cout << "[GLDebug] OpenGL debug output disabled" << std::endl;
    }

private:
    static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, 
                                       GLenum severity, GLsizei length, 
                                       const GLchar* message, const void* userParam) {
        // Ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

        std::cout << "---------------" << std::endl;
        std::cout << "[GLDebug] Message ID: " << id << std::endl;
        std::cout << "Message: " << message << std::endl;

        std::cout << "Source: ";
        switch (source) {
            case GL_DEBUG_SOURCE_API:             std::cout << "API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Application"; break;
            case GL_DEBUG_SOURCE_OTHER:           std::cout << "Other"; break;
        }
        std::cout << std::endl;

        std::cout << "Type: ";
        switch (type) {
            case GL_DEBUG_TYPE_ERROR:               std::cout << "Error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Deprecated Behaviour"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Undefined Behaviour"; break;
            case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Performance"; break;
            case GL_DEBUG_TYPE_MARKER:              std::cout << "Marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Push Group"; break;
            case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Pop Group"; break;
            case GL_DEBUG_TYPE_OTHER:               std::cout << "Other"; break;
        }
        std::cout << std::endl;

        std::cout << "Severity: ";
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:         std::cout << "High"; break;
            case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Medium"; break;
            case GL_DEBUG_SEVERITY_LOW:          std::cout << "Low"; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Notification"; break;
        }
        std::cout << std::endl;
        std::cout << "---------------" << std::endl;
    }
};

// Memory Tracker
class MemoryTracker {
public:
    static MemoryTracker* GetInstance() {
        static MemoryTracker instance;
        return &instance;
    }

    void AllocateTexture(unsigned int id, size_t size, const std::string& name = "") {
        m_TextureMemory[id] = {size, name};
        m_TotalTextureMemory += size;
    }

    void DeallocateTexture(unsigned int id) {
        auto it = m_TextureMemory.find(id);
        if (it != m_TextureMemory.end()) {
            m_TotalTextureMemory -= it->second.size;
            m_TextureMemory.erase(it);
        }
    }

    void AllocateBuffer(unsigned int id, size_t size, const std::string& name = "") {
        m_BufferMemory[id] = {size, name};
        m_TotalBufferMemory += size;
    }

    void DeallocateBuffer(unsigned int id) {
        auto it = m_BufferMemory.find(id);
        if (it != m_BufferMemory.end()) {
            m_TotalBufferMemory -= it->second.size;
            m_BufferMemory.erase(it);
        }
    }

    size_t GetTotalTextureMemory() const { return m_TotalTextureMemory; }
    size_t GetTotalBufferMemory() const { return m_TotalBufferMemory; }
    size_t GetTotalGPUMemory() const { return m_TotalTextureMemory + m_TotalBufferMemory; }

    void PrintMemoryStats() const {
        std::cout << "\n=== GPU Memory Statistics ===" << std::endl;
        std::cout << "Textures: " << (m_TotalTextureMemory / 1024.0f / 1024.0f) << " MB (" 
                  << m_TextureMemory.size() << " objects)" << std::endl;
        std::cout << "Buffers: " << (m_TotalBufferMemory / 1024.0f / 1024.0f) << " MB (" 
                  << m_BufferMemory.size() << " objects)" << std::endl;
        std::cout << "Total: " << (GetTotalGPUMemory() / 1024.0f / 1024.0f) << " MB" << std::endl;
        std::cout << "============================\n" << std::endl;
    }

private:
    MemoryTracker() : m_TotalTextureMemory(0), m_TotalBufferMemory(0) {}
    ~MemoryTracker() {}

    MemoryTracker(const MemoryTracker&) = delete;
    MemoryTracker& operator=(const MemoryTracker&) = delete;

    struct MemoryInfo {
        size_t size;
        std::string name;
    };

    std::map<unsigned int, MemoryInfo> m_TextureMemory;
    std::map<unsigned int, MemoryInfo> m_BufferMemory;
    size_t m_TotalTextureMemory;
    size_t m_TotalBufferMemory;
};

// Debug Renderer - Draw debug shapes
class DebugRenderer {
public:
    static DebugRenderer* GetInstance() {
        static DebugRenderer instance;
        return &instance;
    }

    // Draw debug line
    void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color = glm::vec3(1.0f)) {
        m_Lines.push_back({start, end, color});
    }

    // Draw debug box
    void DrawBox(const glm::vec3& center, const glm::vec3& size, const glm::vec3& color = glm::vec3(1.0f)) {
        glm::vec3 min = center - size * 0.5f;
        glm::vec3 max = center + size * 0.5f;

        // Bottom face
        DrawLine(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z), color);
        DrawLine(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z), color);
        DrawLine(glm::vec3(max.x, min.y, max.z), glm::vec3(min.x, min.y, max.z), color);
        DrawLine(glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, min.y, min.z), color);

        // Top face
        DrawLine(glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z), color);
        DrawLine(glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z), color);
        DrawLine(glm::vec3(max.x, max.y, max.z), glm::vec3(min.x, max.y, max.z), color);
        DrawLine(glm::vec3(min.x, max.y, max.z), glm::vec3(min.x, max.y, min.z), color);

        // Vertical lines
        DrawLine(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, max.y, min.z), color);
        DrawLine(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, max.y, min.z), color);
        DrawLine(glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, max.y, max.z), color);
        DrawLine(glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, max.y, max.z), color);
    }

    // Draw debug sphere (simplified as wireframe)
    void DrawSphere(const glm::vec3& center, float radius, const glm::vec3& color = glm::vec3(1.0f)) {
        const int segments = 16;
        const float angleStep = 2.0f * 3.14159f / segments;

        // XY circle
        for (int i = 0; i < segments; i++) {
            float angle1 = i * angleStep;
            float angle2 = (i + 1) * angleStep;
            glm::vec3 p1 = center + glm::vec3(cos(angle1) * radius, sin(angle1) * radius, 0.0f);
            glm::vec3 p2 = center + glm::vec3(cos(angle2) * radius, sin(angle2) * radius, 0.0f);
            DrawLine(p1, p2, color);
        }

        // XZ circle
        for (int i = 0; i < segments; i++) {
            float angle1 = i * angleStep;
            float angle2 = (i + 1) * angleStep;
            glm::vec3 p1 = center + glm::vec3(cos(angle1) * radius, 0.0f, sin(angle1) * radius);
            glm::vec3 p2 = center + glm::vec3(cos(angle2) * radius, 0.0f, sin(angle2) * radius);
            DrawLine(p1, p2, color);
        }

        // YZ circle
        for (int i = 0; i < segments; i++) {
            float angle1 = i * angleStep;
            float angle2 = (i + 1) * angleStep;
            glm::vec3 p1 = center + glm::vec3(0.0f, cos(angle1) * radius, sin(angle1) * radius);
            glm::vec3 p2 = center + glm::vec3(0.0f, cos(angle2) * radius, sin(angle2) * radius);
            DrawLine(p1, p2, color);
        }
    }

    // Clear debug shapes
    void Clear() {
        m_Lines.clear();
    }

    // Get lines for rendering
    const std::vector<DebugLine>& GetLines() const {
        return m_Lines;
    }

private:
    DebugRenderer() {}
    ~DebugRenderer() {}

    DebugRenderer(const DebugRenderer&) = delete;
    DebugRenderer& operator=(const DebugRenderer&) = delete;

    struct DebugLine {
        glm::vec3 start;
        glm::vec3 end;
        glm::vec3 color;
    };

    std::vector<DebugLine> m_Lines;
};

// Profiling macros
#define PROFILE_SCOPE(name) ScopedTimer timer_##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)

#endif // DEBUGTOOLS_H
