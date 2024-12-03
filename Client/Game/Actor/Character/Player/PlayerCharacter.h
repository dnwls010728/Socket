#pragma once
#include "Actor/Character/CharacterBase.h"
#include "Actor/Character/Player/PlayerCharacter.h"
#include "Actor/Component/TransformComponent.h"
#include "Level/World.h"
class Text;


class PlayerCharacter : public CharacterBase
{
    SHADER_CLASS_HELPER(PlayerCharacter)
    GENERATED_BODY(PlayerCharacter, CharacterBase)
    
public:
    PlayerCharacter(const std::wstring& kName);
    virtual ~PlayerCharacter() override = default;
    

protected:
    virtual void BeginPlay() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;
    virtual void PostTick(float delta_time) override;

private:
    class Sprite* sprite_;
    
    int horizontal_axis_;
    
    float move_speed_;

    Math::Vector2 previous_position_;
    
};
