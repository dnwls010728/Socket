#include "pch.h"
#include "Math/Vector2.h"

#include <cmath>
#include <limits>

#include "Math.h"

Math::Vector2::Vector2() :
    x(),
    y()
{
}

Math::Vector2::Vector2(float new_x, float new_y) :
    x(new_x),
    y(new_y)
{
    if (std::isnan(x) || std::isinf(x)) x = 0.f;
    if (std::isnan(y) || std::isinf(y)) y = 0.f;
}

Math::Vector2& Math::Vector2::operator+=(const Vector2& kVector)
{
    x += kVector.x;
    y += kVector.y;

    return *this;
}

Math::Vector2& Math::Vector2::operator-=(const Vector2& kVector)
{
    x -= kVector.x;
    y -= kVector.y;

    return *this;
}

Math::Vector2& Math::Vector2::operator*=(const Vector2& kVector)
{
    x *= kVector.x;
    y *= kVector.y;

    return *this;
}

Math::Vector2& Math::Vector2::operator/=(const Vector2& kVector)
{
    x /= kVector.x;
    y /= kVector.y;

    return *this;
}

Math::Vector2& Math::Vector2::operator=(const Vector2& kVector)
{
    x = kVector.x;
    y = kVector.y;

    if (std::isnan(x) || std::isinf(x)) x = 0.f;
    if (std::isnan(y) || std::isinf(y)) y = 0.f;

    return *this;
}

Math::Vector2 Math::Vector2::operator+(const Vector2& kVector)
{
    return {x + kVector.x, y + kVector.y};
}

Math::Vector2 Math::Vector2::operator-(const Vector2& kVector)
{
    return {x - kVector.x, y - kVector.y};
}

Math::Vector2 Math::Vector2::operator*(const Vector2& kVector)
{
    return {x * kVector.x, y * kVector.y};
}

Math::Vector2 Math::Vector2::operator/(const Vector2& kVector)
{
    return {x / kVector.x, y / kVector.y};
}

Math::Vector2 Math::Vector2::operator+(const Vector2& kVector) const
{
    return {x + kVector.x, y + kVector.y};
}

Math::Vector2 Math::Vector2::operator-(const Vector2& kVector) const
{
    return {x - kVector.x, y - kVector.y};
}

Math::Vector2 Math::Vector2::operator*(const Vector2& kVector) const
{
    return {x * kVector.x, y * kVector.y};
}

Math::Vector2 Math::Vector2::operator/(const Vector2& kVector) const
{
    return {x / kVector.x, y / kVector.y};
}

Math::Vector2 Math::Vector2::operator+(float val)
{
    return {x + val, y + val};
}

Math::Vector2 Math::Vector2::operator-(float val)
{
    return {x - val, y - val};
}

Math::Vector2 Math::Vector2::operator*(float val)
{
    return {x * val, y * val};
}

Math::Vector2 Math::Vector2::operator/(float val)
{
    return {x / val, y / val};
}

Math::Vector2 Math::Vector2::operator+(float val) const
{
    return {x + val, y + val};
}

Math::Vector2 Math::Vector2::operator-(float val) const
{
    return {x - val, y - val};
}

Math::Vector2 Math::Vector2::operator*(float val) const
{
    return {x * val, y * val};
}

Math::Vector2 Math::Vector2::operator/(float val) const
{
    return {x / val, y / val};
}

bool Math::Vector2::operator==(const Vector2& kVector) const
{
    if (std::fabsf(x - kVector.x) <= FLT_EPSILON &&
        std::fabsf(y - kVector.y) <= FLT_EPSILON)
    {
        return true;
    }

    return false;
}

bool Math::Vector2::operator!=(const Vector2& kVector) const
{
    if (std::fabsf(x - kVector.x) > FLT_EPSILON ||
        std::fabsf(y - kVector.y) > FLT_EPSILON)
    {
        return true;
    }

    return false;
}

bool Math::Vector2::operator==(float val) const
{
    if (std::fabsf(x - val) <= FLT_EPSILON &&
        std::fabsf(y - val) <= FLT_EPSILON)
    {
        return true;
    }

    return false;
}

bool Math::Vector2::operator!=(float val) const
{
    if (std::fabsf(x - val) > FLT_EPSILON ||
        std::fabsf(y - val) > FLT_EPSILON)
    {
        return true;
    }

    return false;
}

Math::Vector2 Math::Vector2::Zero()
{
    return {0.f, 0.f};
}

Math::Vector2 Math::Vector2::One()
{
    return {1.f, 1.f};
}

Math::Vector2 Math::Vector2::Left()
{
    return {-1.f, 0.f};
}

Math::Vector2 Math::Vector2::Up()
{
    return {0.f, 1.f};
}

Math::Vector2 Math::Vector2::Right()
{
    return {1.f, 0.f};
}

Math::Vector2 Math::Vector2::Down()
{
    return {0.f, -1.f};
}

Math::Vector2 Math::Vector2::Lerp(Vector2 a, Vector2 b, float t)
{
    t = Clamp(t, 0.f, 1.f);
    return a + (b - a) * t;
}

float Math::Vector2::Distance(Vector2 a, Vector2 b)
{
    Vector2 diff = a - b;
    return diff.Magnitude();
}

float Math::Vector2::Dot(Vector2 a, Vector2 b)
{
    const float theta = (a.x * b.x) + (a.y * b.y);
    return theta;
}

Math::Vector2 Math::Vector2::Max(Vector2 a, Vector2 b)
{
    const float max_x = Math::Max(a.x, b.x);
    const float max_y = Math::Max(a.y, b.y);

    return {max_x, max_y};
}

Math::Vector2 Math::Vector2::Min(Vector2 a, Vector2 b)
{
    const float min_x = Math::Min(a.x, b.x);
    const float min_y = Math::Min(a.y, b.y);

    return {min_x, min_y};
}

Math::Vector2 Math::Vector2::Reflect(Vector2 direction, Vector2 normal)
{
    float num = -2.f * Dot(normal, direction);
    return direction + (normal * num);
}

Math::Vector2 Math::Vector2::PositiveInfinity()
{
    return {FLT_MAX, FLT_MAX};
}

Math::Vector2 Math::Vector2::NegativeInfinity()
{
    return {-FLT_MAX, -FLT_MAX};
}

float Math::Vector2::Angle(Vector2 from, Vector2 to)
{
    float cos_theta = Dot(from, to) / (from.Magnitude() * to.Magnitude());
    cos_theta = Clamp(cos_theta, -1.f, 1.f);
    
    return std::acos(cos_theta) * Rad2Deg();
}

Math::Vector2 Math::Vector2::Normalized() const
{
    float m = Magnitude();

    if (m > 0)
    {
        return {x / m, y / m};
    }

    return Zero();
}

float Math::Vector2::Magnitude() const
{
    const auto temp_x = static_cast<double>(x);
    const auto temp_y = static_cast<double>(y);

    return static_cast<float>(std::sqrt(std::pow(temp_x, 2) + std::pow(temp_y, 2)));
}

void Math::Vector2::Normalize()
{
    float m = Magnitude();

    if (m > 0)
    {
        x /= m;
        y /= m;
    }
    else
    {
        x = 0.f;
        y = 0.f;
    }
}
