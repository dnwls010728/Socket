#include "pch.h"
#include "TilemapChunk.h"

#include "Data/FileHelper.h"
#include "Level/World.h"
#include "tmxlite/Tileset.hpp"
#include "Windows/DX/Shape.h"
#include "Windows/DX/Texture.h"

TilemapChunk::TilemapChunk(const tmx::TileLayer& kLayer, const tmx::Tileset* kTileset, std::vector<const tmx::Tileset*> tilesets, const Math::Vector2& kPosition, const Math::Vector2& kTileCount, const Math::Vector2& kTileSize, Type::uint64 row_size, Texture* kTexture, std::map<std::wstring, Texture*>& tileset_textures) :
    tile_count_(kTileCount),
    texture_(kTexture),
    chunk_tile_ids_(),
    chunk_arrays_(),
    vertices_(),
    indices_(),
    shape_(nullptr)
{
    for (const auto& kTileset : tilesets)
    {
        const std::string kPath = kTileset->getImagePath();
        
        std::wstring to_wide_string = std::wstring(kPath.begin(), kPath.end());
        to_wide_string = FileHelper::GetRelativePath(to_wide_string);

        if (kTileset->getImagePath().empty()) continue;
        chunk_arrays_.emplace_back(std::make_unique<TilemapChunkArray>(tileset_textures.find(to_wide_string)->second, *kTileset));
    }
    
    Type::uint32 pos_x = static_cast<Type::uint32>(kPosition.x / kTileSize.x);
    Type::uint32 pos_y = static_cast<Type::uint32>(kPosition.y / kTileSize.y);

    for (Type::uint32 y = pos_y; y < pos_y + kTileCount.y; ++y)
    {
        for (Type::uint32 x = pos_x; x < pos_x + kTileCount.x; ++x)
        {
            Type::uint32 idx = y * row_size + x;
            chunk_tile_ids_.push_back(kLayer.getTiles()[idx]);
        }
    }

    GenerateTiles(kTileset, pos_x, pos_y, kTileSize);
}

void TilemapChunk::UpdateShape(const Math::Vector2& kPosition, const Math::Vector2& kScale, const Math::Vector2& kPivot)
{
    for (const auto& chunk_array : chunk_arrays_)
    {
        chunk_array->GetShape()->SetPosition(kPosition);
        chunk_array->GetShape()->SetScale(kScale);
        chunk_array->GetShape()->SetPivot(kPivot);
    }
}

int TilemapChunk::GetTileIndex(int x, int y) const
{
    return y * tile_count_.x + x;
}

void TilemapChunk::GenerateTiles(const tmx::Tileset* kTileset, const Type::uint32& kPosX, const Type::uint32& kPosY, const Math::Vector2& kTileSize)
{
    for (const auto& chunk_array : chunk_arrays_)
    {
        Type::uint32 idx = 0;

        const float u_normal = chunk_array->GetTilesetSize().x / chunk_array->GetTextureSize().x;
        const float v_normal = chunk_array->GetTilesetSize().y / chunk_array->GetTextureSize().y;
        
        for (Type::uint32 y = kPosY; y < kPosY + tile_count_.y; ++y)
        {
            for (Type::uint32 x = kPosX; x < kPosX + tile_count_.x; ++x)
            {
                if (idx < chunk_tile_ids_.size() && chunk_tile_ids_[idx].ID >= chunk_array->GetFirstGID() &&
                    chunk_tile_ids_[idx].ID <= chunk_array->GetLastGID())
                {
                    Type::uint32 id_idx = chunk_tile_ids_[idx].ID - chunk_array->GetFirstGID();
                    float u = static_cast<float>(id_idx % static_cast<Type::uint32>(chunk_array->GetTileCount().x));
                    float v = static_cast<float>(id_idx / static_cast<Type::uint32>(chunk_array->GetTileCount().x));
                    u *= chunk_array->GetTilesetSize().x;
                    v *= chunk_array->GetTilesetSize().y;

                    u /= chunk_array->GetTextureSize().x;
                    v /= chunk_array->GetTextureSize().y;

                    const float tile_pos_x = static_cast<float>(x) * kTileSize.x;
                    const float tile_pos_y = static_cast<float>(y) * kTileSize.y + kTileSize.y;
                    
                    DefaultVertex vertex = { {tile_pos_x, -tile_pos_y, 0.f}, {1.f, 1.f, 1.f, 1.f}, {u, v + v_normal} };
                    vertices_.push_back(vertex);
                    vertex = { {tile_pos_x + chunk_array->GetTilesetSize().x, -tile_pos_y, 0.f}, {1.f, 1.f, 1.f, 1.f}, {u + u_normal, v + v_normal } };
                    vertices_.push_back(vertex);
                    vertex = { {tile_pos_x, -tile_pos_y + chunk_array->GetTilesetSize().y, 0.f}, {1.f, 1.f, 1.f, 1.f}, {u, v } };
                    vertices_.push_back(vertex);
                    vertex = { {tile_pos_x + chunk_array->GetTilesetSize().x, -tile_pos_y + chunk_array->GetTilesetSize().y, 0.f}, {1.f, 1.f, 1.f, 1.f}, {u + u_normal, v } };
                    vertices_.push_back(vertex);

                    Type::uint32 base_index = static_cast<Type::uint32>(vertices_.size() - 4);
                    indices_.push_back(base_index);
                    indices_.push_back(base_index + 1);
                    indices_.push_back(base_index + 2);
                    indices_.push_back(base_index + 2);
                    indices_.push_back(base_index + 1);
                    indices_.push_back(base_index + 3);
                }

                ++idx;
            }
        }

        chunk_array->SetShape(vertices_, indices_);
    }
}
