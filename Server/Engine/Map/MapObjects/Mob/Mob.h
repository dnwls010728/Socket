#pragma once

#include "FunctionMacros.h"
#include "Engine/Map/MapObject.h"
#include "FSM/StateMachine.h"
#include "Misc/Function.h"

struct MobData;
class MobHitState;
class MobWalkState;
class MobIdleState;

class Mob : public MapObject
{
private:
    Function<void(uint32_t)> death_event_;
    
public:
    Mob(const MobData& mob_data);
    virtual ~Mob() override = default;

    virtual void SendSpawn(const std::shared_ptr<PlayerCharacter>& player) override;

    inline uint32_t GetMobID() const { return mob_id_; }

    inline void SetFlipped(bool is_flipped) { is_flipped_ = is_flipped; }
    inline bool IsFlipped() const { return is_flipped_; }

    inline void SetAnimation(const std::wstring& animation) { animation_ = animation; }
    inline const std::wstring& GetAnimation() const { return animation_; }
    
    inline const Math::Vector2& GetVelocity() const { return velocity_; }
    inline void SetVelocity(const Math::Vector2& velocity) { velocity_ = velocity; }
    
    inline void SetVelocityX(float x) { velocity_.x = x; }
    inline float GetVelocityX() const { return velocity_.x; }
    
    inline void SetVelocityY(float y) { velocity_.y = y; }
    inline float GetVelocityY() const { return velocity_.y; }
    
    inline void SetLastPosition(const Math::Vector2& last_position) { last_position_ = last_position; }
    
    inline float GetMoveSpeed() const { return move_speed_; }
    
    inline std::shared_ptr<MobIdleState> GetIdleState() const { return idle_state_; }
    inline std::shared_ptr<MobWalkState> GetWalkState() const { return walk_state_; }
    inline std::shared_ptr<MobHitState> GetHitState() const { return hit_state_; }

    DEFINE_BIND_OVERLOADS(death_event_, OnDeath, void, uint32_t)

protected:
    friend class Map;

    virtual void BeginPlay() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;

    void SendPositionPacket(const Math::Vector2& position, bool time_update = false) const;
    void SendAnimationPacket(const std::wstring& animation, bool is_flip, bool instant_play = false) const;
    void TakeDamage(uint32_t attacker, uint32_t damage);

    uint32_t mob_id_;
    uint32_t damage_;

    std::unique_ptr<FSM::StateMachine> state_machine_;

    Math::Vector2 velocity_;
    Math::Vector2 last_position_;

    float gravity_;
    float move_speed_;

    bool is_grounded_;
    bool was_moving_;
    
    bool is_flipped_;
    bool last_flipped_;

    std::wstring animation_;
    std::wstring last_animation_;

    class Foothold* foothold_;

    std::atomic_uint32_t hp_;

    int32_t exp_;

#pragma region 상태
    std::shared_ptr<MobIdleState> idle_state_;
    std::shared_ptr<MobWalkState> walk_state_;
    std::shared_ptr<MobHitState> hit_state_;
#pragma endregion
};
