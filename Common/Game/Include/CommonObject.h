#pragma	 once
#include <vector>
#include <string>

struct CharacterInfo
{
    int unique_id;
    int account_unique_id;
    std::wstring name;
    int lv;
    int job;
    int map;
    float last_position_x;
    float last_position_y;
};