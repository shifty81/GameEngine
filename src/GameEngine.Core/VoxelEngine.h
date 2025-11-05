#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>

class VoxelChunk;
class Renderer;
class Camera;

struct ChunkCoord {
    int x, y, z;
    
    bool operator==(const ChunkCoord& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

namespace std {
    template <>
    struct hash<ChunkCoord> {
        size_t operator()(const ChunkCoord& coord) const {
            return hash<int>()(coord.x) ^ (hash<int>()(coord.y) << 1) ^ (hash<int>()(coord.z) << 2);
        }
    };
}

class VoxelEngine {
public:
    VoxelEngine();
    ~VoxelEngine();
    
    void Initialize();
    void Update(float deltaTime);
    void Render(Renderer* renderer, Camera* camera);
    
    void SetVoxel(int x, int y, int z, uint8_t blockType);
    uint8_t GetVoxel(int x, int y, int z);
    
    void GenerateTerrain(int seed);
    
private:
    ChunkCoord WorldToChunk(int x, int y, int z);
    VoxelChunk* GetChunk(const ChunkCoord& coord);
    VoxelChunk* GetOrCreateChunk(const ChunkCoord& coord);
    
    std::unordered_map<ChunkCoord, std::unique_ptr<VoxelChunk>> m_chunks;
    int m_seed;
};
