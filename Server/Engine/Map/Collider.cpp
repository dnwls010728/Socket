#include "pch.h"
#include "Collider.h"

Collider::Collider()
{
}

Collider::Projection Collider::Project(const Polygon& polygon, const Math::Vector2& axis) const
{
    float min = Math::Vector2::Dot(polygon.vertices[0], axis);
    float max = min;

    for (int32_t i = 1; i < polygon.vertices.size(); ++i)
    {
        float temp = Math::Vector2::Dot(polygon.vertices[i], axis);
        if (temp < min) min = temp;
        if (temp > max) max = temp;
    }

    return { min, max };
}

bool Collider::Overlap(const Polygon& a, const Polygon& b, const Math::Vector2& axis) const
{
    Projection result_a = Project(a, axis);
    Projection result_b = Project(b, axis);
    return !(result_a.max < result_b.min || result_b.max < result_a.min);
}
