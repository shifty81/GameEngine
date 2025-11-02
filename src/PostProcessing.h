#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "Shader.h"

// Framebuffer for post-processing
class Framebuffer {
public:
    Framebuffer(int width, int height, bool multisampled = false, int samples = 4)
        : m_Width(width), m_Height(height), m_Multisampled(multisampled), m_Samples(samples) {
        CreateFramebuffer();
    }

    ~Framebuffer() {
        if (m_FBO != 0) glDeleteFramebuffers(1, &m_FBO);
        if (m_ColorTexture != 0) glDeleteTextures(1, &m_ColorTexture);
        if (m_DepthTexture != 0) glDeleteTextures(1, &m_DepthTexture);
        if (m_RBO != 0) glDeleteRenderbuffers(1, &m_RBO);
    }

    void Bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glViewport(0, 0, m_Width, m_Height);
    }

    void Unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Resize(int width, int height) {
        m_Width = width;
        m_Height = height;
        
        // Delete old resources
        if (m_FBO != 0) glDeleteFramebuffers(1, &m_FBO);
        if (m_ColorTexture != 0) glDeleteTextures(1, &m_ColorTexture);
        if (m_DepthTexture != 0) glDeleteTextures(1, &m_DepthTexture);
        if (m_RBO != 0) glDeleteRenderbuffers(1, &m_RBO);
        
        // Recreate
        CreateFramebuffer();
    }

    unsigned int GetColorTexture() const { return m_ColorTexture; }
    unsigned int GetDepthTexture() const { return m_DepthTexture; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }

private:
    void CreateFramebuffer() {
        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        if (m_Multisampled) {
            // Multisampled color texture
            glGenTextures(1, &m_ColorTexture);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ColorTexture);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Samples, GL_RGB16F, m_Width, m_Height, GL_TRUE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_ColorTexture, 0);

            // Multisampled depth renderbuffer
            glGenRenderbuffers(1, &m_RBO);
            glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_Samples, GL_DEPTH24_STENCIL8, m_Width, m_Height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
        } else {
            // Regular color texture (HDR format)
            glGenTextures(1, &m_ColorTexture);
            glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture, 0);

            // Depth texture
            glGenTextures(1, &m_DepthTexture);
            glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Width, m_Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "ERROR: Framebuffer is not complete!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    unsigned int m_FBO = 0;
    unsigned int m_ColorTexture = 0;
    unsigned int m_DepthTexture = 0;
    unsigned int m_RBO = 0;
    int m_Width, m_Height;
    bool m_Multisampled;
    int m_Samples;
};

// Post-Processing Effect base class
class PostProcessEffect {
public:
    virtual ~PostProcessEffect() = default;
    virtual void Apply(unsigned int inputTexture, Framebuffer* outputFB, Shader& shader) = 0;
    virtual bool IsEnabled() const { return m_Enabled; }
    virtual void SetEnabled(bool enabled) { m_Enabled = enabled; }

protected:
    bool m_Enabled = true;
};

// Bloom Effect
class BloomEffect : public PostProcessEffect {
public:
    BloomEffect(int width, int height, float threshold = 1.0f, float intensity = 0.5f)
        : m_Threshold(threshold), m_Intensity(intensity) {
        // Create downsampled framebuffers for bloom
        for (int i = 0; i < 5; i++) {
            int w = width / (2 << i);
            int h = height / (2 << i);
            m_BloomBuffers.push_back(std::make_unique<Framebuffer>(w, h));
        }
    }

    void Apply(unsigned int inputTexture, Framebuffer* outputFB, Shader& shader) override {
        // Extract bright areas
        // Blur
        // Combine with original
        // (Simplified implementation - full bloom requires multiple passes)
    }

    void SetThreshold(float threshold) { m_Threshold = threshold; }
    void SetIntensity(float intensity) { m_Intensity = intensity; }

private:
    std::vector<std::unique_ptr<Framebuffer>> m_BloomBuffers;
    float m_Threshold;
    float m_Intensity;
};

// FXAA (Fast Approximate Anti-Aliasing)
class FXAAEffect : public PostProcessEffect {
public:
    void Apply(unsigned int inputTexture, Framebuffer* outputFB, Shader& shader) override {
        shader.use();
        shader.setInt("screenTexture", 0);
        shader.setVec2("texelSize", glm::vec2(1.0f / outputFB->GetWidth(), 1.0f / outputFB->GetHeight()));
        
        outputFB->Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inputTexture);
        RenderQuad();
        outputFB->Unbind();
    }

