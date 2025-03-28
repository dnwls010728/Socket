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

    void UpdateShapes(const Math::Vector2& kPosition, const Math::Vector2& kScale, const Math::Vector2& kPivot = Math::Vector2::Zero());

    TilemapChunk* GetChunk(int tile_x, int tile_y);
    TilemapChunk* GetChunk(int tile_x, int tile_y, Math::Vector2& tile_relative_position);

private:
    void CreateChunks(const tmx::Map& kMap, const tmx::TileLayer& kLayer, const Math::Vector2& kTileSize);

    Math::Vector2 chunk_size_;
    Math::Vector2 chunk_count_;
    Math::Vector2 map_tile_size_;

    std::map<std::wstring, Sprite*> tileset_textures_;
    
    std::vector<std::unique_ptr<TilemapChunk>> chunks_;
    
};
