#include "pch.h"
#include "SpriteRendererComponent.h"

#include "TransformComponent.h"
#include "Actor/Actor.h"
#include "Level/World.h"
#include "Windows/DX/Shape.h"
#include "Windows/DX/Sprite.h"

SpriteRendererComponent::SpriteRendererComponent(Actor* owner, const std::wstring& kName) :
    ActorComponent(owner, kName),
    shape_(nullptr),
    sprite_(nullptr),
    frame_index_(0),
    flip_x_(false),
    flip_y_(false),
    color_(Math::Color::White),
    z_order_(0)
{
}

void SpriteRendererComponent::SetZOrder(int z_order)
{
    z_order_ = z_order;
    if (HasBegunPlay())
    {
        shape_->SetZOrder(z_order_);
        World::Get()->SortZOrder();
    }
}

void SpriteRendererComponent::InitializeComponent()
{
    ActorComponent::InitializeComponent();
    if (!sprite_) return;

    shape_ = std::make_shared<Shape>();
    shape_->SetVertices(sprite_->GetVertices());
    shape_->SetIndices(sprite_->GetIndices());
    shape_->SetTexture(sprite_);
    shape_->SetZOrder(z_order_);
    
    World::Get()->AddShape(shape_);
}

void SpriteRendererComponent::UninitializeComponent()
{
    ActorComponent::UninitializeComponent();

    World::Get()->RemoveShape(shape_);
}

void SpriteRendererComponent::Render(float alpha)
{
    ActorComponent::Render(alpha);
    if (!sprite_) return;

    const TransformComponent* transform = GetOwner()->GetTransform();
    if (!transform) return;

    const std::vector<SpriteFrame>& frames = sprite_->GetFrames();
    if (frames.empty() || frame_index_ >= frames.size()) return;

    const SpriteFrame& current_frame = frames[frame_index_];

    const float width = (sprite_->GetWidth() * current_frame.uv_scale.x / sprite_->GetPPU()) * transform->GetScale().x;
    const float height = (sprite_->GetHeight() * current_frame.uv_scale.y / sprite_->GetPPU()) * transform->GetScale().y;

    const float pivot_x = current_frame.pivot.x * width;
    const float pivot_y = current_frame.pivot.y * height;

    const int flip_x = flip_x_ ? -1 : 1;
    const int flip_y = flip_y_ ? -1 : 1;

    shape_->SetPosition(transform->GetPosition());
    shape_->SetRotation(transform->GetAngle());
    shape_->SetScale({width * flip_x, height * flip_y});
    // shape_->SetUVOffset(current_frame.uv_offset);
    // shape_->SetUVScale(current_frame.uv_scale);
    shape_->SetColor(color_);
    shape_->SetPivot({pivot_x, pivot_y});
}

void SpriteRendererComponent::OnEnable()
{
    ActorComponent::OnEnable();

    World::Get()->AddShape(shape_);
}

void SpriteRendererComponent::OnDisable()
{
    ActorComponent::OnDisable();

    World::Get()->RemoveShape(shape_);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<SpriteRendererComponent>("SpriteRendererComponent")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
