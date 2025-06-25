#pragma once
#include "Engine/Map/MapObject.h"
#include "FSM/StateMachine.h"

class Mob : public MapObject
{
public:
    Mob();
    virtual ~Mob() override = default;

    inline void SetLastPostion(Math::Vector2 last_position){last_position_ =  last_position; }
protected:
    friend class Map;
    
    virtual void Tick(float delta_time) override;
    virtual void PhysicsTick(float delta_time) override;

    void OnHit(int32_t damage);

    std::unique_ptr<FSM::StateMachine> state_machine_;

    Math::Vector2 velocity_;
    Math::Vector2 last_position_;

    float gravity_;
    float timer_;

    int32_t direction_;

    bool is_grounded_;
    bool prev_is_moving_;

    class Foothold* foothold_;

    std::atomic_int32_t hp_;

    // 임시
    bool is_flipped_;
    std::wstring animation_;
    
};
