#pragma once
#include "Subsystem/Tickable.h"
#include "Subsystem/WorldSubsystem.h"

class BlockPreview;
class Grid;

class PlacementSubsystem : public WorldSubsystem, public Tickable
{
    GENERATED_BODY(PlacementSubsystem, WorldSubsystem)
    
public:
    PlacementSubsystem();
    virtual ~PlacementSubsystem() override = default;

    virtual void OnWorldBeginPlay() override;
    virtual void Tick(float delta_time) override;
    
    void StartPlacement();
    void StopPlacement();
    
    FORCEINLINE void SetTilemapComponent(const std::shared_ptr<class TilemapComponent>& tilemap) { tilemap_component_ = tilemap; }
    FORCEINLINE bool IsPlacing() const { return is_placing_; }

    static PlacementSubsystem* Get();

private:
    std::shared_ptr<Grid> grid_;
    std::shared_ptr<BlockPreview> block_preview_;

    std::shared_ptr<TilemapComponent> tilemap_component_;

    bool is_placing_;
    
};
