#pragma once
#include <memory>
#include <vector>

#include "TilemapChunk.h"
#include "Math/Vector2.h"

namespace tmx
{
    class TileLayer;
    class Map;
}

class Sprite;

class TilemapLayer
{
public:
    TilemapLayer(const tmx::Map& kMap, const tmx::TileLayer& kLayer, const Math::Vector2& kChunkSize);
    ~TilemapLayer() = default;
    
    TilemapChunk* GetChunk(int32_t tile_x, int32_t tile_y);
    TilemapChunk* GetChunk(int32_t tile_x, int32_t tile_y, Math::Vector2& tile_relative_position);

private:
    void CreateChunks(const tmx::Map& kMap, const tmx::TileLayer& kLayer, const Math::Vector2& kTileSize);

    Math::Vector2 chunk_size_;
    Math::Vector2 chunk_count_;
    Math::Vector2 map_tile_size_;
    
    std::vector<std::unique_ptr<TilemapChunk>> chunks_;
    
};
