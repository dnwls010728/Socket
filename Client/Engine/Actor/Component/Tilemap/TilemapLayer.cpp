#include "pch.h"
#include "TilemapLayer.h"

#include "TilemapChunk.h"
#include "Asset/AssetManager.h"
#include "Data/FileHelper.h"
#include "tmxlite/Map.hpp"
#include "Windows/DX/Texture.h"

TilemapLayer::TilemapLayer(const tmx::Map& map, const tmx::TileLayer& layer, Texture* texture, const Math::Vector2& chunk_size) :
    chunk_size_(chunk_size),
    chunk_count_(Math::Vector2::Zero()),
    chunks_()
{
    const tmx::Vector2u tile_size = map.getTileSize();
    map_tile_size_.x = tile_size.x;
    map_tile_size_.y = tile_size.y;

    chunk_size_.x = std::floor(chunk_size_.x / tile_size.x) * tile_size.x;
    chunk_size_.y = std::floor(chunk_size_.y / tile_size.y) * tile_size.y;

    CreateChunks(map, layer, texture, map_tile_size_);
}

void TilemapLayer::UpdateShapes(const Math::Vector2& position, const Math::Vector2& scale, const Math::Vector2& pivot)
{
    for (const auto& chunk : chunks_)
    {
        chunk->UpdateShape(position, scale, pivot);
    }
}

TilemapChunk* TilemapLayer::GetChunk(int x, int y)
{
    Type::uint32 chunk_x = (x * map_tile_size_.x) / chunk_size_.x;
    Type::uint32 chunk_y = (y * map_tile_size_.y) / chunk_size_.y;
    return chunks_[chunk_y * chunk_count_.x + chunk_x].get();
}

TilemapChunk* TilemapLayer::GetChunk(int x, int y, Math::Vector2& tile_relative_position)
{
    Type::uint32 chunk_x = (x * map_tile_size_.x) / chunk_size_.x;
    Type::uint32 chunk_y = (y * map_tile_size_.y) / chunk_size_.y;

    tile_relative_position.x = ((x * map_tile_size_.x) - chunk_x * chunk_size_.x) / map_tile_size_.x;
    tile_relative_position.y = ((y * map_tile_size_.y) - chunk_y * chunk_size_.y) / map_tile_size_.y;

    return chunks_[chunk_y * chunk_count_.x + chunk_x].get();
}

void TilemapLayer::CreateChunks(const tmx::Map& map, const tmx::TileLayer& layer, Texture* texture, const Math::Vector2& tile_size)
{
    const std::vector<tmx::Tileset>& tilesets = map.getTilesets();
    const std::vector<tmx::TileLayer::Tile>& tiles = layer.getTiles();

    Type::uint32 max_id = UINT_MAX;

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

        Texture* temp = AssetManager::Get()->Load<Texture>(to_wide_string);
        tileset_textures_[to_wide_string] = temp;
    }
    
    const auto bounds = map.getBounds();
    chunk_count_.x = std::ceil(bounds.width / chunk_size_.x);
    chunk_count_.y = std::ceil(bounds.height / chunk_size_.y);

    for (auto y = 0u; y < chunk_count_.y; ++y)
    {
        Math::Vector2 tile_count = {chunk_size_.x / tile_size.x, chunk_size_.y / tile_size.y};
        for (auto x = 0u; x < chunk_count_.x; ++x)
        {
            if ((x + 1) * chunk_size_.x > bounds.width)
            {
                tile_count.x = (bounds.width - x * chunk_size_.x) / tile_size.x;
            }

            if ((y + 1) * chunk_size_.y > bounds.height)
            {
                tile_count.y = (bounds.height - y * chunk_size_.y) / tile_size.y;
            }

            chunks_.push_back(std::make_unique<TilemapChunk>(layer, &map.getTilesets()[0], used_tilesets, Math::Vector2(x * chunk_size_.x, y * chunk_size_.y), tile_count, tile_size, map.getTileCount().x, texture, tileset_textures_));
        }
    }
}
