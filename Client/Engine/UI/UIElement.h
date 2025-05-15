#pragma once
#include "UI.h"
#include "Input/Mouse.h"
#include "Misc/EngineMacros.h"

class UIElement
{
    GENERATED_BODY(UIElement)
    
public:
    UIElement();
    UIElement(const Math::Vector2& position, const Math::Vector2& size);
    
    virtual ~UIElement() = default;
    
    bool IsInRange(const Math::Vector2& position) const;

protected:
    friend class UIState;
    
    FORCEINLINE virtual void Init() {}
    FORCEINLINE virtual void Tick(float delta_time) {}
    FORCEINLINE virtual void Render() {}
    
    virtual UI::MouseEventResult OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta);
    virtual UI::MouseEventResult OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp);
    
    virtual bool OnScroll(const Math::Vector2& position, const Math::Vector2& delta);
    virtual bool OnKey(uint16_t key_code, bool is_pressed);
    virtual bool OnChar(wchar_t character);

private:
    Math::Vector2 position_;
    Math::Vector2 size_;
    
};
