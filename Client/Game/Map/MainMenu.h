#pragma once
#include "Level/Level.h"

class MainMenu : public Level
{
    SHADER_CLASS_HELPER(MainMenu)
    GENERATED_BODY(MainMenu, Level)
    
public:
    MainMenu(const std::wstring& kName);
    virtual ~MainMenu() override = default;

    virtual void Load() override;
    
};
