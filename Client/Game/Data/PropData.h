#pragma once

struct PropData
{
    RTTR_ENABLE()

public:
    int id;
    std::wstring name;
    std::wstring sprite_path;
    std::wstring animation_pack_path;
    int weight;
    
};
