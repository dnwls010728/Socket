#include "pch.h"
#include "Dummy.h"

#include "Actor/Component/CircleColliderComponent.h"

Dummy::Dummy(const std::wstring& kName) :
    Actor(kName)
{
    collider_ = AddComponent<CircleColliderComponent>(L"CircleCollider");
    collider_->SetRadius(1.f);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Dummy>("Dummy")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
