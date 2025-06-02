#include "pch.h"
#include "Foothold.h"

Foothold::Foothold(const Math::Vector2& point1, const Math::Vector2& point2) :
    point1_(point1),
    point2_(point2)
{
}

float Foothold::GetYAt(float x) const
{
    float t = (x - point1_.x) / (point2_.x - point1_.x);
    return point1_.y + t * (point2_.y - point1_.y);
}
