#pragma once
#include <memory>
#include <vector>

#include "Math/Vector2.h"
#include "tmxlite/TileLayer.hpp"

namespace tmx
{
    class Tileset;
}

class TilemapChunk
{
public:
    TilemapChunk(const tmx::TileLayer& kLayer, std::vector<const tmx::Tileset*> tilesets, const Math::Vector2& kPosition, const Math::Vector2& kTileCount, const Math::Vector2& kTileSize, uint64_t row_size);
    int32_t GetTileIndex(int32_t x, int32_t y) const;

private:
    void GenerateTiles(const uint32_t& kPosX, const uint32_t& kPosY, const Math::Vector2& kTileSize);

    Math::Vector2 tile_count_;
    
    std::vector<tmx::TileLayer::Tile> chunk_tile_ids_;
    
};
