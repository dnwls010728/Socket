#pragma once
#include "Character/Actor/CharacterBase.h"
#include "Actor/Character/Player/PlayerCharacter.h"
#include "Actor/Component/TransformComponent.h"
#include "Level/World.h"

class AnimatorComponent;
class PlayerLocomotion;

class PlayerCharacter : public CharacterBase
{
    SHADER_CLASS_HELPER(PlayerCharacter)
    GENERATED_BODY(PlayerCharacter, CharacterBase)
    
public:
    PlayerCharacter(const std::wstring& kName);
    virtual ~PlayerCharacter() override = default;

    FORCEINLINE AnimatorComponent* GetAnimator() const { return animator_; }
    
    FORCEINLINE int GetHorizontalAxis() const { return horizontal_axis_; }

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(EndPlayReason type) override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;
    virtual void PostTick(float delta_time) override;

private:
    std::shared_ptr<PlayerLocomotion> locomotion_state_;
    
    AnimatorComponent* animator_;
    class StateMachine* state_machine_;
    
    class AnimationPack* animation_pack_;
    
    int horizontal_axis_;
    
    float move_speed_;

    Math::Vector2 previous_position_;

    class Weapon* weapon_;

    class Audio* audio_;
    
};
