#pragma once
#include "Level/Level.h"

class TempMap : public Level
{
    SHADER_CLASS_HELPER(TempMap)
    GENERATED_BODY(TempMap, Level)
    
public:
    TempMap(const std::wstring& kName);
    virtual ~TempMap() override = default;

protected:
    virtual void Load() override;
    virtual void Tick(float delta_time) override;
    
};
