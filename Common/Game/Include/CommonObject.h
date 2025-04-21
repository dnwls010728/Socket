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
    int map_unique_id;
    float last_position_x;
    float last_position_y;

    inline bool IsValid() const { return unique_id > 0; }
};