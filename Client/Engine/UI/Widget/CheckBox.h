#pragma once
#include "UI/Widget.h"

namespace UI
{
    class CheckBox : public Widget
    {
        SHADER_CLASS_HELPER(CheckBox)
        GENERATED_BODY(CheckBox, Widget)
        
    public:
        CheckBox(const std::wstring& kName);
        virtual ~CheckBox() override = default;
        
        template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(bool)>, std::decay_t<F>>>>
        void AddCheckEvent(F&& func);

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void AddCheckEvent(M* target, void(M::*func)(bool));

        template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void AddCheckEvent(M* target, void(M::*func)(bool) const);

        void AddCheckEvent(void(*func)(bool));

        void SetCheck(bool value);

        static std::shared_ptr<CheckBox> Create(const std::wstring& kName);

    protected:
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;

        virtual bool OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed) override;
        
        Function<void(bool)> check_event;
        
        bool is_checked_;
    
    };

    template <typename F, typename>
    void CheckBox::AddCheckEvent(F&& func)
    {
        check_event = std::forward<F>(func);
    }

    template <typename M, typename>
    void CheckBox::AddCheckEvent(M* target, void(M::* func)(bool))
    {
        check_event = { target, func };
    }

    template <typename M, typename>
    void CheckBox::AddCheckEvent(M* target, void(M::* func)(bool) const)
    {
        check_event = { target, func };
    }
}
