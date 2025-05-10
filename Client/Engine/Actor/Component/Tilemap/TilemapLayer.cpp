#include "pch.h"
#include "TilemapLayer.h"

#include "TilemapChunk.h"
#include "Asset/AssetManager.h"
#include "Data/FileHelper.h"
#include "tmxlite/Map.hpp"
#include "Windows/DX/Sprite.h"

TilemapLayer::TilemapLayer(const tmx::Map& kMap, const tmx::TileLayer& kLayer, const Math::Vector2& kChunkSize) :
    chunk_size_(kChunkSize),
    chunk_count_(Math::Vector2::Zero()),
    chunks_()
{
    const tmx::Vector2u tile_size = kMap.getTileSize();
    map_tile_size_.x = tile_size.x;
    map_tile_size_.y = tile_size.y;

    chunk_size_.x = std::floor(chunk_size_.x / tile_size.x) * tile_size.x;
    chunk_size_.y = std::floor(chunk_size_.y / tile_size.y) * tile_size.y;

    CreateChunks(kMap, kLayer, map_tile_size_);
}

void TilemapLayer::UpdateShapes(const Math::Vector2& kPosition, const Math::Vector2& kScale, const Math::Vector2& kPivot)
{
    for (const auto& chunk : chunks_)
    {
        chunk->UpdateShape(kPosition, kScale, kPivot);
    }
}

TilemapChunk* TilemapLayer::GetChunk(int32_t x, int32_t y)
{
    uint32_t chunk_x = (x * map_tile_size_.x) / chunk_size_.x;
    uint32_t chunk_y = (y * map_tile_size_.y) / chunk_size_.y;
    return chunks_[chunk_y * chunk_count_.x + chunk_x].get();
}

TilemapChunk* TilemapLayer::GetChunk(int32_t x, int32_t y, Math::Vector2& tile_relative_position)
{
    uint32_t chunk_x = (x * map_tile_size_.x) / chunk_size_.x;
    uint32_t chunk_y = (y * map_tile_size_.y) / chunk_size_.y;

    tile_relative_position.x = ((x * map_tile_size_.x) - chunk_x * chunk_size_.x) / map_tile_size_.x;
    tile_relative_position.y = ((y * map_tile_size_.y) - chunk_y * chunk_size_.y) / map_tile_size_.y;

    return chunks_[chunk_y * chunk_count_.x + chunk_x].get();
}

void TilemapLayer::CreateChunks(const tmx::Map& kMap, const tmx::TileLayer& kLayer, const Math::Vector2& kTileSize)
{
    const std::vector<tmx::Tileset>& tilesets = kMap.getTilesets();
    const std::vector<tmx::TileLayer::Tile>& tiles = kLayer.getTiles();

    uint32_t max_id = UINT_MAX;

    std::vector<const tmx::Tileset*> used_tilesets;

    for (auto it = tilesets.rbegin(); it != tilesets.rend(); ++it)
    {
        for (const auto& kTile : tiles)
        {
            if (kTile.ID >= it->getFirstGID() && kTile.ID < max_id)
            {
                used_tilesets.push_back(&(*it));
                break;
            }
        }

        max_id = it->getFirstGID();
    }

    for (const auto& kTileset : used_tilesets)
    {
        const std::string kPath = kTileset->getImagePath();
        
        std::wstring to_wide_string = std::wstring(kPath.begin(), kPath.end());
        to_wide_string = FileHelper::GetRelativePath(to_wide_string);

        Sprite* temp = AssetManager::Get()->Load<Sprite>(to_wide_string);
        tileset_textures_[to_wide_string] = temp;
    }
    
    const auto bounds = kMap.getBounds();
    chunk_count_.x = std::ceil(bounds.width / chunk_size_.x);
    chunk_count_.y = std::ceil(bounds.height / chunk_size_.y);

    for (auto y = 0u; y < chunk_count_.y; ++y)
    {
        Math::Vector2 tile_count = {chunk_size_.x / kTileSize.x, chunk_size_.y / kTileSize.y};
        for (auto x = 0u; x < chunk_count_.x; ++x)
        {
            if ((x + 1) * chunk_size_.x > bounds.width)
            {
                tile_count.x = (bounds.width - x * chunk_size_.x) / kTileSize.x;
            }

            if ((y + 1) * chunk_size_.y > bounds.height)
            {
                tile_count.y = (bounds.height - y * chunk_size_.y) / kTileSize.y;
            }

            chunks_.push_back(std::make_unique<TilemapChunk>(kLayer, used_tilesets, Math::Vector2(x * chunk_size_.x, y * chunk_size_.y), tile_count, kTileSize, kMap.getTileCount().x, tileset_textures_));
        }
    }
}
