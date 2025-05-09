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
        
        template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(const std::wstring&)>, std::decay_t<F>>>>
        void OnValueChanged(F&& func);

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void OnValueChanged(M* target, void(M::*func)(const std::wstring&));

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void OnValueChanged(M* target, void(M::*func)(const std::wstring&) const);

        void OnValueChanged(void(*func)(const std::wstring&));
        
        template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(const std::wstring&)>, std::decay_t<F>>>>
        void OnReturn(F&& func);

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void OnReturn(M* target, void(M::*func)(const std::wstring&));

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void OnReturn(M* target, void(M::*func)(const std::wstring&) const);

        void OnReturn(void(*func)(const std::wstring&));
        void SetText(const std::wstring& kText);

        FORCEINLINE const std::wstring& GetText() const { return text_; }

        FORCEINLINE void SetPlaceholder(const std::wstring& kPlaceholder) { placeholder_ = kPlaceholder; }
        FORCEINLINE const std::wstring& GetPlaceholder() const { return placeholder_; }

        static std::shared_ptr<EditableTextBox> Create(const std::wstring& kName);

    protected:
        virtual void Tick(float delta_time) override;
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;

        virtual bool OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed, double timestamp) override;
        virtual bool OnKey(uint16_t key_code, bool is_pressed) override;
        virtual bool OnChar(wchar_t character) override;

        virtual void OnFocus(bool is_focus) override;

        float GetAdvances(const std::wstring& kString, std::vector<float>& advances);
        
        std::wstring font_name_;
        std::wstring text_;
        std::wstring placeholder_;

        float font_size_;
        float elapsed_time_;
        float total_advance_;

        bool cursor_visible_;

        int32_t cursor_position_;

        Function<void(const std::wstring&)> value_changed_event_;
        Function<void(const std::wstring&)> return_event_;

        std::vector<float> advances_;
    
    };

    template <typename F, typename>
    void EditableTextBox::OnValueChanged(F&& func)
    {
        value_changed_event_ = std::forward<F>(func);
    }

    template <typename M, typename>
    void EditableTextBox::OnValueChanged(M* target, void(M::* func)(const std::wstring&))
    {
        value_changed_event_ = { target, func };
    }

    template <typename M, typename>
    void EditableTextBox::OnValueChanged(M* target, void(M::* func)(const std::wstring&) const)
    {
        value_changed_event_ = { target, func };
    }

    template <typename F, typename>
    void EditableTextBox::OnReturn(F&& func)
    {
        return_event_ = std::forward<F>(func);
    }

    template <typename M, typename>
    void EditableTextBox::OnReturn(M* target, void(M::* func)(const std::wstring&))
    {
        return_event_ = { target, func };
    }

    template <typename M, typename>
    void EditableTextBox::OnReturn(M* target, void(M::* func)(const std::wstring&) const)
    {
        return_event_ = { target, func };
    }
}
