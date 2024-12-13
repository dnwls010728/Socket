#include "pch.h"
#include "Shape.h"

#include "Math/Math.h"

Shape::Shape() :
    vertices_(),
    indices_(),
    world_matrix_(DirectX::XMMatrixIdentity()),
    primitive_topology_(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
    position_(Math::Vector2::Zero()),
    scale_(Math::Vector2::One()),
    pivot_(Math::Vector2::Zero()),
    uv_offset_(Math::Vector2::Zero()),
    uv_scale_(Math::Vector2::One()),
    color_(Math::Color::White),
    rotation_(0.f),
    texture_(nullptr),
    z_order_(0)
{
    UpdateMatrixx();
}

void Shape::SetPosition(const Math::Vector2& position)
{
    position_ = position;
    UpdateMatrixx();
}

void Shape::SetScale(const Math::Vector2& scale)
{
    scale_ = scale;
    UpdateMatrixx();
}

void Shape::SetPivot(const Math::Vector2& pivot)
{
    pivot_ = pivot;
    UpdateMatrixx();
}

void Shape::SetRotation(float rotation)
{
    rotation_ = DirectX::XMConvertToRadians(rotation);
    UpdateMatrixx();
}

Bounds Shape::GetBounds() const
{
    if (vertices_.empty())
    {
        return {Math::Vector2::Zero(), Math::Vector2::Zero()};
    }

    Math::Vector2 min = Math::Vector2::PositiveInfinity();
    Math::Vector2 max = Math::Vector2::NegativeInfinity();
    
    // 변환 행렬을 계산
    DirectX::XMMATRIX transform = DirectX::XMMatrixScaling(scale_.x, scale_.y, 1.f) * // 크기 조정
                                  DirectX::XMMatrixTranslation(-pivot_.x, -pivot_.y, 0.f) * // Pivot 위치로 이동
                                  DirectX::XMMatrixRotationZ(rotation_) * // 회전
                                  DirectX::XMMatrixTranslation(pivot_.x, pivot_.y, 0.f) * // 원래 위치로 이동
                                  DirectX::XMMatrixTranslation(position_.x - pivot_.x, position_.y - pivot_.y, 0.f); // 최종 위치로 이동

    for (const auto& kVertex : vertices_)
    {
        // 정점 위치를 변환
        DirectX::XMVECTOR pos = DirectX::XMVectorSet(kVertex.position.x, kVertex.position.y, 0.f, 1.f);
        pos = DirectX::XMVector3TransformCoord(pos, transform);

        // 변환된 정점 위치로 min/max 계산
        min.x = Math::Min(min.x, DirectX::XMVectorGetX(pos));
        min.y = Math::Min(min.y, DirectX::XMVectorGetY(pos));
        max.x = Math::Max(max.x, DirectX::XMVectorGetX(pos));
        max.y = Math::Max(max.y, DirectX::XMVectorGetY(pos));
    }

    return {(min + max) * .5f, max - min};
}

bool Shape::CompareZOrder(const std::shared_ptr<Shape>& kLHS, const std::shared_ptr<Shape>& kRHS)
{
    return kLHS->GetZOrder() < kRHS->GetZOrder();
}

void Shape::UpdateMatrixx()
{
    if (scale_.x < 0.f) pivot_.x *= -1.f;
    if (scale_.y < 0.f) pivot_.y *= -1.f;
    
    world_matrix_ = DirectX::XMMatrixScaling(scale_.x, scale_.y, 1.f) * // 크기 조정
        DirectX::XMMatrixTranslation(-pivot_.x, -pivot_.y, 0.f) * // Pivot 위치로 이동
        DirectX::XMMatrixRotationZ(rotation_) * // 회전
        DirectX::XMMatrixTranslation(pivot_.x, pivot_.y, 0.f) * // 원래 위치로 이동
        DirectX::XMMatrixTranslation(position_.x - pivot_.x, position_.y - pivot_.y, 0.f); // 최종 위치로 이동
}
