#include "pch.h"
#include "Foothold.h"

#include "Math/Math.h"

Foothold::Foothold(const Math::Vector2& point1, const Math::Vector2& point2) :
    point1_(point1),
    point2_(point2)
{
}

float Foothold::GetYAt(float x) const
{
    float t = (x - point1_.x) / (point2_.x - point1_.x);
    return Math::Lerp(point1_.y, point2_.y, t);
}
