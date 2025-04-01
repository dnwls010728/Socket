#pragma once
#include "UI/Widget.h"

namespace UI
{
    class EditableTextBox : public Widget
    {
        SHADER_CLASS_HELPER(EditableTextBox)
        GENERATED_BODY(EditableTextBox, Widget)
        
    public:
        EditableTextBox(const std::wstring& kName);
        virtual ~EditableTextBox() override = default;

        static std::shared_ptr<EditableTextBox> Create(const std::wstring& kName);

    protected:
        virtual bool OnKey(Type::uint16 key_code, bool is_pressed) override;
        virtual bool OnChar(wchar_t character) override;
        
        std::wstring text_;
    
    };
}