private:
    void RenderQuad() {
        static unsigned int quadVAO = 0;
        static unsigned int quadVBO;
        
        if (quadVAO == 0) {
            float quadVertices[] = {
                -1.0f,  1.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f,
                 1.0f, -1.0f, 1.0f, 0.0f,
                -1.0f,  1.0f, 0.0f, 1.0f,
                 1.0f, -1.0f, 1.0f, 0.0f,
                 1.0f,  1.0f, 1.0f, 1.0f
            };
            
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        }
        
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
};

// Tone Mapping / Color Correction
class ToneMappingEffect : public PostProcessEffect {
public:
    enum class Mode {
        REINHARD,
        EXPOSURE,
        ACES
    };

    ToneMappingEffect(Mode mode = Mode::REINHARD, float exposure = 1.0f)
        : m_Mode(mode), m_Exposure(exposure) {}

    void Apply(unsigned int inputTexture, Framebuffer* outputFB, Shader& shader) override {
        shader.use();
        shader.setInt("screenTexture", 0);
        shader.setInt("toneMapMode", static_cast<int>(m_Mode));
        shader.setFloat("exposure", m_Exposure);
        
        outputFB->Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inputTexture);
        RenderQuad();
        outputFB->Unbind();
    }

    void SetMode(Mode mode) { m_Mode = mode; }
    void SetExposure(float exposure) { m_Exposure = exposure; }

private:
    void RenderQuad() {
        static unsigned int quadVAO = 0;
        static unsigned int quadVBO;
        
        if (quadVAO == 0) {
            float quadVertices[] = {
                -1.0f,  1.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f,
                 1.0f, -1.0f, 1.0f, 0.0f,
                -1.0f,  1.0f, 0.0f, 1.0f,
                 1.0f, -1.0f, 1.0f, 0.0f,
                 1.0f,  1.0f, 1.0f, 1.0f
            };
            
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        }
        
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    Mode m_Mode;
    float m_Exposure;
};

// Post-Processing Manager
class PostProcessingManager {
public:
    static PostProcessingManager* GetInstance() {
        static PostProcessingManager instance;
        return &instance;
    }

    void Initialize(int width, int height) {
        m_ScreenWidth = width;
        m_ScreenHeight = height;
        m_SceneFB = std::make_unique<Framebuffer>(width, height);
        m_TempFB = std::make_unique<Framebuffer>(width, height);
    }

    void Resize(int width, int height) {
        m_ScreenWidth = width;
        m_ScreenHeight = height;
        if (m_SceneFB) m_SceneFB->Resize(width, height);
        if (m_TempFB) m_TempFB->Resize(width, height);
    }

    void BeginScene() {
        if (m_SceneFB) {
            m_SceneFB->Bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    }

    void EndScene() {
        if (m_SceneFB) {
            m_SceneFB->Unbind();
        }
    }

    void ApplyEffects(Shader& shader) {
        // Apply each enabled effect in sequence
        unsigned int currentTexture = m_SceneFB->GetColorTexture();
        
        for (auto& effect : m_Effects) {
            if (effect->IsEnabled()) {
                effect->Apply(currentTexture, m_TempFB.get(), shader);
                currentTexture = m_TempFB->GetColorTexture();
                std::swap(m_SceneFB, m_TempFB);
            }
        }
    }

    void AddEffect(std::shared_ptr<PostProcessEffect> effect) {
        m_Effects.push_back(effect);
    }

    void ClearEffects() {
        m_Effects.clear();
    }

    Framebuffer* GetSceneFramebuffer() { return m_SceneFB.get(); }

private:
    PostProcessingManager() {}
    ~PostProcessingManager() {}

    PostProcessingManager(const PostProcessingManager&) = delete;
    PostProcessingManager& operator=(const PostProcessingManager&) = delete;

    std::unique_ptr<Framebuffer> m_SceneFB;
    std::unique_ptr<Framebuffer> m_TempFB;
    std::vector<std::shared_ptr<PostProcessEffect>> m_Effects;
    int m_ScreenWidth = 0;
    int m_ScreenHeight = 0;
};

#endif // POSTPROCESSING_H
