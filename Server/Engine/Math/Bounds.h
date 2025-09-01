#pragma once
#include "Vector2.h"

struct Bounds
{
    Bounds() = default;
    Bounds(const Math::Vector2& center, const Math::Vector2& size);

    void Expand(float amount);
    
    /**
     * \brief 두 바운드의 교차점을 반환합니다.
     * \param a 첫번째 바운드
     * \param b 두번째 바운드
     * \return Bounds
     */
    static Bounds Intersect(const Bounds& a, const Bounds& b);

    /**
     * \brief 첫번째 바운드가 두번째 바운드를 포함하는지 확인합니다.
     * \param a 첫번째 바운드
     * \param b 두번째 바운드
     * \return bool
     */
    static bool Contains(const Bounds& a, const Bounds& b);
    
    Math::Vector2 center;
    Math::Vector2 size;
    Math::Vector2 extents;
    Math::Vector2 min;
    Math::Vector2 max;
};
