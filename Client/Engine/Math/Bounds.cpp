#include "pch.h"
#include "Math/Bounds.h"

Bounds::Bounds(Math::Vector2 center, Math::Vector2 size) :
    center(center),
    size(size)
{
    extents = size * .5f;

    min = center - extents;
    max = center + extents;
}

void Bounds::Expand(float amount)
{
    size += {amount, amount};
    extents = size * .5f;

    min = center - extents;
    max = center + extents;
}

Bounds Bounds::Intersect(Bounds a, Bounds b)
{
    Math::Vector2 min = Math::Vector2::Max(a.min, b.min);
    Math::Vector2 max = Math::Vector2::Min(a.max, b.max);

    return {(min + max) * .5f, max - min};
}

bool Bounds::Contains(Bounds a, Bounds b)
{
    if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
    if (a.max.y < b.min.y || a.min.y > b.max.y) return false;

    return true;
}
