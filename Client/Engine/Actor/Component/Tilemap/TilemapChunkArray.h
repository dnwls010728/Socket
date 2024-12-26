#pragma once
#include "Math/Math.h"
#include "Math/Vector2.h"

struct DefaultVertex;
class Shape;

namespace tmx
{
    class Tileset;
}

class Texture;

class TilemapChunkArray
{
public:
    TilemapChunkArray(Texture* texture, const tmx::Tileset& kTileset, int order);
    ~TilemapChunkArray() = default;

    void SetShape(const std::vector<DefaultVertex>& kVertices, const std::vector<Type::uint32>& kIndices);

    Math::Vector2 GetTextureSize();
    
    FORCEINLINE Math::Vector2 GetTilesetSize() const { return tileset_size_; }
    FORCEINLINE Math::Vector2 GetTileCount() const { return tile_count_; }

    FORCEINLINE Type::uint32 GetFirstGID() const { return first_gid_; }
    FORCEINLINE Type::uint32 GetLastGID() const { return last_gid_; }

    FORCEINLINE Shape* GetShape() const { return shape_.get(); }

private:
    Texture* texture_;

    int order_;
    
    Math::Vector2 tileset_size_;
    Math::Vector2 tile_count_;
    
    Type::uint32 first_gid_;
    Type::uint32 last_gid_;

    std::shared_ptr<Shape> shape_;
    
};
