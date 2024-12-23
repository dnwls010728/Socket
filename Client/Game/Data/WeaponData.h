#pragma once

struct WeaponData
{
    int id;
    
    std::wstring name;
    std::wstring description;

    int size;
    int capacity;

    float reload_time;
    float speed;
    float power;
    float delay;
    float range;
    float spread;
};
