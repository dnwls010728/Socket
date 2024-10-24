#pragma once
#include "Vector2.h"

namespace Math
{
    struct Rect
    {
        Rect();
        Rect(float x, float y, float width, float height);
        Rect operator=(const Rect& kOther);

        static Rect Zero();

        bool Contains(const Math::Vector2& kPoint) const;
        bool Overlaps(const Rect& other) const;

        inline float MinX() const { return x; }
        inline float MaxX() const { return x + width; }
        inline float MinY() const { return y; }
        inline float MaxY() const { return y + height; }
        
        inline Math::Vector2 Center() const { return {x + width / 2.f, y + height / 2.f}; }
        
        float x;
        float y;
        float width;
        float height;
    };
}
