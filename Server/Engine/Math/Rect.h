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
        
        static bool Contains(const Rect& kRect, const Math::Vector2& kPoint);
        static bool Overlaps(const Rect& kRect, const Rect& other);

        FORCEINLINE float MinX() const { return x; }
        FORCEINLINE float MaxX() const { return x + width; }
        FORCEINLINE float MinY() const { return y; }
        FORCEINLINE float MaxY() const { return y + height; }
        
        FORCEINLINE Math::Vector2 Center() const { return {x + width / 2.f, y + height / 2.f}; }
        
        float x;
        float y;
        float width;
        float height;
    };
}
