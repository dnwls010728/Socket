#include "pch.h"
#include "Foothold.h"

#include "Math/Math.h"

Foothold::Foothold(const Math::Vector2& point1, const Math::Vector2& point2, uint32_t id, uint32_t previous, uint32_t next) :
    point1_(point1),
    point2_(point2),
    id_(id),
    previous_(previous),
    next_(next)
{
}

float Foothold::GetYAt(float x) const
{
    float t = (x - point1_.x) / (point2_.x - point1_.x);
    return Math::Lerp(point1_.y, point2_.y, t);
}

float Foothold::GetXDelta() const
{
    return point2_.x - point1_.x;
}

float Foothold::GetYDelta() const
{
    return point2_.y - point1_.y;
}

float Foothold::GetSlope() const
{
    return GetYDelta() / GetXDelta();
}
