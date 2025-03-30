#pragma once
#include "Actor/Actor.h"

class BoomBase : public Actor
{
    SHADER_CLASS_HELPER(BoomBase)
    GENERATED_BODY(BoomBase, Actor)
    
public:
    BoomBase(const std::wstring& kName);
    virtual ~BoomBase() override = default;

protected:
    virtual void BeginPlay() override;
    
#pragma region COMPONENTS
    class SpriteRendererComponent* renderer_;
#pragma endregion
    
};
