#pragma once

class Map_Begin : public Level
{
    SHADER_CLASS_HELPER(Map_Begin)
    GENERATED_BODY(Map_Begin, Level)
    
public:
    Map_Begin(const std::wstring& kName);
    virtual ~Map_Begin() override = default;
    
};
