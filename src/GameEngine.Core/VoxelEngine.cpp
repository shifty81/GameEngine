#include "VoxelEngine.h"
#include "VoxelChunk.h"
#include "Renderer.h"
#include "Camera.h"
#include <cmath>
#include <random>

VoxelEngine::VoxelEngine()
    : m_seed(12345)
{
}

VoxelEngine::~VoxelEngine() = default;

void VoxelEngine::Initialize() {
    // Generate initial terrain
    GenerateTerrain(m_seed);
}

void VoxelEngine::Update(float deltaTime) {
    // Update chunks, unload far chunks, load near chunks, etc.
}

void VoxelEngine::Render(Renderer* renderer, Camera* camera) {
    // Render all visible chunks
    for (auto& pair : m_chunks) {
        pair.second->Render(renderer, camera);
    }
}

void VoxelEngine::SetVoxel(int x, int y, int z, uint8_t blockType) {
    ChunkCoord chunkCoord = WorldToChunk(x, y, z);
    VoxelChunk* chunk = GetOrCreateChunk(chunkCoord);
    
    if (chunk) {
        int localX = x - chunkCoord.x * CHUNK_SIZE;
        int localY = y - chunkCoord.y * CHUNK_SIZE;
        int localZ = z - chunkCoord.z * CHUNK_SIZE;
        chunk->SetVoxel(localX, localY, localZ, blockType);
    }
}

uint8_t VoxelEngine::GetVoxel(int x, int y, int z) {
    ChunkCoord chunkCoord = WorldToChunk(x, y, z);
    VoxelChunk* chunk = GetChunk(chunkCoord);
    
    if (chunk) {
        int localX = x - chunkCoord.x * CHUNK_SIZE;
        int localY = y - chunkCoord.y * CHUNK_SIZE;
        int localZ = z - chunkCoord.z * CHUNK_SIZE;
        return chunk->GetVoxel(localX, localY, localZ);
    }
    
    return 0;
}

void VoxelEngine::GenerateTerrain(int seed) {
    m_seed = seed;
    m_chunks.clear();
    
    // Generate a 4x2x4 grid of chunks for testing
    for (int cx = -2; cx < 2; ++cx) {
        for (int cy = -1; cy < 1; ++cy) {
            for (int cz = -2; cz < 2; ++cz) {
                ChunkCoord coord{ cx, cy, cz };
                auto chunk = std::make_unique<VoxelChunk>(coord.x, coord.y, coord.z);
                chunk->GenerateTerrain(seed);
                m_chunks[coord] = std::move(chunk);
            }
        }
    }
}

ChunkCoord VoxelEngine::WorldToChunk(int x, int y, int z) {
    return ChunkCoord{
        x >= 0 ? x / CHUNK_SIZE : (x - CHUNK_SIZE + 1) / CHUNK_SIZE,
        y >= 0 ? y / CHUNK_SIZE : (y - CHUNK_SIZE + 1) / CHUNK_SIZE,
        z >= 0 ? z / CHUNK_SIZE : (z - CHUNK_SIZE + 1) / CHUNK_SIZE
    };
}

VoxelChunk* VoxelEngine::GetChunk(const ChunkCoord& coord) {
    auto it = m_chunks.find(coord);
    return (it != m_chunks.end()) ? it->second.get() : nullptr;
}

VoxelChunk* VoxelEngine::GetOrCreateChunk(const ChunkCoord& coord) {
    auto it = m_chunks.find(coord);
    if (it != m_chunks.end()) {
        return it->second.get();
    }
    
    auto chunk = std::make_unique<VoxelChunk>(coord.x, coord.y, coord.z);
    VoxelChunk* ptr = chunk.get();
    m_chunks[coord] = std::move(chunk);
    return ptr;
}
