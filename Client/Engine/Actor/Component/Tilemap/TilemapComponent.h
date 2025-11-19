#pragma once
#include "TilemapLayer.h"
#include "Actor/Component/ActorComponent.h"
#include "box2d/id.h"
#include "Math/Vector2.h"
#include "Math/Vector2i.h"
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

    int32_t GetType(b2ShapeId shape_id);

    Math::Vector2i WorldToCell(const Math::Vector2& position) const;

    Math::Vector2 GetCellCenter(const Math::Vector2i& position) const;

    FORCEINLINE const Math::Vector2& GetMapSize() const { return map_size_; }

protected:
    virtual void UninitializeComponent() override;
    virtual void BeginPlay() override;
    virtual void EndPlay(EndPlayReason type) override;
    virtual void Render(float alpha) override;
    virtual void OnEnable() override;
    virtual void OnDisable() override;

private:
    void GeneratePhysics(const tmx::ObjectGroup& kObject);
    void GenerateSpawn(const tmx::ObjectGroup& kObject) const;
    void GeneratePortal(const tmx::ObjectGroup& kObject) const;
    void GenerateProp(const tmx::ObjectGroup& kObject) const;
    void GenerateBounds();
    
    float ppu_;

    Tilemap* tilemap_;

    Math::Vector2 map_size_;
    Math::Vector2 tile_size_;

    std::vector<std::unique_ptr<TilemapLayer>> tilemap_layers_;

    b2BodyId tilemap_body_id_;
    b2BodyId bounds_body_id_;

    std::unordered_map<uint64_t, int32_t> type_map_;
    
};
