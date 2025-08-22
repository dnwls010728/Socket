#include "pch.h"
#include "Tilemap.h"

#include <format>

#include "Data/FileHelper.h"
#include "Misc/StringHelper.h"

Tilemap::Tilemap() :
    map_(),
    unique_id_(0),
    bgm_(L""),
    name_(L""),
    ppu_(0.f),
    ui_sprite_(nullptr)
{
}

bool Tilemap::Load(const std::wstring& kPath)
{
    Asset::Load(kPath);
    
    if (!map_.load(StringHelper::UTF16ToUTF8(kPath))) return false;

    const auto& properties = map_.getProperties();
    if (properties.size() < 4) return false;

    bgm_ = StringHelper::UTF8ToUTF16(properties[0].getStringValue());
    name_ = StringHelper::UTF8ToUTF16(properties[1].getStringValue());

    ppu_ = properties[2].getFloatValue();
    unique_id_ = properties[3].getIntValue();

    ui_sprite_ = std::make_unique<UISprite>();
    
    std::wstring texture_path = L".\\Content\\" + FileHelper::GetRelativePath(kPath) + L".png";
    ui_sprite_->Load(texture_path);
    
    return true;
}

Math::Vector2 Tilemap::GetMapSize() const
{
    return {
        static_cast<float>(map_.getTileCount().x),
        static_cast<float>(map_.getTileCount().y)
    };
}

Math::Vector2 Tilemap::GetTileSize() const
{
    return {
        static_cast<float>(map_.getTileSize().x),
        static_cast<float>(map_.getTileSize().y)
    };
}

Bounds Tilemap::GetWorldBounds() const
{
    Math::Vector2 map_size = GetMapSize();
    Math::Vector2 tile_size = GetTileSize();

    float width = (map_size.x * tile_size.x) / ppu_;
    float height = (map_size.y * tile_size.y) / ppu_;

    return { {0.f, 0.f}, {width, height} };
}

RTTR_REGISTRATION
{
    using namespace rttr;
    
    registration::class_<Tilemap>("Tilemap")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
