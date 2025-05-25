#include "pch.h"
#include "ItemDrop.h"

#include "Characters/Components/Controller2DComponent.h"

ItemDrop::ItemDrop(const std::wstring& name) :
    NetworkActor(name),
    gravity_(-20.f),
    velocity_(Math::Vector2::Zero())
{
    controller_ = AddComponent<Controller2DComponent>(L"Controller2D");
    
}

void ItemDrop::Tick(float delta_time)
{
    NetworkActor::Tick(delta_time);
    
    const Controller2DComponent::CollisionInfo& collisions = controller_->GetCollisions();
    
    controller_->Move(velocity_ * delta_time);
    
    if (collisions.is_above || collisions.is_below) velocity_.y = 0.f;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<ItemDrop>("ItemDrop")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
