#pragma once
#include "Actor/Actor.h"

class CharacterBase : public Actor
{
    SHADER_CLASS_HELPER(CharacterBase)
    GENERATED_BODY(CharacterBase, Actor)
    
public:
    CharacterBase(const std::wstring& kName);
    virtual ~CharacterBase() override = default;

protected:
#pragma region 컴포넌트
    std::shared_ptr<class SpriteRendererComponent> renderer_;
#pragma endregion
    
};
