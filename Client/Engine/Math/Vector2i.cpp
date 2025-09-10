#include "pch.h"
#include "Vector2i.h"

Math::Vector2i::Vector2i() :
    x(0),
    y(0)
{
}

Math::Vector2i::Vector2i(int32_t new_x, int32_t new_y) :
    x(new_x),
    y(new_y)
{
}

Math::Vector2i& Math::Vector2i::operator+=(const Vector2i& vector)
{
    x += vector.x;
    y += vector.y;
    
    return *this;
}

Math::Vector2i& Math::Vector2i::operator-=(const Vector2i& vector)
{
    x -= vector.x;
    y -= vector.y;

    return *this;
}

Math::Vector2i& Math::Vector2i::operator*=(const Vector2i& vector)
{
    x *= vector.x;
    y *= vector.y;

    return *this;
}

Math::Vector2i& Math::Vector2i::operator=(const Vector2i& vector)
{
    x = vector.x;
    y = vector.y;

    return *this;
}

Math::Vector2i Math::Vector2i::operator+(const Vector2i& vector)
{
    x += vector.x;
    y += vector.y;

    return *this;
}

Math::Vector2i Math::Vector2i::operator-(const Vector2i& vector)
{
    x -= vector.x;
    y -= vector.y;

    return *this;
}

Math::Vector2i Math::Vector2i::operator*(const Vector2i& vector)
{
    x *= vector.x;
    y *= vector.y;

    return *this;
}

Math::Vector2i Math::Vector2i::operator+(const Vector2i& vector) const
{
    return {x + vector.x, y + vector.y};
}

Math::Vector2i Math::Vector2i::operator-(const Vector2i& vector) const
{
    return {x - vector.x, y - vector.y};
}

Math::Vector2i Math::Vector2i::operator*(const Vector2i& vector) const
{
    return {x * vector.x, y * vector.y};
}

Math::Vector2i Math::Vector2i::operator+(int32_t val)
{
    return {x + val, y + val};
}

Math::Vector2i Math::Vector2i::operator-(int32_t val)
{
    return {x - val, y - val};
}

Math::Vector2i Math::Vector2i::operator*(int32_t val)
{
    return {x * val, y * val};
}

Math::Vector2i Math::Vector2i::operator/(int32_t val)
{
    return {x / val, y / val};
}

Math::Vector2i Math::Vector2i::operator+(int32_t val) const
{
    return {x + val, y + val};
}

Math::Vector2i Math::Vector2i::operator-(int32_t val) const
{
    return {x - val, y - val};
}

Math::Vector2i Math::Vector2i::operator*(int32_t val) const
{
    return {x * val, y * val};
}

Math::Vector2i Math::Vector2i::operator/(int32_t val) const
{
    return {x / val, y / val};
}

bool Math::Vector2i::operator==(const Vector2i& vector) const
{
    return x == vector.x && y == vector.y;
}

bool Math::Vector2i::operator!=(const Vector2i& vector) const
{
    return x != vector.x || y != vector.y;
}

Math::Vector2i Math::Vector2i::Zero()
{
    return {0, 0};
}

Math::Vector2i Math::Vector2i::One()
{
    return {1, 1};
}

Math::Vector2i Math::Vector2i::Left()
{
    return {-1, 0};
}

Math::Vector2i Math::Vector2i::Up()
{
    return {0, 1};
}

Math::Vector2i Math::Vector2i::Right()
{
    return {1, 0};
}

Math::Vector2i Math::Vector2i::Down()
{
    return {0, -1};
}

Math::Vector2i Math::operator+(int32_t val, const Vector2i& vector)
{
    return vector * val;
}

Math::Vector2i Math::operator-(int32_t val, const Vector2i& vector)
{
    return vector - val;
}

Math::Vector2i Math::operator*(int32_t val, const Vector2i& vector)
{
    return vector * val;
}

Math::Vector2i Math::operator/(int32_t val, const Vector2i& vector)
{
    return vector / val;
}
