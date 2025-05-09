#pragma	 once
#include <vector>
#include <string>

struct Movement
{
    Movement() :
        x(0.f),
        y(0.f)
    {
    }

    Movement(float x, float y) :
        x(x),
        y(y)
    {
    }
    
    float x;
    float y;
};

struct CharacterInfo
{
    int unique_id;
    int account_unique_id;
    std::wstring name;
    int lv;
    int job;
    int map_unique_id;
    float last_position_x;
    float last_position_y;

    inline bool IsValid() const { return unique_id > 0; }
};

struct ObjectInfo
{
    int unique_id;
    std::wstring name;
    std::wstring class_name;
    int map_unique_id;
    float last_position_x;
    float last_position_y;

    inline bool IsValid() const { return unique_id > 0; }
};