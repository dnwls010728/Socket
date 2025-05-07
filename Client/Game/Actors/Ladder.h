#pragma once
#include "Actor/Actor.h"

class Ladder : public Actor
{
    SHADER_CLASS_HELPER(Ladder)
    GENERATED_BODY(Ladder, Actor)
    
public:
    Ladder(const std::wstring& kName);
    virtual ~Ladder() override = default;

private:
#pragma region 컴포넌트
    std::shared_ptr<BoxColliderComponent> collider_;
#pragma endregion
    
};
