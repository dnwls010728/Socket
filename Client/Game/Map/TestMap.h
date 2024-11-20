#pragma once
#include "Level/Level.h"

class Image;

class TestMap : public Level
{
    SHADER_CLASS_HELPER(TestMap)
    GENERATED_BODY(TestMap, Level)
    
public:
    TestMap(const std::wstring& kName);
    virtual ~TestMap() override = default;

protected:
    virtual void Load() override;

private:
    Image* message_input_background_;
    Image* message_history_;
    Image* resize_handle_;

    class EditableTextBox* message_input_;
    
    class ScrollBox* message_scroll_area_;
    
};
