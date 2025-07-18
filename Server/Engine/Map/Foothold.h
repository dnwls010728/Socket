#pragma once
#include "Math/Vector2.h"
#include <cstdint>

class Foothold
{
public:
    Foothold(const Math::Vector2& point1, const Math::Vector2& point2, uint32_t id, uint32_t previous, uint32_t next);
    ~Foothold() = default;

    inline float GetX1() const { return point1_.x; }
    inline float GetX2() const { return point2_.x; }
    inline float GetY1() const { return point1_.y; }
    inline float GetY2() const { return point2_.y; }

    inline const Math::Vector2& GetPoint1() const { return point1_; }
    inline const Math::Vector2& GetPoint2() const { return point2_; }

    inline uint32_t GetID() const { return id_; }
    inline uint32_t GetPrevious() const { return previous_; }
    inline uint32_t GetNext() const { return next_; }
    
    float GetYAt(float x) const;

private:
    Math::Vector2 point1_;
    Math::Vector2 point2_;

    uint32_t id_;
    uint32_t previous_;
    uint32_t next_;
    
};
