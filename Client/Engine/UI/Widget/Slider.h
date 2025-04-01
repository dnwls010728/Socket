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
        void OnSlide(F&& func);

        template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void OnSlide(M* target, void (M::*func)(float));

        template <typename M, typename = std::enable_if_t<std::is_class_v<M>>>
        void OnSlide(M* target, void (M::*func)(float) const);

        void OnSlide(void (*func)(float));
        void SetValue(float value);

        FORCEINLINE float GetValue() const { return value_; }

        FORCEINLINE float GetMinValue() const { return min_value_; }
        FORCEINLINE void SetMinValue(float value) { min_value_ = value; }

        FORCEINLINE float GetMaxValue() const { return max_value_; }
        FORCEINLINE void SetMaxValue(float value) { max_value_ = value; }

        FORCEINLINE float GetStep() const { return step_; }
        FORCEINLINE void SetStep(float value) { step_ = value; }

        static std::shared_ptr<Slider> Create(const std::wstring& kName);

    protected:
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;

        virtual bool OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed) override;
        virtual bool OnDragBegin(const Math::Vector2& kPosition) override;
        virtual bool OnDragEnd(const Math::Vector2& kPosition) override;
        virtual bool OnDrag(const Math::Vector2& kPosition, const Math::Vector2& kDelta) override;

        void SetValueInternal(const Math::Vector2& kPosition);

        float value_;
        float min_value_;
        float max_value_;
        float step_;

        Function<void(float)> value_changed_event_;
    };

    template <typename F, typename>
    void Slider::OnSlide(F&& func)
    {
        value_changed_event_ = std::forward<F>(func);
    }

    template <typename M, typename>
    void Slider::OnSlide(M* target, void (M::*func)(float))
    {
        value_changed_event_ = {target, func};
    }

    template <typename M, typename>
    void Slider::OnSlide(M* target, void (M::*func)(float) const)
    {
        value_changed_event_ = {target, func};
    }
}
