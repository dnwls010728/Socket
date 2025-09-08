#pragma once
#include <cstdint>

struct BuffStatBlock
{
    int32_t max_hp = 0; // 최대 체력
    int32_t atk = 0; // 공격력
    int32_t def = 0; // 방어력
    int32_t dig = 0; // 방어력 무시
    
    BuffStatBlock& operator+=(const BuffStatBlock& rhs)
    {
        max_hp += rhs.max_hp;
        atk += rhs.atk;
        def += rhs.def;
        dig += rhs.dig;
        return *this;
    }

    BuffStatBlock& operator-=(const BuffStatBlock& rhs)
    {
        max_hp -= rhs.max_hp;
        atk -= rhs.atk;
        def -= rhs.def;
        dig -= rhs.dig;
        return *this;
    }
};
