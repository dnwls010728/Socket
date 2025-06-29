#pragma once
#include <CommonObject.h>

#include "Engine/Map/MapObject.h"
#include "FSM/StateMachine.h"

class MobHitState;
class MobWalkState;
class MobIdleState;

class Mob : public MapObject
{
public:
    Mob();
    virtual ~Mob() override = default;

    void SetState(MobState state);
    void SetFlipped(bool is_flipped);
    
    inline const Math::Vector2& GetVelocity() const { return velocity_; }
    inline void SetVelocity(const Math::Vector2& velocity) { velocity_ = velocity; }
    
    inline void SetVelocityX(float x) { velocity_.x = x; }
    inline float GetVelocityX() const { return velocity_.x; }
    
    inline void SetVelocityY(float y) { velocity_.y = y; }
    inline float GetVelocityY() const { return velocity_.y; }
    
    inline void SetLastPosition(const Math::Vector2& last_position) { last_position_ = last_position; }
    
    inline std::shared_ptr<MobIdleState> GetIdleState() const { return idle_state_; }
    inline std::shared_ptr<MobWalkState> GetWalkState() const { return walk_state_; }
    inline std::shared_ptr<MobHitState> GetHitState() const { return hit_state_; }

protected:
    friend class Map;

    virtual void BeginPlay() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;

    void OnHit(int32_t damage);

    std::unique_ptr<FSM::StateMachine> state_machine_;

    Math::Vector2 velocity_;
    Math::Vector2 last_position_;

    float gravity_;

    bool is_grounded_;
    bool prev_is_moving_;

    class Foothold* foothold_;

    std::atomic_int32_t hp_;

    uint16_t state_;

#pragma region 상태
    std::shared_ptr<MobIdleState> idle_state_;
    std::shared_ptr<MobWalkState> walk_state_;
    std::shared_ptr<MobHitState> hit_state_;
#pragma endregion
};
