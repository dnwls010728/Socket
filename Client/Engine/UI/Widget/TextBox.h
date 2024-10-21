#pragma once
#include "Math/Color.h"
#include "UI/Widget.h"

namespace UI
{
    enum class ContentType
    {
        kText,
        kPassword
    };
    
    class TextBox : public Widget
    {
        SHADER_CLASS_HELPER(TextBox)
        GENERATED_BODY(TextBox, Widget)
        
    public:
        TextBox(const std::wstring& kName);
        virtual ~TextBox() override = default;

        virtual void BeginPlay() override;
        virtual void Tick(float delta_time) override;
        virtual void Render() override;
        virtual void OnFocus() override;
        virtual void OnBlur() override;
        virtual void OnKeyEvent(Type::uint16 key_code, bool is_pressed) override;
        virtual void OnCharEvent(wchar_t character) override;

        inline void SetContentType(ContentType content_type) { content_type_ = content_type; }
        inline void SetPlaceholder(const std::wstring& kPlaceholder) { placeholder_ = kPlaceholder; }

    private:
        ContentType content_type_;
        
        Math::Rect text_area_;
        
        std::wstring text_;
        std::wstring placeholder_;
        
        int cursor_position_;

        float elapsed_;

        bool show_cursor_;

        std::vector<float> advances_;
        
    };
}
