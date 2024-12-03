#pragma once
#include "Level/Level.h"

class EditableTextBox;

class MainMenu : public Level
{
    SHADER_CLASS_HELPER(MainMenu)
    GENERATED_BODY(MainMenu, Level)
    
public:
    MainMenu(const std::wstring& kName);
    virtual ~MainMenu() override = default;

protected:
    virtual void Load() override;
    virtual void Tick(float delta_time) override;

private:
    class ScrollBox* scroll_box;
    
    EditableTextBox* id_text_box;
    EditableTextBox* pw_text_box;
    
};
