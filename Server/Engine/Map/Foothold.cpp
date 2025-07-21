#include "pch.h"
#include "Foothold.h"

#include "Math/Math.h"

Foothold::Foothold(const Math::Vector2& point1, const Math::Vector2& point2, uint32_t id, uint32_t previous_id, uint32_t next_id) :
    point1_(point1),
    point2_(point2),
    id_(id),
    previous_id_(previous_id),
    next_id_(next_id)
{
}

float Foothold::GetYAt(float x) const
{
    float t = (x - point1_.x) / (point2_.x - point1_.x);
    return Math::Lerp(point1_.y, point2_.y, t);
}

bool Foothold::IsSlope() const
{
    float dx = point2_.x - point1_.x;
    float dy = point2_.y - point1_.y;

    return dx != 0.0f && dy != 0.0f;
}
