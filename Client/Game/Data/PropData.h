#pragma once

struct PropData
{
    RTTR_ENABLE()

public:
    int32_t id;
    std::wstring name;
    std::wstring sprite_path;
    std::wstring animation_pack_path;
    int32_t weight;
    
};
