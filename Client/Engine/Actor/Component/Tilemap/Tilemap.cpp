#include "pch.h"
#include "Tilemap.h"

Tilemap::Tilemap() :
    map_()
{
}

bool Tilemap::Load(const std::wstring& kPath)
{
    Asset::Load(kPath);
    
    const std::string kFinalPath(kPath.begin(), kPath.end());
    if (!map_.load(kFinalPath)) return false;

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

RTTR_REGISTRATION
{
    using namespace rttr;
    
    registration::class_<Tilemap>("Tilemap")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
