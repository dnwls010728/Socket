#pragma once
#include "Level/Level.h"

class EditorMap : public Level
{
    SHADER_CLASS_HELPER(EditorMap)
    GENERATED_BODY(EditorMap, Level)
    
public:
    EditorMap(const std::wstring& name);
    virtual ~EditorMap() override = default;

protected:
    virtual void Load() override;
    
};
