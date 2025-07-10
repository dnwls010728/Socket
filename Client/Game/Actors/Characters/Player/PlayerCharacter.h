#pragma once
#include <CommonObject.h>
#include <deque>

#include "Actors/Characters/CharacterBase.h"
#include "Actors/Interfaces/IDamageable.h"
#include "Utils/TimedBool.h"

class PlayerCharacter : public CharacterBase, public IDamageable
{
    SHADER_CLASS_HELPER(PlayerCharacter)
    GENERATED_BODY(PlayerCharacter, CharacterBase)

    struct MovementSnapshot
    {
        Math::Vector2 position;
        Math::Vector2 velocity;
        float         server_time;
        bool          time_update;
    };
    
    struct AnimationSnapshot
    {
        bool          is_flipped;
        std::wstring  animation;
        float         server_time;
    };
    
public:
    PlayerCharacter(const std::wstring& kName);
    virtual ~PlayerCharacter() override = default;

    virtual void ReceivePacket(Net::IPacket* packet) override;
    virtual void TakeDamage(uint32_t updated_hp, uint32_t damage_amount, float server_time) override;

    void Init(const std::wstring& name, const Math::Vector2& position);
    void UpdateFlip();
    
    FORCEINLINE const Math::Vector2& GetMoveAxis() const { return move_axis_; }
    
    FORCEINLINE float GetMoveAxisX() const { return move_axis_.x; }
    FORCEINLINE float GetMoveAxisY() const { return move_axis_.y; }

protected:
    virtual void BeginPlay() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;
    
    virtual void SyncCharacterMovement(float delta_time);

    Math::Vector2 move_axis_;
    Math::Vector2 last_position_;
    std::wstring  last_animation_;
    bool          last_flip_;
    
    bool was_moving_;
    bool is_jump_pressed_;
    
    std::deque<MovementSnapshot>  movement_snapshots_;
    std::deque<AnimationSnapshot> animation_snapshots_;
    
    float movement_sync_accumulator_;
    float invincible_time_;
    
};
