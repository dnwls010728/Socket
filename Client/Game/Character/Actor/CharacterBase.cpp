#include "pch.h"
#include "CharacterBase.h"

#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"

CharacterBase::CharacterBase(const std::wstring& kName) :
    NetworkActor(kName)
{
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    collider_ = AddComponent<CapsuleColliderComponent>(L"CapsuleCollider");
    
    rigid_body_ = AddComponent<RigidBody2DComponent>(L"RigidBody2D");
    rigid_body_->UseAutoMass(false);
    rigid_body_->SetFreezeRotation(true);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<CharacterBase>("CharacterBase")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
