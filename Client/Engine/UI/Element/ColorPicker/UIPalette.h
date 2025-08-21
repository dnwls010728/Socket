#pragma once
#include "Math/Color.h"
#include "Misc/FunctionMacros.h"
#include "UI/UIContainer.h"

class UIPalette : public UIContainer
{
    GENERATED_BODY(UIPalette, UIContainer)
    
public:
    UIPalette(const std::wstring& name);
    virtual ~UIPalette() override = default;

    void SetHue(float hue);

    FORCEINLINE const Math::Color& GetColor() const { return color_; }

    DEFINE_BIND_OVERLOADS(value_changed_event_, OnValueChanged, void, const Math::Color&)

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

    Function<void(const Math::Color&)> value_changed_event_;
    
};
