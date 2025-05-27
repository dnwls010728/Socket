#pragma once
#include "Math/Vector2.h"

class Collider
{
public:
    struct Polygon
    {
        std::vector<Math::Vector2> vertices;
    };

    struct Projection
    {
        float min;
        float max;
    };

    Collider();
    ~Collider() = default;

    Projection Project(const Polygon& polygon, const Math::Vector2& axis) const;

    bool Overlap(const Polygon& a, const Polygon& b, const Math::Vector2& axis) const;
    
};
