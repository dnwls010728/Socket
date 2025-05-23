#pragma once
#include "Asset/Asset.h"
#include "Math/Bounds.h"
#include "tmxlite/Map.hpp"
#include "Math/Vector2.h"

class Tilemap : public Asset
{
    GENERATED_BODY(Tilemap, Asset)
    
public:
    Tilemap();
    virtual ~Tilemap() override = default;

    virtual bool Load(const std::wstring& kPath) override;

    Math::Vector2 GetMapSize() const;
    Math::Vector2 GetTileSize() const;

    Bounds GetWorldBounds();

    FORCEINLINE const tmx::Map& GetMap() const { return map_; }
    FORCEINLINE uint32_t GetUniqueID() const { return unique_id_; }
    FORCEINLINE const std::wstring& GetName() const { return name_; }
    FORCEINLINE float GetPPU() const { return ppu_; }

private:
    tmx::Map map_;
    uint32_t unique_id_;
    std::wstring name_;
    float ppu_;
    
};
