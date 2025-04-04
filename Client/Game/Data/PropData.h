#pragma once

struct PropData
{
    RTTR_ENABLE()

public:
    Type::uint32 id;
    std::wstring name;
    std::wstring sprite_path;
    std::wstring animation_pack_path;
    Type::uint32 weight;
    
};
