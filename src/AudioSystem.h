#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include <string>
#include <map>
#include <memory>
#include <glm/glm.hpp>
#include <iostream>

// Audio Source - represents a sound emitter in 3D space
class AudioSource {
public:
    AudioSource(const std::string& filepath = "")
        : m_Filepath(filepath),
          m_Volume(1.0f),
          m_Pitch(1.0f),
          m_Loop(false),
          m_Playing(false),
          m_Position(0.0f),
          m_MinDistance(1.0f),
          m_MaxDistance(100.0f),
          m_Spatial(true) {}

    // Playback control
    void Play() { 
        m_Playing = true; 
        std::cout << "[Audio] Playing: " << m_Filepath << std::endl;
    }
    
    void Pause() { 
        m_Playing = false; 
        std::cout << "[Audio] Paused: " << m_Filepath << std::endl;
    }
    
    void Stop() { 
        m_Playing = false; 
        std::cout << "[Audio] Stopped: " << m_Filepath << std::endl;
    }

    bool IsPlaying() const { return m_Playing; }

    // Properties
    void SetVolume(float volume) { m_Volume = glm::clamp(volume, 0.0f, 1.0f); }
    float GetVolume() const { return m_Volume; }

    void SetPitch(float pitch) { m_Pitch = glm::clamp(pitch, 0.1f, 3.0f); }
    float GetPitch() const { return m_Pitch; }

    void SetLoop(bool loop) { m_Loop = loop; }
    bool IsLooping() const { return m_Loop; }

    void SetPosition(const glm::vec3& position) { m_Position = position; }
    glm::vec3 GetPosition() const { return m_Position; }

    void SetMinDistance(float distance) { m_MinDistance = distance; }
    float GetMinDistance() const { return m_MinDistance; }

    void SetMaxDistance(float distance) { m_MaxDistance = distance; }
    float GetMaxDistance() const { return m_MaxDistance; }

    void SetSpatial(bool spatial) { m_Spatial = spatial; }
    bool IsSpatial() const { return m_Spatial; }

    std::string GetFilepath() const { return m_Filepath; }

private:
    std::string m_Filepath;
    float m_Volume;
    float m_Pitch;
    bool m_Loop;
    bool m_Playing;
    glm::vec3 m_Position;
    float m_MinDistance;  // Distance at which sound is at full volume
    float m_MaxDistance;  // Distance at which sound becomes inaudible
    bool m_Spatial;       // Whether to apply 3D positioning
};

// Audio Listener - represents the "ears" in the scene (usually attached to camera)
class AudioListener {
public:
    AudioListener() 
        : m_Position(0.0f),
          m_Forward(0.0f, 0.0f, -1.0f),
          m_Up(0.0f, 1.0f, 0.0f) {}

    void SetPosition(const glm::vec3& position) { m_Position = position; }
    glm::vec3 GetPosition() const { return m_Position; }

    void SetOrientation(const glm::vec3& forward, const glm::vec3& up) {
        m_Forward = glm::normalize(forward);
        m_Up = glm::normalize(up);
    }

    glm::vec3 GetForward() const { return m_Forward; }
    glm::vec3 GetUp() const { return m_Up; }

private:
    glm::vec3 m_Position;
    glm::vec3 m_Forward;
    glm::vec3 m_Up;
};

// Audio Manager - Central audio system
class AudioManager {
public:
    static AudioManager* GetInstance() {
        static AudioManager instance;
        return &instance;
    }

    // Initialize audio system (stub for actual implementation)
    bool Initialize() {
        std::cout << "[Audio] Audio System Initialized (Stub)" << std::endl;
        std::cout << "[Audio] Note: For full audio support, integrate:" << std::endl;
        std::cout << "[Audio]   - OpenAL: https://github.com/kcat/openal-soft" << std::endl;
        std::cout << "[Audio]   - FMOD: https://www.fmod.com/" << std::endl;
        std::cout << "[Audio]   - irrKlang: https://www.ambiera.com/irrklang/" << std::endl;
        std::cout << "[Audio]   - miniaudio: https://github.com/mackron/miniaudio" << std::endl;
        m_Initialized = true;
        return true;
    }

