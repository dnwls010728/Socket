#pragma once
#include <CommonObject.h>
#include "Actors/Characters/CharacterBase.h"
#include "Actors/Interfaces/IDamageable.h"
#include "Math/Color.h"

class PlayerCharacter : public CharacterBase
{
    SHADER_CLASS_HELPER(PlayerCharacter)
    GENERATED_BODY(PlayerCharacter, CharacterBase)

public:
    PlayerCharacter(const std::wstring& kName);
    virtual ~PlayerCharacter() override = default;

    virtual void ReceivePacket(Net::IPacket* packet) override;
    virtual void TakeDamage(const std::vector<DamageInfo>& damage_amount, float server_time) override;

    void Init(const std::wstring& name, const std::wstring& body_color, const Math::Vector2& position, int8_t gm_level = 0);
    void UpdateFlip() const;
    void SetDead();

    FORCEINLINE const Math::Vector2& GetMoveAxis() const { return move_axis_; }

    FORCEINLINE float GetMoveAxisX() const { return move_axis_.x; }
    FORCEINLINE float GetMoveAxisY() const { return move_axis_.y; }

    // 테스트
    std::unordered_map<int32_t, float> buff_effects_;

protected:
    virtual void BeginPlay() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;
    
    virtual void StartCreateParty();
    
    virtual void SyncCharacterMovement(float delta_time);
    
    void OnFootstep() const;

    Math::Vector2 move_axis_;
    Math::Vector2 last_position_;
    std::wstring last_animation_;

    bool was_grounded_;
    bool last_flip_;
    bool was_moving_;
    bool is_dead_;

    uint32_t party_id_;
    
    float movement_sync_accumulator_;
    float invincible_time_;

    Math::Color color_;
    
    int32_t bonus_jumps_;
};
