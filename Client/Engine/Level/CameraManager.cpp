#include "pch.h"
#include "CameraManager.h"

#include "DebugDrawHelper.h"
#include "Actor/Component/ColliderComponent.h"
#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

CameraManager::CameraManager() :
    size_(3.f),
    near_z_(.3f),
    far_z_(1000.f),
    angle_(0.f),
    vertical_offset_(0.f),
    focus_area_(),
    target_weak_ptr(),
    collider_weak_ptr_(),
    position_(Math::Vector2::Zero()),
    focus_area_size_({2.f, 2.f}),
    tick_type_(TickType::kTick)
{
    limit_half_width_ = std::numeric_limits<float>::max();
    limit_half_height_ = std::numeric_limits<float>::max();
}

float CameraManager::GetAspect()
{
    if (Viewport* viewport = Renderer::Get()->FindViewport(World::Get()->GetWindow()))
    {
        return viewport->d3d_viewport.Width / viewport->d3d_viewport.Height;
    }

    return 0.f;
}

void CameraManager::SetPosition(const Math::Vector2& kPosition)
{
    position_ = kPosition;
    UpdateViewMatrix();

    World::Get()->UpdateCameraBounds(GetBounds());
}

void CameraManager::SetAngle(float angle)
{
    angle_ = angle;
    UpdateViewMatrix();
}

void CameraManager::SetSize(float size)
{
    size_ = size;
    UpdateProjectionMatrix();
}

void CameraManager::SetNearZ(float near_z)
{
    near_z_ = near_z;
    UpdateProjectionMatrix();
}

void CameraManager::SetFarZ(float far_z)
{
    far_z_ = far_z;
    UpdateProjectionMatrix();
}

void CameraManager::SetTarget(const std::shared_ptr<Actor>& kActor)
{
    if (!kActor) return;

    target_weak_ptr = kActor;

    std::shared_ptr<ColliderComponent> collider_component = kActor->GetComponent<ColliderComponent>(ColliderComponent::StaticClass());
    collider_weak_ptr_ = collider_component;

    const Bounds bounds = collider_component->GetBounds();
    focus_area_.Setup(bounds, focus_area_size_);
}

void CameraManager::SetLimit(const float width, const float height)
{
    limit_half_width_ = width * .5f;
    limit_half_height_ = height * .5f;
}

const Bounds& CameraManager::GetBounds()
{
    const float kHeight = size_ * 2.f;
    const float kWidth = kHeight * GetAspect();
    
    return {position_, {kWidth, kHeight}};
}

void CameraManager::Init()
{
    UpdateProjectionMatrix();
}

void CameraManager::PhysicsTick(float delta_time)
{
    if (tick_type_ == TickType::kPhysicsTick) MoveToTarget(delta_time);
}

void CameraManager::Tick(float delta_time)
{
    if (tick_type_ == TickType::kTick) MoveToTarget(delta_time);
}

void CameraManager::PostTick(float delta_time)
{
    if (tick_type_ == TickType::kPostTick) MoveToTarget(delta_time);
}

void CameraManager::UpdateProjectionMatrix()
{
    if (Viewport* viewport = Renderer::Get()->FindViewport(World::Get()->GetWindow()))
    {
        const float aspect = GetAspect();
        const float left = -size_ * aspect;
        const float right = size_ * aspect;

        viewport->projection_matrix = DirectX::XMMatrixOrthographicOffCenterLH(
            left, right, -size_, size_, near_z_, far_z_);
    }
}

void CameraManager::UpdateViewMatrix()
{
    if (Viewport* viewport = Renderer::Get()->FindViewport(World::Get()->GetWindow()))
    {
        viewport->view_matrix = DirectX::XMMatrixTranslation(-position_.x, -position_.y, 0.f)
            * DirectX::XMMatrixRotationZ(-angle_ * MATH_PI / 180.f);
    }
}

void CameraManager::MoveToTarget(float delta_time)
{
    std::shared_ptr<Actor> target_ptr = target_weak_ptr.lock();
    std::shared_ptr<ColliderComponent> collider = collider_weak_ptr_.lock();
    if (!target_ptr || !collider) return;

    const Bounds bounds = collider->GetBounds();
    focus_area_.Update(bounds);

    Math::Vector2 focus_position = focus_area_.center + Math::Vector2::Up() * vertical_offset_;
    Math::Vector2 new_position = Math::Vector2::Lerp(position_, focus_position, delta_time * 2.f);

    const float half_height = size_;
    const float half_width = GetAspect() * half_height;
    
    float limit_x = limit_half_width_ - half_width;
    if (limit_x < 0.f) limit_x = half_width;

    float limit_y = limit_half_height_ - half_height;
    if (limit_y < 0.f) limit_y = half_height;

    float clamp_x = Math::Clamp(new_position.x, -limit_x, limit_x);
    float clamp_y = Math::Clamp(new_position.y, -limit_y, limit_y);
    SetPosition({clamp_x, clamp_y});

    DebugDrawHelper::Get()->DrawBox(focus_area_.center, focus_area_size_, Math::Color::Red);
}
