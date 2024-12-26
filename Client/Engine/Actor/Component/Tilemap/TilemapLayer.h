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

class Texture;

class TilemapLayer
{
public:
    TilemapLayer(const tmx::Map& map, const tmx::TileLayer& layer, Texture* texture, const Math::Vector2& chunk_size);
    ~TilemapLayer() = default;

    void UpdateShapes(const Math::Vector2& position, const Math::Vector2& scale, const Math::Vector2& pivot = Math::Vector2::Zero());

    TilemapChunk* GetChunk(int tile_x, int tile_y);
    TilemapChunk* GetChunk(int tile_x, int tile_y, Math::Vector2& tile_relative_position);

private:
    void CreateChunks(const tmx::Map& map, const tmx::TileLayer& layer, Texture* texture, const Math::Vector2& tile_size);

    Math::Vector2 chunk_size_;
    Math::Vector2 chunk_count_;
    Math::Vector2 map_tile_size_;

    std::map<std::wstring, Texture*> tileset_textures_;
    
    std::vector<std::unique_ptr<TilemapChunk>> chunks_;
    
};
