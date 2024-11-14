#pragma once
#include "Level/Level.h"

class MainMap : public Level
{
    SHADER_CLASS_HELPER(MainMap)
    GENERATED_BODY(MainMap, Level)
    
public:
    MainMap(const std::wstring& kName);
    virtual ~MainMap() override = default;

protected:
    virtual void Load() override;
    virtual void Tick(float delta_time) override;

private:
    class EditableTextBox* chat_input_box_;
    class Text* chat_text_;

};
