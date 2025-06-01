#pragma once
#include "Actors/NetworkActor.h"
#include "Actors/ServerActor.h"

class AnimatorComponent;

class MobBase : public ServerActor
{
    SHADER_CLASS_HELPER(MobBase)
    GENERATED_BODY(MobBase, NetworkActor)
    
public:
    MobBase(const std::wstring& name);
    virtual ~MobBase() override = default;

protected:
#pragma region 컴포넌트
    std::shared_ptr<BoxColliderComponent> collider_;
    std::shared_ptr<SpriteRendererComponent> renderer_;
    std::shared_ptr<AnimatorComponent> animator_;
#pragma endregion
    
};
