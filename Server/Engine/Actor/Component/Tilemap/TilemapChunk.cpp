#include "pch.h"
#include "TilemapChunk.h"

#include "Data/FileHelper.h"
#include "tmxlite/Tileset.hpp"

TilemapChunk::TilemapChunk(const tmx::TileLayer& kLayer, std::vector<const tmx::Tileset*> tilesets, const Math::Vector2& kPosition, const Math::Vector2& kTileCount, const Math::Vector2& kTileSize, uint64_t row_size) :
    tile_count_(kTileCount),
    chunk_tile_ids_()
{
    const std::vector<tmx::Property> kProperties = kLayer.getProperties();
    
    int32_t order = 0;
    for (const auto& kProperty : kProperties)
    {
        if (kProperty.getName() == "Order")
        {
            order = kProperty.getIntValue();
            break;
        }
    }
    
    uint32_t pos_x = static_cast<uint32_t>(kPosition.x / kTileSize.x);
    uint32_t pos_y = static_cast<uint32_t>(kPosition.y / kTileSize.y);

    for (uint32_t y = pos_y; y < pos_y + kTileCount.y; ++y)
    {
        for (uint32_t x = pos_x; x < pos_x + kTileCount.x; ++x)
        {
            uint32_t idx = y * row_size + x;
            chunk_tile_ids_.push_back(kLayer.getTiles()[idx]);
        }
    }
}

int32_t TilemapChunk::GetTileIndex(int32_t x, int32_t y) const
{
    return y * tile_count_.x + x;
}
