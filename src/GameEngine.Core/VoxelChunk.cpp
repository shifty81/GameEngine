#include "VoxelChunk.h"
#include "Renderer.h"
#include "Camera.h"
#include <cmath>
#include <algorithm>

// Simple 3D Perlin-like noise function
static float Noise3D(float x, float y, float z, int seed) {
    int n = static_cast<int>(x * 57 + y * 113 + z * 197 + seed * 1019);
    n = (n << 13) ^ n;
    return 1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f;
}

static float PerlinNoise3D(float x, float y, float z, int seed) {
    int xi = static_cast<int>(std::floor(x));
    int yi = static_cast<int>(std::floor(y));
    int zi = static_cast<int>(std::floor(z));
    
    float xf = x - xi;
    float yf = y - yi;
    float zf = z - zi;
    
    // Smoothstep
    float u = xf * xf * (3 - 2 * xf);
    float v = yf * yf * (3 - 2 * yf);
    float w = zf * zf * (3 - 2 * zf);
    
    // Sample corners
    float n000 = Noise3D(static_cast<float>(xi), static_cast<float>(yi), static_cast<float>(zi), seed);
    float n100 = Noise3D(static_cast<float>(xi + 1), static_cast<float>(yi), static_cast<float>(zi), seed);
    float n010 = Noise3D(static_cast<float>(xi), static_cast<float>(yi + 1), static_cast<float>(zi), seed);
    float n110 = Noise3D(static_cast<float>(xi + 1), static_cast<float>(yi + 1), static_cast<float>(zi), seed);
    float n001 = Noise3D(static_cast<float>(xi), static_cast<float>(yi), static_cast<float>(zi + 1), seed);
    float n101 = Noise3D(static_cast<float>(xi + 1), static_cast<float>(yi), static_cast<float>(zi + 1), seed);
    float n011 = Noise3D(static_cast<float>(xi), static_cast<float>(yi + 1), static_cast<float>(zi + 1), seed);
    float n111 = Noise3D(static_cast<float>(xi + 1), static_cast<float>(yi + 1), static_cast<float>(zi + 1), seed);
    
    // Trilinear interpolation
    float x00 = n000 * (1 - u) + n100 * u;
    float x10 = n010 * (1 - u) + n110 * u;
    float x01 = n001 * (1 - u) + n101 * u;
    float x11 = n011 * (1 - u) + n111 * u;
    
    float y0 = x00 * (1 - v) + x10 * v;
    float y1 = x01 * (1 - v) + x11 * v;
    
    return y0 * (1 - w) + y1 * w;
}

VoxelChunk::VoxelChunk(int chunkX, int chunkY, int chunkZ)
    : m_chunkX(chunkX)
    , m_chunkY(chunkY)
    , m_chunkZ(chunkZ)
    , m_meshDirty(true)
{
    std::fill(std::begin(m_voxels), std::end(m_voxels), static_cast<uint8_t>(BlockType::Air));
}

VoxelChunk::~VoxelChunk() = default;

void VoxelChunk::SetVoxel(int x, int y, int z, uint8_t blockType) {
    if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE) {
        m_voxels[GetIndex(x, y, z)] = blockType;
        m_meshDirty = true;
    }
}

uint8_t VoxelChunk::GetVoxel(int x, int y, int z) const {
    if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE) {
        return m_voxels[GetIndex(x, y, z)];
    }
    return static_cast<uint8_t>(BlockType::Air);
}

void VoxelChunk::GenerateTerrain(int seed) {
    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int z = 0; z < CHUNK_SIZE; ++z) {
            int worldX = m_chunkX * CHUNK_SIZE + x;
            int worldZ = m_chunkZ * CHUNK_SIZE + z;
            
            // Generate height using Perlin noise
            float noise = PerlinNoise3D(worldX * 0.05f, 0, worldZ * 0.05f, seed);
            int height = static_cast<int>(noise * 8.0f) + 8; // Height from 0 to 16
            
            for (int y = 0; y < CHUNK_SIZE; ++y) {
                int worldY = m_chunkY * CHUNK_SIZE + y;
                
                if (worldY < height - 3) {
                    SetVoxel(x, y, z, static_cast<uint8_t>(BlockType::Stone));
                } else if (worldY < height - 1) {
                    SetVoxel(x, y, z, static_cast<uint8_t>(BlockType::Dirt));
                } else if (worldY < height) {
                    SetVoxel(x, y, z, static_cast<uint8_t>(BlockType::Grass));
                } else {
                    SetVoxel(x, y, z, static_cast<uint8_t>(BlockType::Air));
                }
            }
        }
    }
    
    m_meshDirty = true;
}

