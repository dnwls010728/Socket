#pragma once
#include "UI.h"
#include "Input/Mouse.h"
#include "Misc/EngineMacros.h"

class UIElement
{
    GENERATED_BODY(UIElement)
    
public:
    virtual ~UIElement() = default;
    
    bool IsInRange(const Math::Vector2& position) const;

    FORCEINLINE Math::Vector2 GetPosition() const { return position_; }
    FORCEINLINE Math::Vector2 GetSize() const { return size_; }

protected:
    friend class UIState;
    friend class UIContainer;
    
    UIElement();
    
    FORCEINLINE virtual void Init() {}
    FORCEINLINE virtual void Tick(float delta_time) {}
    FORCEINLINE virtual void Render(const Math::Vector2& parent_position) {}

    FORCEINLINE bool IsActive() const { return is_active_; }
    FORCEINLINE void SetActive(bool active) { is_active_ = active; }
    
    virtual UI::MouseEventResult OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta);
    virtual UI::MouseEventResult OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp);
    
    virtual bool OnScroll(const Math::Vector2& position, const Math::Vector2& delta);
    virtual bool OnKey(uint16_t key_code, bool is_pressed);
    virtual bool OnChar(wchar_t character);

    Math::Vector2 position_;
    Math::Vector2 size_;

    bool is_active_;
    
};
