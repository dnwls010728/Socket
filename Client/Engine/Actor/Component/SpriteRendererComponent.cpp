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

void SpriteRendererComponent::SetZOrder(int32_t z_order)
{
    z_order_ = z_order;
    if (HasBegunPlay())
    {
        shape_->SetZOrder(z_order_);
        World::Get()->SortZOrder();
    }
}

void SpriteRendererComponent::SetSprite(Sprite* sprite, const std::wstring& kFrame)
{
    if (!sprite) return;
    sprite_ = sprite;
    
    const auto& frame_indexes = sprite->GetFrameIndexes();
    if (frame_indexes.empty()) return;

    auto it = frame_indexes.find(kFrame);
    if (it != frame_indexes.end()) frame_index_ = it->second;
    else frame_index_ = 0;

    if (shape_)
    {
        shape_->SetVertices(sprite_->GetVertices());
        shape_->SetIndices(sprite_->GetIndices());
        shape_->SetTexture(sprite_);
    }
}

Bounds SpriteRendererComponent::GetBounds() const
{
    if (!shape_) return Bounds();
    return shape_->GetBounds();
}

void SpriteRendererComponent::InitializeComponent()
{
    ActorComponent::InitializeComponent();

    shape_ = std::make_shared<Shape>();
    if (sprite_)
    {
        shape_->SetVertices(sprite_->GetVertices());
        shape_->SetIndices(sprite_->GetIndices());
        shape_->SetTexture(sprite_);
    }
    
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

    const std::shared_ptr<TransformComponent> transform = GetOwner()->GetTransform();
    if (!transform) return;

    const auto& frames = sprite_->GetFrames();
    if (frames.empty()) return;

    const SpriteFrame& current_frame = frames[frame_index_];

    const float width = (sprite_->GetWidth() * current_frame.uv_scale.x / sprite_->GetPPU()) * transform->GetScale().x;
    const float height = (sprite_->GetHeight() * current_frame.uv_scale.y / sprite_->GetPPU()) * transform->GetScale().y;

    const float pivot_x = current_frame.pivot.x * width;
    const float pivot_y = current_frame.pivot.y * height;

    const int32_t flip_x = flip_x_ ? -1 : 1;
    const int32_t flip_y = flip_y_ ? -1 : 1;

    shape_->SetPosition(transform->GetPosition());
    shape_->SetAngle(transform->GetAngle());
    shape_->SetScale({width * flip_x, height * flip_y});
    shape_->SetUVOffset(current_frame.uv_offset);
    shape_->SetUVScale(current_frame.uv_scale);
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
