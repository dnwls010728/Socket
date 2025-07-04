#pragma once
#include "Math/Math.h"
#include "Math/Vector2.h"

struct DefaultVertex;
class Shape;

namespace tmx
{
    class Tileset;
}

class Sprite;

class TilemapChunkArray
{
public:
    TilemapChunkArray(Sprite* texture, const tmx::Tileset& kTileset, int32_t order);
    ~TilemapChunkArray() = default;

    void SetShape(const std::vector<DefaultVertex>& kVertices, const std::vector<uint32_t>& kIndices);

    Math::Vector2 GetTextureSize();
    
    FORCEINLINE const Math::Vector2& GetTilesetSize() const { return tileset_size_; }
    FORCEINLINE const Math::Vector2& GetTileCount() const { return tile_count_; }

    FORCEINLINE uint32_t GetFirstGID() const { return first_gid_; }
    FORCEINLINE uint32_t GetLastGID() const { return last_gid_; }

    FORCEINLINE Shape* GetShape() const { return shape_.get(); }

private:
    Sprite* texture_;

    int32_t order_;
    
    Math::Vector2 tileset_size_;
    Math::Vector2 tile_count_;
    
    uint32_t first_gid_;
    uint32_t last_gid_;

    std::shared_ptr<Shape> shape_;
    
};
