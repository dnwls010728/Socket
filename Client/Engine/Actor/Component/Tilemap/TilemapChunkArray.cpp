#include "pch.h"
#include "TilemapChunkArray.h"

#include "tmxlite/Tileset.hpp"
#include "Windows/DX/Shape.h"
#include "Windows/DX/Texture.h"

TilemapChunkArray::TilemapChunkArray(Texture* texture, const tmx::Tileset& kTileset) :
    texture_(texture),
    shape_(nullptr)
{
    Math::Vector2 texture_size(
        static_cast<float>(texture->GetWidth()),
        static_cast<float>(texture->GetHeight())
    );

    tileset_size_.x = static_cast<float>(kTileset.getTileSize().x);
    tileset_size_.y = static_cast<float>(kTileset.getTileSize().y);

    tile_count_.x = texture_size.x / tileset_size_.x;
    tile_count_.y = texture_size.y / tileset_size_.y;

    first_gid_ = kTileset.getFirstGID();
    last_gid_ = kTileset.getLastGID();
}

void TilemapChunkArray::SetShape(const std::vector<DefaultVertex>& kVertices, const std::vector<Type::uint32>& kIndices)
{
    shape_ = std::make_shared<Shape>();
    shape_->SetVertices(kVertices);
    shape_->SetIndices(kIndices);
    shape_->SetTexture(texture_);

    World::Get()->AddShape(shape_);
}

Math::Vector2 TilemapChunkArray::GetTextureSize()
{
    return {static_cast<float>(texture_->GetWidth()), static_cast<float>(texture_->GetHeight())};
}
