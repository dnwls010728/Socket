#pragma once
#include "UI/Widget.h"

namespace UI_OLD
{
    class Button : public Widget
    {
        SHADER_CLASS_HELPER(Button)
        GENERATED_BODY(Button, Widget)
        
    public:
        Button(const std::wstring& kName);
        virtual ~Button() override = default;

        template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(void)>, std::decay_t<F>>>>
        void OnClick(F&& func);

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void OnClick(M* target, void(M::*func)(void));

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void OnClick(M* target, void(M::*func)(void) const);

        void OnClick(void(*func)(void));

        FORCEINLINE void SetText(const std::wstring& kText) { text_ = kText; }
        FORCEINLINE const std::wstring& GetText() const { return text_; }

        static std::shared_ptr<Button> Create(const std::wstring& kName);

    protected:
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;
        
        virtual bool OnMouseEnter() override;
        virtual bool OnMouseLeave() override;
        virtual bool OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed, double timestamp) override;
        
        Function<void(void)> click_event;

        std::wstring text_;
    
    };

    template <typename F, typename>
    void Button::OnClick(F&& func)
    {
        click_event = std::forward<F>(func);
    }

    template <typename M, typename>
    void Button::OnClick(M* target, void(M::* func)())
    {
        click_event = {target, func};
    }

    template <typename M, typename>
    void Button::OnClick(M* target, void(M::* func)() const)
    {
        click_event = {target, func};
    }
}
