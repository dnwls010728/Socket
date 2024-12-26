#pragma once
#include <memory>
#include <vector>

#include "TilemapChunkArray.h"
#include "Math/Vector2.h"
#include "tmxlite/TileLayer.hpp"
#include "Windows/DX/Vertex.h"

namespace tmx
{
    class Tileset;
}

class Texture;

class TilemapChunk
{
public:
    TilemapChunk(const tmx::TileLayer& kLayer, const tmx::Tileset* kTileset, std::vector<const tmx::Tileset*> tilesets, const Math::Vector2& kPosition, const Math::Vector2& kTileCount, const Math::Vector2& kTileSize, Type::uint64 row_size, Texture* kTexture, std::map<std::wstring, Texture*>& tileset_textures);

    void UpdateShape(const Math::Vector2& kPosition, const Math::Vector2& kScale, const Math::Vector2& kPivot = Math::Vector2::Zero());
    
    int GetTileIndex(int x, int y) const;

private:
    void GenerateTiles(const tmx::Tileset* kTileset, const Type::uint32& kPosX, const Type::uint32& kPosY, const Math::Vector2& kTileSize);

    Math::Vector2 tile_count_;
    
    Texture* texture_;
    
    std::vector<tmx::TileLayer::Tile> chunk_tile_ids_;

    std::vector<std::unique_ptr<TilemapChunkArray>> chunk_arrays_;
    std::vector<DefaultVertex> vertices_;
    std::vector<Type::uint32> indices_;

    std::shared_ptr<class Shape> shape_;
    
};
