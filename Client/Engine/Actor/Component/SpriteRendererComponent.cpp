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
    frame_(std::nullopt),
    frame_width_(0.f),
    frame_height_(0.f),
    flip_x_(false),
    flip_y_(false),
    color_(Math::Color::White),
    z_order_(0)
{
}

void SpriteRendererComponent::SetColor(const Math::Color& color)
{
    color_ = color;
    if (HasBegunPlay())
        shape_->SetColor(color_);
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
    sprite_ = sprite;
    if (!sprite) return;
    
    const auto& frame_indexes = sprite->GetFrameIndexes();

    auto it = frame_indexes.find(kFrame);
    if (it != frame_indexes.end())
        frame_index_ = it->second;
    else frame_index_ = 0;

    if (shape_)
    {
        shape_->SetVertices(sprite_->GetVertices());
        shape_->SetIndices(sprite_->GetIndices());
        shape_->SetTexture(sprite_);
    }

    UpdateFrame();
}

void SpriteRendererComponent::SetSprite(Sprite* sprite, uint64_t frame_index)
{
    sprite_ = sprite;
    frame_index_ = frame_index;

    if (shape_)
    {
        shape_->SetVertices(sprite_->GetVertices());
        shape_->SetIndices(sprite_->GetIndices());
        shape_->SetTexture(sprite_);
    }
    
    UpdateFrame();
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

    shape_->SetColor(color_);
    shape_->SetZOrder(z_order_);
    
    if (frame_)
    {
        const auto& frame = frame_.value();
        shape_->SetUVOffset(frame.uv_offset);
        shape_->SetUVScale(frame.uv_scale);
    }
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

    const auto& transform = GetOwner()->GetTransform();
    if (!transform) return;

    if (!frame_) return;
    const auto& frame = frame_.value();

    const auto& scale = transform->GetScale();
    float scaled_width = frame_width_ * scale.x;
    float scaled_height = frame_height_ * scale.y;

    const float pivot_x = frame.pivot.x * scaled_width;
    const float pivot_y = frame.pivot.y * scaled_height;

    const int32_t flip_x = flip_x_ ? -1 : 1;
    const int32_t flip_y = flip_y_ ? -1 : 1;

    shape_->SetPosition(transform->GetPosition());
    shape_->SetAngle(transform->GetAngle());
    shape_->SetScale({scaled_width * flip_x, scaled_height * flip_y});
    shape_->SetPivot({pivot_x, pivot_y});
}

void SpriteRendererComponent::OnEnable()
{
    ActorComponent::OnEnable();
    
    const auto& transform = GetOwner()->GetTransform();
    shape_->SetPosition(transform->GetPosition());
    shape_->SetAngle(transform->GetAngle());

    World::Get()->AddShape(shape_);
}

void SpriteRendererComponent::OnDisable()
{
    ActorComponent::OnDisable();

    World::Get()->RemoveShape(shape_);
}

void SpriteRendererComponent::UpdateFrame()
{
    const auto& frames = sprite_->GetFrames();
    if (frames.empty()) return;

    const auto& frame = frames[frame_index_ % frames.size()];
    frame_ = frame;

    uint32_t ppu = sprite_->GetPPU();
    frame_width_ = sprite_->GetWidth() * frame.uv_scale.x / ppu;
    frame_height_ = sprite_->GetHeight() * frame.uv_scale.y / ppu;

    if (HasBegunPlay())
    {
        shape_->SetUVOffset(frame.uv_offset);
        shape_->SetUVScale(frame.uv_scale);
    }
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
