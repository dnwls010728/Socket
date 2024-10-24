#include "pch.h"
#include "Rect.h"

Math::Rect::Rect() :
    x(0.f),
    y(0.f),
    width(0.f),
    height(0.f)
{
}

Math::Rect::Rect(float x, float y, float width, float height) :
    x(x),
    y(y),
    width(width),
    height(height)
{
}

Math::Rect Math::Rect::operator=(const Rect& kOther)
{
    x = kOther.x;
    y = kOther.y;
    width = kOther.width;
    height = kOther.height;
    return *this;
}

Math::Rect Math::Rect::Zero()
{
    return {0.f, 0.f, 0.f, 0.f};
}

bool Math::Rect::Contains(const Math::Vector2& kPoint) const
{
    return kPoint.x >= x && kPoint.x <= x + width && kPoint.y >= y && kPoint.y <= y + height;
}

bool Math::Rect::Overlaps(const Rect& other) const
{
    return x < other.MaxX() && MaxX() > other.x && y < other.MaxY() && MaxY() > other.y;
}
