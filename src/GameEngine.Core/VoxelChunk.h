#pragma once

#include <cstdint>
#include <vector>
#include <DirectXMath.h>

class Renderer;
class Camera;

constexpr int CHUNK_SIZE = 16;
constexpr int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

enum class BlockType : uint8_t {
    Air = 0,
    Grass = 1,
    Dirt = 2,
    Stone = 3,
    Sand = 4,
    Water = 5
};

struct Vertex {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 texCoord;
    DirectX::XMFLOAT3 color;
};

class VoxelChunk {
public:
    VoxelChunk(int chunkX, int chunkY, int chunkZ);
    ~VoxelChunk();
    
    void SetVoxel(int x, int y, int z, uint8_t blockType);
    uint8_t GetVoxel(int x, int y, int z) const;
    
    void GenerateTerrain(int seed);
    void RegenerateMesh();
    void Render(Renderer* renderer, Camera* camera);
    
private:
    int GetIndex(int x, int y, int z) const;
    bool IsVoxelSolid(int x, int y, int z) const;
    void AddFace(const DirectX::XMFLOAT3& pos, int face, BlockType blockType);
    DirectX::XMFLOAT3 GetBlockColor(BlockType type) const;
    
    uint8_t m_voxels[CHUNK_VOLUME];
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    
    int m_chunkX, m_chunkY, m_chunkZ;
    bool m_meshDirty;
};
