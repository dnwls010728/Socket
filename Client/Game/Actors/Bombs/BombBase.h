#pragma once
#include "Actor/Actor.h"

class BombBase : public Actor
{
    SHADER_CLASS_HELPER(BombBase)
    GENERATED_BODY(BombBase, Actor)
    
public:
    BombBase(const std::wstring& kName);
    virtual ~BombBase() override = default;

protected:
    virtual void BeginPlay() override;
    
    virtual void OnBoom();

#pragma region COMPONENTS
    class SpriteRendererComponent* renderer_;
#pragma endregion
    
    float explosion_time_;

    TimerHandle timer_handle_;
    
};
