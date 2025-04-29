#pragma once
#include "Asset/Asset.h"
#include "tmxlite/Map.hpp"

class Tilemap : public Asset
{
    GENERATED_BODY(Tilemap, Asset)
    
public:
    Tilemap();
    virtual ~Tilemap() override = default;

    virtual bool Load(const std::wstring& kPath) override;

    Math::Vector2 GetMapSize() const;
    Math::Vector2 GetTileSize() const;

    FORCEINLINE const tmx::Map& GetMap() const { return map_; }

private:
    tmx::Map map_;
    
};
