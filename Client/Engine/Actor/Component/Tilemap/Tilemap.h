#pragma once
#include "Asset/Asset.h"
#include "tmxlite/Map.hpp"
#include "Windows/DX/UISprite.h"

class Tilemap : public Asset
{
    GENERATED_BODY(Tilemap, Asset)
    
public:
    Tilemap();
    virtual ~Tilemap() override = default;

    virtual bool Load(const std::wstring& kPath) override;

    Math::Vector2 GetMapSize() const;
    Math::Vector2 GetTileSize() const;

    Bounds GetWorldBounds() const;

    FORCEINLINE const tmx::Map& GetMap() const { return map_; }
    FORCEINLINE uint32_t GetUniqueID() const { return unique_id_; }
    FORCEINLINE const std::wstring& GetName() const { return name_; }
    FORCEINLINE float GetPPU() const { return ppu_; }
    FORCEINLINE UISprite* GetUISprite() const { return ui_sprite_.get(); }

private:
    tmx::Map map_;
    uint32_t unique_id_;
    std::wstring name_;
    float ppu_;
    std::unique_ptr<UISprite> ui_sprite_;
    
};
