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

        FORCEINLINE float XMin() const { return x; }
        FORCEINLINE float XMax() const { return x + width; }
        FORCEINLINE float YMin() const { return y; }
        FORCEINLINE float YMax() const { return y + height; }
        
        FORCEINLINE Vector2 Center() const { return {x + width / 2.f, y + height / 2.f}; }
        FORCEINLINE Vector2 Size() const { return {width, height}; }
        FORCEINLINE Vector2 Min() const { return {x, y}; }
        FORCEINLINE Vector2 Max() const { return {x + width, y + height}; }
        
        float x;
        float y;
        float width;
        float height;
    };
}
