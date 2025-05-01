#include "pch.h"
#include "Tilemap.h"

#include <format>

#include "Data/FileHelper.h"

Tilemap::Tilemap() :
    map_(),
    unique_id_(0),
    name_(L""),
    ppu_(0.f),
    ui_texture_(nullptr)
{
}

bool Tilemap::Load(const std::wstring& kPath)
{
    Asset::Load(kPath);
    
    const std::string kFinalPath(kPath.begin(), kPath.end());
    if (!map_.load(kFinalPath)) return false;

    const std::vector<tmx::Property> properties = map_.getProperties();
    
    std::string name_str = properties[0].getStringValue();
    name_ = std::wstring(name_str.begin(), name_str.end());

    ppu_ = properties[1].getFloatValue();
    unique_id_ = properties[2].getIntValue();

    ui_texture_ = std::make_unique<UITexture>();
    
    std::wstring texture_path = L".\\Content\\" + FileHelper::GetRelativePath(kPath) + L".png";
    ui_texture_->Load(texture_path);
    
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

Bounds Tilemap::GetWorldBounds()
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