    void Shutdown() {
        m_AudioSources.clear();
        m_Initialized = false;
        std::cout << "[Audio] Audio System Shutdown" << std::endl;
    }

    // Load audio file
    std::shared_ptr<AudioSource> LoadAudio(const std::string& name, const std::string& filepath) {
        auto source = std::make_shared<AudioSource>(filepath);
        m_AudioSources[name] = source;
        std::cout << "[Audio] Loaded: " << filepath << " as '" << name << "'" << std::endl;
        return source;
    }

    // Get audio source by name
    std::shared_ptr<AudioSource> GetAudioSource(const std::string& name) {
        auto it = m_AudioSources.find(name);
        if (it != m_AudioSources.end()) {
            return it->second;
        }
        return nullptr;
    }

    // Quick play functions
    void PlaySound(const std::string& name, float volume = 1.0f) {
        auto source = GetAudioSource(name);
        if (source) {
            source->SetVolume(volume);
            source->Play();
        }
    }

    void PlaySoundAtPosition(const std::string& name, const glm::vec3& position, float volume = 1.0f) {
        auto source = GetAudioSource(name);
        if (source) {
            source->SetPosition(position);
            source->SetVolume(volume);
            source->SetSpatial(true);
            source->Play();
        }
    }

    void StopSound(const std::string& name) {
        auto source = GetAudioSource(name);
        if (source) {
            source->Stop();
        }
    }

    // Global volume control
    void SetMasterVolume(float volume) { 
        m_MasterVolume = glm::clamp(volume, 0.0f, 1.0f); 
    }
    
    float GetMasterVolume() const { return m_MasterVolume; }

    void SetMusicVolume(float volume) { 
        m_MusicVolume = glm::clamp(volume, 0.0f, 1.0f); 
    }
    
    float GetMusicVolume() const { return m_MusicVolume; }

    void SetSFXVolume(float volume) { 
        m_SFXVolume = glm::clamp(volume, 0.0f, 1.0f); 
    }
    
    float GetSFXVolume() const { return m_SFXVolume; }

    // Listener (camera position)
    AudioListener& GetListener() { return m_Listener; }

    // Update (process 3D audio, fade-outs, etc.)
    void Update(float deltaTime) {
        // Update 3D audio positions, volume fades, etc.
        // In a real implementation, this would calculate distance attenuation
    }

    // Clear all audio
    void ClearAll() {
        m_AudioSources.clear();
    }

private:
    AudioManager() 
        : m_Initialized(false),
          m_MasterVolume(1.0f),
          m_MusicVolume(0.8f),
          m_SFXVolume(1.0f) {}
    
    ~AudioManager() {
        if (m_Initialized) {
            Shutdown();
        }
    }

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    bool m_Initialized;
    float m_MasterVolume;
    float m_MusicVolume;
    float m_SFXVolume;
    std::map<std::string, std::shared_ptr<AudioSource>> m_AudioSources;
    AudioListener m_Listener;
};

// Supported Audio Libraries for Integration:
// 
// 1. OpenAL (Open Audio Library) - Cross-platform 3D audio API
//    - GitHub: https://github.com/kcat/openal-soft
//    - License: LGPL
//    - Best for: 3D positional audio
//
// 2. FMOD - Professional audio middleware
//    - Website: https://www.fmod.com/
//    - License: Free for indie, paid for commercial
//    - Best for: Full-featured audio with advanced effects
//
// 3. miniaudio - Single-file public domain audio library
//    - GitHub: https://github.com/mackron/miniaudio
//    - License: Public Domain / MIT
//    - Best for: Simple integration, no dependencies
//
// 4. SoLoud - Easy to use, free audio engine
//    - GitHub: https://github.com/jarikomppa/soloud
//    - License: zlib/libpng
//    - Best for: Games, simple API

#endif // AUDIOSYSTEM_H
