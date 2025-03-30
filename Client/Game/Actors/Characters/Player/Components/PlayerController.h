#pragma once
#include "Actor/Component/ActorComponent.h"
#include "Math/Vector2.h"

class PlayerController : public ActorComponent
{
    SHADER_CLASS_HELPER(PlayerController)
    GENERATED_BODY(PlayerController, ActorComponent)
    
public:
    PlayerController(Actor* owner, const std::wstring& kName);
    virtual ~PlayerController() override = default;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float delta_time) override;

    Math::Vector2 movement_input_;

    class PlayerCharacter* player_character_;
    
};
