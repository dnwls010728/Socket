#include "pch.h"
#include "Camera.h"

#include "DebugDrawHelper.h"
#include "Component/ColliderComponent.h"
#include "Component/TransformComponent.h"
#include "Level/World.h"
#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

std::weak_ptr<Camera> Camera::camera_;

Camera::Camera(const std::wstring& kName) :
    Actor(kName),
    size_(3.f),
    near_z_(.3f),
    far_z_(1000.f),
    target_(nullptr),
    target_collider_(nullptr),
    focus_area_(),
    focus_area_size_({1.f, 3.f}),
    vertical_offset_(1.f),
    half_width_(0),
    half_height_(0),
    limit_half_width_(0),
    limit_half_height_(0)
{
}

void Camera::PreInitializeComponents()
{
    Actor::PreInitializeComponents();
    
    std::shared_ptr<Camera> camera = camera_.lock();
    if (!camera) camera_ = GetSharedThis();
    else
    {
        Destroy();
        return;
    }

    UpdateProjectionMatrix();
}

void Camera::PhysicsTick(float delta_time)
{
    Actor::PhysicsTick(delta_time);
    
    if (target_ && target_collider_)
    {
        const Bounds& bounds = target_collider_->GetBounds();
        focus_area_.Update(bounds);
        
        const Math::Vector2 position = GetTransform()->GetPosition();
        
        Math::Vector2 focus_position = focus_area_.center + Math::Vector2::Up() * vertical_offset_;
        Math::Vector2 new_position = Math::Vector2::Lerp(position, focus_position, delta_time * 2.f);
        
        half_height_ = size_;
        half_width_ = GetAspect() * half_height_;
        
        float limit_x = limit_half_width_ - half_width_;
        if (limit_x < 0.f) limit_x = half_width_;
        
        float clamp_x = Math::Clamp(new_position.x, -limit_x, limit_x);
        
        float limit_y = limit_half_height_ - half_width_;
        if (limit_y < 0.f) limit_y = half_width_;
        
        float clamp_y = Math::Clamp(new_position.y, -limit_y, limit_y);
        GetTransform()->SetPosition({ clamp_x, clamp_y });;
    }
    
    TransformComponent* transform = GetTransform();
    Math::Vector2 position = transform->GetPosition();

    if (Viewport* viewport = Renderer::Get()->FindViewport(World::Get()->GetWindow()))
    {
        viewport->view_matrix = DirectX::XMMatrixTranslation(-position.x, -position.y, 0.f)
            * DirectX::XMMatrixRotationZ(-transform->GetAngle() * MATH_PI / 180.f);
    }
}

Camera* Camera::Get()
{
    if (camera_.expired()) return nullptr;
    if (const auto kCamera = camera_.lock())
    {
        if (Camera* ptr = kCamera.get(); IsValid(ptr)) return ptr;
    }

    return nullptr;
}

void Camera::SetSize(float size)
{
    size_ = size;
    UpdateProjectionMatrix();
}

void Camera::SetNearZ(float near_z)
{
    near_z_ = near_z;
    UpdateProjectionMatrix();
}

void Camera::SetFarZ(float far_z)
{
    far_z_ = far_z;
    UpdateProjectionMatrix();
}

void Camera::SetTarget(Actor* target)
{
    if (!IsValid(target)) return;
    target_ = target;

    target_collider_ = static_cast<ColliderComponent*>(target->GetComponent(ColliderComponent::StaticClass()));
    if (target_collider_)
    {
        const Bounds bounds = target_collider_->GetBounds();
        focus_area_.Setup(bounds, focus_area_size_);
    }
}

float Camera::GetAspect() const
{
    if (Viewport* viewport = Renderer::Get()->FindViewport(World::Get()->GetWindow()))
    {
        return viewport->d3d_viewport.Width / viewport->d3d_viewport.Height;
    }

    return 0.f;
}

Bounds Camera::GetBounds() const
{
    Math::Vector2 position = GetTransform()->GetPosition();

    const float height = size_ * 2.f;
    const float width = height * GetAspect();
    
    return {position, {width, height}};
}

void Camera::UpdateProjectionMatrix() const
{
    if (Viewport* viewport = Renderer::Get()->FindViewport(World::Get()->GetWindow()))
    {
        const float aspect = GetAspect();
        const float left = -size_ * aspect;
        const float right = size_ * aspect;
        
        viewport->projection_matrix = DirectX::XMMatrixOrthographicOffCenterLH(left, right, -size_, size_, near_z_, far_z_);
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Camera>("Camera")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
