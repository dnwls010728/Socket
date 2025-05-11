#pragma once
#include "ActorLayer.h"
#include "Singleton.h"

class Level;

class EngineSettings : public Singleton<EngineSettings>
{
public:
    EngineSettings();
    virtual ~EngineSettings() override = default;
    
    FORCEINLINE void SetFixedTimeStep(float fixed_time_step) { fixed_time_step_ = fixed_time_step; }
    FORCEINLINE float GetFixedTimeStep() const { return fixed_time_step_; }

    FORCEINLINE void AddCollisionLayer(ActorLayer layer, ActorLayer collision) { layer_collision_matrix_[layer] = collision; }
    FORCEINLINE ActorLayer GetCollisionLayer(ActorLayer layer) const { return layer_collision_matrix_.at(layer); }

private:
    
    float fixed_time_step_;
    
    std::unordered_map<ActorLayer, ActorLayer> layer_collision_matrix_;
};
