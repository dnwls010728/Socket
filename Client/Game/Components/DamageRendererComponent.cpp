#include "pch.h"
#include "DamageRendererComponent.h"

#include "Actor/Component/TransformComponent.h"
#include "Asset/AssetManager.h"
#include "DirectXTK/SimpleMath.h"
#include "Math/Math.h"
#include "Windows/DX/Shape.h"
#include "Windows/DX/Sprite.h"

DamageRendererComponent::DamageRendererComponent(Actor* owner, const std::wstring& name) :
    ActorComponent(owner, name),
    damage_(0),
    vertices_(),
    indices_(),
    shape_(nullptr),
    damage_sprite_(nullptr),
    miss_sprite_(nullptr),
    color_(Math::Color::White),
    is_dirty_(false)
{
}

void DamageRendererComponent::SetDamage(uint64_t damage)
{
    damage_ = damage;
    is_dirty_ = true;
}

void DamageRendererComponent::SetColor(const Math::Color& color)
{
    color_ = color;
    if (HasBegunPlay())
        shape_->SetColor(color_);
}

void DamageRendererComponent::InitializeComponent()
{
    ActorComponent::InitializeComponent();

    shape_ = std::make_shared<Shape>();
    shape_->SetColor(color_);
    shape_->SetZOrder(std::numeric_limits<int32_t>::max());
}

void DamageRendererComponent::UninitializeComponent()
{
    ActorComponent::UninitializeComponent();

    World::Get()->RemoveShape(shape_);
}

void DamageRendererComponent::TickComponent(float delta_time)
{
    ActorComponent::TickComponent(delta_time);

    if (is_dirty_)
    {
        RebuildGridGeometry();
        shape_->SetVertices(vertices_);
        shape_->SetIndices(indices_);
        shape_->SetTexture(damage_ > 0 ? damage_sprite_ : miss_sprite_);
        is_dirty_ = false;
    }
}

void DamageRendererComponent::Render(float alpha)
{
    ActorComponent::Render(alpha);
    if (!damage_sprite_ && !miss_sprite_) return;

    const auto& transform = GetOwner()->GetTransform();
    if (!transform) return;

    Bounds bounds = shape_->GetBounds();

    const auto& scale = transform->GetScale();
    float width = scale.x;
    float height = scale.y;
    
    float pivot_x = bounds.size.x * .5f;
    float pivot_y = bounds.size.y * .5f;

    if (damage_ == 0)
    {
        const SpriteFrame& frame = miss_sprite_->GetFrames()[0];
        width = (miss_sprite_->GetWidth() * frame.uv_scale.x / miss_sprite_->GetPPU()) * scale.x;
        height = (miss_sprite_->GetHeight() * frame.uv_scale.y / miss_sprite_->GetPPU()) * scale.y;

        pivot_x = frame.pivot.x * width;
        pivot_y = frame.pivot.y * height;
    }

    shape_->SetPosition(transform->GetPosition());
    shape_->SetAngle(transform->GetAngle());
    shape_->SetScale({width, height});
    shape_->SetPivot({pivot_x, pivot_y});
}

void DamageRendererComponent::OnEnable()
{
    ActorComponent::OnEnable();
    
    const auto& transform = GetOwner()->GetTransform();
    shape_->SetPosition(transform->GetPosition());
    shape_->SetAngle(transform->GetAngle());

    World::Get()->AddShape(shape_);
}

void DamageRendererComponent::OnDisable()
{
    ActorComponent::OnDisable();

    World::Get()->RemoveShape(shape_);
}

void DamageRendererComponent::RebuildGridGeometry()
{
    vertices_.clear();
    indices_.clear();

    std::wstring damage_string = std::to_wstring(damage_);
    if (damage_ > 0)
    {
        if (!damage_sprite_) return;

        const auto& frames = damage_sprite_->GetFrames();
        for (uint64_t i = 0; i < damage_string.length(); ++i)
        {
            uint64_t frame_index = damage_string[i] - L'0';
            const SpriteFrame& frame = frames[frame_index];

            const float width = damage_sprite_->GetWidth() * frame.uv_scale.x / damage_sprite_->GetPPU();
            const float height = damage_sprite_->GetHeight() * frame.uv_scale.y / damage_sprite_->GetPPU();

            DefaultVertex v0 = {
                {i * width, 0.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {frame.uv_offset.x, frame.uv_offset.y + frame.uv_scale.y}
            };
            DefaultVertex v1 = {
                {i * width + width, 0.f, 0.f}, {1.f, 1.f, 1.f, 1.f},
                {frame.uv_offset.x + frame.uv_scale.x, frame.uv_offset.y + frame.uv_scale.y}
            };
            DefaultVertex v2 = {{i * width, height, 0.f}, {1.f, 1.f, 1.f, 1.f}, {frame.uv_offset.x, frame.uv_offset.y}};
            DefaultVertex v3 = {
                {i * width + width, height, 0.f}, {1.f, 1.f, 1.f, 1.f},
                {frame.uv_offset.x + frame.uv_scale.x, frame.uv_offset.y}
            };

            uint32_t base_index = static_cast<uint32_t>(vertices_.size());

            vertices_.push_back(v0);
            vertices_.push_back(v1);
            vertices_.push_back(v2);
            vertices_.push_back(v3);
            
            indices_.push_back(base_index);
            indices_.push_back(base_index + 1);
            indices_.push_back(base_index + 2);
            indices_.push_back(base_index + 2);
            indices_.push_back(base_index + 1);
            indices_.push_back(base_index + 3);
        }
    }
    else
    {
        if (!miss_sprite_) return;

        vertices_ = miss_sprite_->GetVertices();
        indices_ = miss_sprite_->GetIndices();
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<DamageRendererComponent>("DamageRendererComponent")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
