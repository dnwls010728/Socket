#include "pch.h"
#include "TilemapChunk.h"

#include "Data/FileHelper.h"
#include "Level/World.h"
#include "Misc/StringHelper.h"
#include "tmxlite/Tileset.hpp"
#include "Windows/DX/Shape.h"
#include "Windows/DX/Sprite.h"

TilemapChunk::TilemapChunk(const tmx::TileLayer& kLayer, std::vector<const tmx::Tileset*> tilesets, const Math::Vector2& kPosition, const Math::Vector2& kTileCount, const Math::Vector2& kTileSize, uint64_t row_size, std::map<std::wstring, Sprite*>& tileset_textures) :
    tile_count_(kTileCount),
    chunk_tile_ids_(),
    chunk_arrays_()
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
    
    for (const auto& kTileset : tilesets)
    {
        std::wstring path = StringHelper::UTF8ToUTF16(kTileset->getImagePath());
        path = FileHelper::GetRelativePath(path);

        if (kTileset->getImagePath().empty()) continue;
        chunk_arrays_.emplace_back(std::make_unique<TilemapChunkArray>(tileset_textures.find(path)->second, *kTileset, order));
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

    GenerateTiles(pos_x, pos_y, kTileSize);
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

void TilemapChunk::AddShapes() const
{
    for (const auto& chunk_array : chunk_arrays_)
    {
        chunk_array->AddShape();
    }
}

void TilemapChunk::RemoveShapes() const
{
    for (const auto& chunk_array : chunk_arrays_)
    {
        chunk_array->RemoveShape();
    }
}

int32_t TilemapChunk::GetTileIndex(int32_t x, int32_t y) const
{
    return y * tile_count_.x + x;
}

void TilemapChunk::GenerateTiles(const uint32_t& kPosX, const uint32_t& kPosY, const Math::Vector2& kTileSize)
{
    for (const auto& chunk_array : chunk_arrays_)
    {
        uint32_t idx = 0;

        const float u_normal = chunk_array->GetTilesetSize().x / chunk_array->GetTextureSize().x;
        const float v_normal = chunk_array->GetTilesetSize().y / chunk_array->GetTextureSize().y;

        std::vector<DefaultVertex> vertices;
        std::vector<uint32_t> indices;
        
        for (uint32_t y = kPosY; y < kPosY + tile_count_.y; ++y)
        {
            for (uint32_t x = kPosX; x < kPosX + tile_count_.x; ++x)
            {
                if (idx < chunk_tile_ids_.size() && chunk_tile_ids_[idx].ID >= chunk_array->GetFirstGID() &&
                    chunk_tile_ids_[idx].ID <= chunk_array->GetLastGID())
                {
                    uint32_t id_idx = chunk_tile_ids_[idx].ID - chunk_array->GetFirstGID();
                    float u = static_cast<float>(id_idx % static_cast<uint32_t>(chunk_array->GetTileCount().x));
                    float v = static_cast<float>(id_idx / static_cast<uint32_t>(chunk_array->GetTileCount().x));
                    u *= chunk_array->GetTilesetSize().x;
                    v *= chunk_array->GetTilesetSize().y;

                    u /= chunk_array->GetTextureSize().x;
                    v /= chunk_array->GetTextureSize().y;

                    const float tile_pos_x = static_cast<float>(x) * kTileSize.x;
                    const float tile_pos_y = static_cast<float>(y) * kTileSize.y + kTileSize.y;
                    
                    DefaultVertex vertex = { {tile_pos_x, -tile_pos_y, 0.f}, {1.f, 1.f, 1.f, 1.f}, {u, v + v_normal} };
                    vertices.push_back(vertex);
                    vertex = { {tile_pos_x + chunk_array->GetTilesetSize().x, -tile_pos_y, 0.f}, {1.f, 1.f, 1.f, 1.f}, {u + u_normal, v + v_normal } };
                    vertices.push_back(vertex);
                    vertex = { {tile_pos_x, -tile_pos_y + chunk_array->GetTilesetSize().y, 0.f}, {1.f, 1.f, 1.f, 1.f}, {u, v } };
                    vertices.push_back(vertex);
                    vertex = { {tile_pos_x + chunk_array->GetTilesetSize().x, -tile_pos_y + chunk_array->GetTilesetSize().y, 0.f}, {1.f, 1.f, 1.f, 1.f}, {u + u_normal, v } };
                    vertices.push_back(vertex);

                    uint32_t base_index = static_cast<uint32_t>(vertices.size() - 4);
                    indices.push_back(base_index);
                    indices.push_back(base_index + 1);
                    indices.push_back(base_index + 2);
                    indices.push_back(base_index + 2);
                    indices.push_back(base_index + 1);
                    indices.push_back(base_index + 3);
                }

                ++idx;
            }
        }

        chunk_array->SetShape(vertices, indices);
        
        vertices.clear();
        indices.clear();
    }
}
