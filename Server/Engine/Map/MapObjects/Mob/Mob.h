#pragma once
#include "Engine/Map/MapObject.h"
#include "FSM/StateMachine.h"

class Mob : public MapObject
{
public:
    Mob();
    virtual ~Mob() override = default;

protected:
    friend class Map;
    
    virtual void Tick(float delta_time) override;

    std::unique_ptr<FSM::StateMachine> state_machine_;

    Math::Vector2 velocity_;
    Math::Vector2 last_position_;

    float gravity_;

    bool is_grounded_;
    
};
