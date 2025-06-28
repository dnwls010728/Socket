#pragma once
#include "Math/Vector2.h"

class Foothold
{
public:
    Foothold(const Math::Vector2& point1, const Math::Vector2& point2);
    ~Foothold() = default;

    inline float GetX1() const { return point1_.x; }
    inline float GetX2() const { return point2_.x; }
    inline float GetY1() const { return point1_.y; }
    inline float GetY2() const { return point2_.y; }

    inline const Math::Vector2& GetPoint1() const { return point1_; }
    inline const Math::Vector2& GetPoint2() const { return point2_; }
    
    float GetYAt(float x) const;

private:
    Math::Vector2 point1_;
    Math::Vector2 point2_;
    
};
