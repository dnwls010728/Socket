#pragma once
#include "TilemapLayer.h"
#include "Actor/Component/ActorComponent.h"
#include "box2d/id.h"
#include "Math/Vector2.h"
#include "tmxlite/Map.hpp"

namespace tmx
{
    class ObjectGroup;
}

class Shape;

class TilemapComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(TilemapComponent)
    GENERATED_BODY(TilemapComponent, ActorComponent)
    
    const float PPU = 32.f;
    
public:
    TilemapComponent(Actor* owner, const std::wstring& kName);

    void LoadMap(const char* kPath);

protected:
    virtual void InitializeComponent() override;
    virtual void UninitializeComponent() override;
    virtual void Render(float alpha) override;

private:
    void GeneratePhysics(const tmx::ObjectGroup& kObject);
    void GenerateSpawn(const tmx::ObjectGroup& kObject);

    tmx::Map map_;
    Math::Vector2 map_size_;

    std::vector<std::unique_ptr<TilemapLayer>> tilemap_layers_;

    b2BodyId tilemap_body_id_;
    
};