void VoxelChunk::RegenerateMesh() {
    m_vertices.clear();
    m_indices.clear();
    
    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                BlockType blockType = static_cast<BlockType>(GetVoxel(x, y, z));
                if (blockType == BlockType::Air) continue;
                
                DirectX::XMFLOAT3 blockPos(
                    static_cast<float>(m_chunkX * CHUNK_SIZE + x),
                    static_cast<float>(m_chunkY * CHUNK_SIZE + y),
                    static_cast<float>(m_chunkZ * CHUNK_SIZE + z)
                );
                
                // Check each face and add if not occluded
                if (!IsVoxelSolid(x, y, z + 1)) AddFace(blockPos, 0, blockType); // Front
                if (!IsVoxelSolid(x, y, z - 1)) AddFace(blockPos, 1, blockType); // Back
                if (!IsVoxelSolid(x, y + 1, z)) AddFace(blockPos, 2, blockType); // Top
                if (!IsVoxelSolid(x, y - 1, z)) AddFace(blockPos, 3, blockType); // Bottom
                if (!IsVoxelSolid(x + 1, y, z)) AddFace(blockPos, 4, blockType); // Right
                if (!IsVoxelSolid(x - 1, y, z)) AddFace(blockPos, 5, blockType); // Left
            }
        }
    }
    
    m_meshDirty = false;
}

void VoxelChunk::Render(Renderer* renderer, Camera* camera) {
    if (m_meshDirty) {
        RegenerateMesh();
    }
    
    // TODO: Implement actual rendering with DirectX
    // For now, this is a placeholder
}

int VoxelChunk::GetIndex(int x, int y, int z) const {
    return x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE;
}

bool VoxelChunk::IsVoxelSolid(int x, int y, int z) const {
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE) {
        return false; // Assume air outside chunk
    }
    return GetVoxel(x, y, z) != static_cast<uint8_t>(BlockType::Air);
}

void VoxelChunk::AddFace(const DirectX::XMFLOAT3& pos, int face, BlockType blockType) {
    using namespace DirectX;
    
    // Face vertices (simplified cube)
    static const XMFLOAT3 faceVertices[6][4] = {
        // Front (+Z)
        { XMFLOAT3(0, 0, 1), XMFLOAT3(1, 0, 1), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 1, 1) },
        // Back (-Z)
        { XMFLOAT3(1, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1, 0), XMFLOAT3(1, 1, 0) },
        // Top (+Y)
        { XMFLOAT3(0, 1, 0), XMFLOAT3(1, 1, 0), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 1, 1) },
        // Bottom (-Y)
        { XMFLOAT3(0, 0, 1), XMFLOAT3(1, 0, 1), XMFLOAT3(1, 0, 0), XMFLOAT3(0, 0, 0) },
        // Right (+X)
        { XMFLOAT3(1, 0, 1), XMFLOAT3(1, 0, 0), XMFLOAT3(1, 1, 0), XMFLOAT3(1, 1, 1) },
        // Left (-X)
        { XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 1), XMFLOAT3(0, 1, 1), XMFLOAT3(0, 1, 0) }
    };
    
    static const XMFLOAT3 faceNormals[6] = {
        XMFLOAT3(0, 0, 1),   // Front
        XMFLOAT3(0, 0, -1),  // Back
        XMFLOAT3(0, 1, 0),   // Top
        XMFLOAT3(0, -1, 0),  // Bottom
        XMFLOAT3(1, 0, 0),   // Right
        XMFLOAT3(-1, 0, 0)   // Left
    };
    
    XMFLOAT3 color = GetBlockColor(blockType);
    uint32_t baseIndex = static_cast<uint32_t>(m_vertices.size());
    
    for (int i = 0; i < 4; ++i) {
        Vertex v;
        v.position = XMFLOAT3(
            pos.x + faceVertices[face][i].x,
            pos.y + faceVertices[face][i].y,
            pos.z + faceVertices[face][i].z
        );
        v.normal = faceNormals[face];
        v.texCoord = XMFLOAT2(i % 2 == 0 ? 0.0f : 1.0f, i < 2 ? 0.0f : 1.0f);
        v.color = color;
        m_vertices.push_back(v);
    }
    
    // Two triangles per face
    m_indices.push_back(baseIndex + 0);
    m_indices.push_back(baseIndex + 1);
    m_indices.push_back(baseIndex + 2);
    m_indices.push_back(baseIndex + 0);
    m_indices.push_back(baseIndex + 2);
    m_indices.push_back(baseIndex + 3);
}

DirectX::XMFLOAT3 VoxelChunk::GetBlockColor(BlockType type) const {
    switch (type) {
        case BlockType::Grass: return DirectX::XMFLOAT3(0.3f, 0.8f, 0.2f);
        case BlockType::Dirt: return DirectX::XMFLOAT3(0.6f, 0.4f, 0.2f);
        case BlockType::Stone: return DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
        case BlockType::Sand: return DirectX::XMFLOAT3(0.9f, 0.9f, 0.6f);
        case BlockType::Water: return DirectX::XMFLOAT3(0.2f, 0.4f, 0.8f);
        default: return DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
    }
}
