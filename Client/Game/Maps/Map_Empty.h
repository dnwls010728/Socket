#pragma once
#include "World/Level.h"

class Map_Empty : public Level
{
    SHADER_CLASS_HELPER(Map_Empty)
    GENERATED_BODY(Map_Empty, Level)
    
public:
    Map_Empty(const std::wstring& kName);
    virtual ~Map_Empty() override = default;

protected:
    virtual void Load() override;
    
};
