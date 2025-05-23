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

bool Math::Rect::Contains(const Rect& kRect, const Math::Vector2& kPoint)
{
    return kPoint.x >= kRect.x && kPoint.x <= kRect.x + kRect.width && kPoint.y >= kRect.y && kPoint.y <= kRect.y + kRect.height;
}

bool Math::Rect::Overlaps(const Rect& kRect, const Rect& other)
{
    return kRect.x < other.x + other.width && kRect.x + kRect.width > other.x && kRect.y < other.y + other.height && kRect.y + kRect.height > other.y;
}
