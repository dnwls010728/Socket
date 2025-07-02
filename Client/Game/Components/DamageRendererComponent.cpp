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
    count_timer_(0.f),
    fade_timer_(0.f),
    alpha_(0.f)
{
}

void DamageRendererComponent::SetDamage(uint64_t damage)
{
    damage_ = damage;
    Refresh(damage_);
}

void DamageRendererComponent::InitializeComponent()
{
    ActorComponent::InitializeComponent();

    Refresh(damage_);

    shape_ = std::make_shared<Shape>();
    if (!vertices_.empty() && !indices_.empty())
    {
        shape_->SetVertices(vertices_);
        shape_->SetIndices(indices_);
    }

    shape_->SetTexture(damage_ > 0 ? damage_sprite_ : miss_sprite_);

    World::Get()->AddShape(shape_);
}

void DamageRendererComponent::UninitializeComponent()
{
    ActorComponent::UninitializeComponent();

    World::Get()->RemoveShape(shape_);
}

void DamageRendererComponent::TickComponent(float delta_time)
{
    ActorComponent::TickComponent(delta_time);
    
    count_timer_ += delta_time;
    if (count_timer_ >= 1.f)
    {
        count_timer_ = 0.f;
        Refresh(++damage_);
    }

    fade_timer_ += delta_time;
    if (fade_timer_ < 1.f)
    {
        float t = Math::Clamp(fade_timer_ / 1.f, 0.f, 1.f);
        alpha_ = 1.f - t;
    }
    else fade_timer_ = 0.f;

}

void DamageRendererComponent::Render(float alpha)
{
    ActorComponent::Render(alpha);
    if (!damage_sprite_ && !miss_sprite_) return;

    const std::shared_ptr<TransformComponent> transform = GetOwner()->GetTransform();
    if (!transform) return;

    const Bounds& bounds = shape_->GetBounds();

    float width = transform->GetScale().x;
    float height = transform->GetScale().y;
    
    float pivot_x = bounds.size.x * .5f;
    float pivot_y = bounds.size.y * .5f;

    if (damage_ == 0)
    {
        const SpriteFrame& frame = miss_sprite_->GetFrames()[0];
        width = (miss_sprite_->GetWidth() * frame.uv_scale.x / miss_sprite_->GetPPU()) * transform->GetScale().x;
        height = (miss_sprite_->GetHeight() * frame.uv_scale.y / miss_sprite_->GetPPU()) * transform->GetScale().y;

        pivot_x = frame.pivot.x * width;
        pivot_y = frame.pivot.y * height;
    }

    shape_->SetPosition(transform->GetPosition());
    shape_->SetAngle(transform->GetAngle());
    shape_->SetScale({width, height});
    shape_->SetPivot({pivot_x, pivot_y});
    shape_->SetColor({ 255, 255, 255, static_cast<uint8_t>(alpha_ * 255) });
    shape_->SetZOrder(std::numeric_limits<int32_t>::max());
}

void DamageRendererComponent::OnEnable()
{
    ActorComponent::OnEnable();

    World::Get()->AddShape(shape_);
}

void DamageRendererComponent::OnDisable()
{
    ActorComponent::OnDisable();

    World::Get()->RemoveShape(shape_);
}

void DamageRendererComponent::Refresh(uint64_t damage)
{
    vertices_.clear();
    indices_.clear();

    std::wstring damage_string = std::to_wstring(damage);
    if (damage > 0)
    {
        if (!damage_sprite_) return;

        const auto& frames = damage_sprite_->GetFrames();
        for (uint64_t i = 0; i < damage_string.length(); ++i)
        {
            uint64_t frame_index = damage_string[i] - L'0';
            const SpriteFrame& frame = frames[frame_index];

            const float width = damage_sprite_->GetWidth() * frame.uv_scale.x / damage_sprite_->GetPPU();
            const float height = damage_sprite_->GetHeight() * frame.uv_scale.y / damage_sprite_->GetPPU();

            DefaultVertex vertex = {
                {i * width, 0.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {frame.uv_offset.x, frame.uv_offset.y + frame.uv_scale.y}
            };
            vertices_.push_back(vertex);

            vertex = {
                {i * width + width, 0.f, 0.f}, {1.f, 1.f, 1.f, 1.f},
                {frame.uv_offset.x + frame.uv_scale.x, frame.uv_offset.y + frame.uv_scale.y}
            };
            vertices_.push_back(vertex);

            vertex = {{i * width, height, 0.f}, {1.f, 1.f, 1.f, 1.f}, {frame.uv_offset.x, frame.uv_offset.y}};
            vertices_.push_back(vertex);

            vertex = {
                {i * width + width, height, 0.f}, {1.f, 1.f, 1.f, 1.f},
                {frame.uv_offset.x + frame.uv_scale.x, frame.uv_offset.y}
            };
            vertices_.push_back(vertex);

            uint32_t base_index = static_cast<uint32_t>(vertices_.size() - 4);
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

    if (HasBegunPlay())
    {
        shape_->SetVertices(vertices_);
        shape_->SetIndices(indices_);
        shape_->SetTexture(damage > 0 ? damage_sprite_ : miss_sprite_);
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
