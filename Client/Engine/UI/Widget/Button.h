#pragma once
#include "UI/Widget.h"

namespace UI
{
    class Button : public Widget
    {
        SHADER_CLASS_HELPER(Button)
        GENERATED_BODY(Button, Widget)
        
    public:
        Button(const std::wstring& kName);
        virtual ~Button() override = default;

        template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(void)>, std::decay_t<F>>>>
        void AddEvent(F&& func);

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void AddEvent(M* target, void(M::*func)(void));

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void AddEvent(M* target, void(M::*func)(void) const);

        void AddEvent(void(*func)(void));

        static std::shared_ptr<Button> Create(const std::wstring& kName);

    protected:
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;
        
        virtual bool OnMouseEnter() override;
        virtual bool OnMouseLeave() override;
        virtual bool OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed) override;
        
        Function<void(void)> click_event;
    
    };

    template <typename F, typename>
    void Button::AddEvent(F&& func)
    {
        click_event = std::forward<F>(func);
    }

    template <typename M, typename>
    void Button::AddEvent(M* target, void(M::* func)())
    {
        click_event = {target, func};
    }

    template <typename M, typename>
    void Button::AddEvent(M* target, void(M::* func)() const)
    {
        click_event = {target, func};
    }
}
