#pragma once
#include "Level/Level.h"

class InGameMap : public Level
{
    SHADER_CLASS_HELPER(InGameMap)
    GENERATED_BODY(InGameMap, Level)
    
public:
    InGameMap(const std::wstring& kName);
    virtual ~InGameMap() override = default;

protected:
    virtual void Load() override;
    
};
