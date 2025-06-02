#include "pch.h"
#include "Ladder.h"

#include "Actor/Component/BoxColliderComponent.h"

Ladder::Ladder(const std::wstring& kName) :
    Actor(kName)
{
    collider_ = AddComponent<BoxColliderComponent>(L"BoxCollider");
    collider_->SetSize({1.f, 1.f});
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Ladder>("Ladder")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
