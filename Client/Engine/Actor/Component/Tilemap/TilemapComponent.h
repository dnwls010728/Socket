#pragma once
#include "TilemapLayer.h"
#include "Actor/Component/ActorComponent.h"
#include "box2d/id.h"
#include "Math/Vector2.h"
#include "tmxlite/Map.hpp"

class Tilemap;

namespace tmx
{
    class ObjectGroup;
}

class Shape;

class TilemapComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(TilemapComponent)
    GENERATED_BODY(TilemapComponent, ActorComponent)
    
public:
    TilemapComponent(Actor* owner, const std::wstring& kName);

    void SetTilemap(Tilemap* tilemap);

    int GetType(b2ShapeId shape_id);

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(EndPlayReason type) override;
    virtual void Render(float alpha) override;

private:
    void GeneratePhysics(const tmx::ObjectGroup& kObject);
    void GenerateSpawn(const tmx::ObjectGroup& kObject);
    
    float ppu_;

    Tilemap* tilemap_;

    Math::Vector2 map_size_;

    std::vector<std::unique_ptr<TilemapLayer>> tilemap_layers_;

    b2BodyId tilemap_body_id_;

    std::unordered_map<Type::uint64, int> type_map_;
    
};
