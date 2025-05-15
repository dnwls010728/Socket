#pragma once
#include "UI.h"
#include "Input/Mouse.h"
#include "Misc/EngineMacros.h"

class UIElement;

class UIState
{
    GENERATED_BODY(UIState)
    
public:
    UIState();
    virtual ~UIState() = default;

protected:
    friend class UI;

    FORCEINLINE virtual void Init() {}
    FORCEINLINE virtual void Tick(float delta_time) {}
    FORCEINLINE virtual void Render() {}
    
    virtual UI::MouseEventResult OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta);
    virtual UI::MouseEventResult OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp);
    
    virtual bool OnScroll(const Math::Vector2& position, const Math::Vector2& delta);
    virtual bool OnKey(uint16_t key_code, bool is_pressed);
    virtual bool OnChar(wchar_t character);

private:
    std::map<rttr::type::type_id, std::unique_ptr<UIElement>> elements_;
    
};
