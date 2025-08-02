#pragma once
#include "Math/Color.h"
#include "UI/UIContainer.h"

class UIPalette : public UIContainer
{
    GENERATED_BODY(UIPalette, UIContainer)
    
public:
    UIPalette(const std::wstring& name);
    virtual ~UIPalette() override = default;
    
    template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(Math::Color)>, std::decay_t<F>>>>
    void OnValueChanged(F&& func);

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnValueChanged(M* target, void(M::*func)(Math::Color));

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnValueChanged(M* target, void(M::*func)(Math::Color) const);

    void OnValueChanged(void(*func)(Math::Color));

    void SetHue(float hue);

    FORCEINLINE const Math::Color& GetColor() const { return color_; }

protected:
    virtual void Init() override;
    virtual void Render() override;

    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;

private:
    void UpdateColor(bool is_notify = true);
    
    float h_;
    float s_;
    float v_;
    
    Math::Color color_;

    Function<void(Math::Color)> value_changed_event_;
    
};

template <typename F, typename>
void UIPalette::OnValueChanged(F&& func)
{
    value_changed_event_ = std::forward<F>(func);
}

template <typename M, typename>
void UIPalette::OnValueChanged(M* target, void(M::* func)(Math::Color))
{
    value_changed_event_ = {target, func};
}

template <typename M, typename>
void UIPalette::OnValueChanged(M* target, void(M::* func)(Math::Color) const)
{
    value_changed_event_ = {target, func};
}
