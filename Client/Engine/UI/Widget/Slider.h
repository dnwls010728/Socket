#pragma once
#include "UI/Widget.h"

namespace UI
{
    class Slider : public Widget
    {
        SHADER_CLASS_HELPER(Slider)
        GENERATED_BODY(Slider, Widget)

    public:
        Slider(const std::wstring& kName);
        virtual ~Slider() override = default;

        template <typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(float)>, std::decay_t<F>>>>
        void AddValueChangedEvent(F&& func);

        template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void AddValueChangedEvent(M* target, void (M::*func)(float));

        template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void AddValueChangedEvent(M* target, void (M::*func)(float) const);

        void AddValueChangedEvent(void (*func)(float));
        void SetValue(float value);

        FORCEINLINE float GetValue() const { return value_; }

        static std::shared_ptr<Slider> Create(const std::wstring& kName);

    protected:
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;

        virtual bool OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed) override;
        virtual bool OnDrag(const Math::Vector2& kPosition) override;

        float value_;
        float min_value_;
        float max_value_;

        Function<void(float)> value_changed_event_;
    };

    template <typename F, typename>
    void Slider::AddValueChangedEvent(F&& func)
    {
        value_changed_event_ = std::forward<F>(func);
    }

    template <typename M, typename>
    void Slider::AddValueChangedEvent(M* target, void (M::*func)(float))
    {
        value_changed_event_ = {target, func};
    }

    template <typename M, typename>
    void Slider::AddValueChangedEvent(M* target, void (M::*func)(float) const)
    {
        value_changed_event_ = {target, func};
    }
}
