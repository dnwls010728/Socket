#pragma once
#include "UI.h"
#include "Input/Mouse.h"
#include "Misc/EngineMacros.h"
#include "rttr/registration_friend.h"

class UIElement
{
    GENERATED_BODY(UIElement)
    
public:
    virtual ~UIElement() = default;
    
    bool IsInRange(const Math::Vector2& position) const;

    FORCEINLINE void SetPosition(const Math::Vector2& position) { position_ = position; }
    FORCEINLINE Math::Vector2 GetPosition() const { return position_; }

    FORCEINLINE void SetSize(const Math::Vector2& size) { size_ = size; }
    FORCEINLINE Math::Vector2 GetSize() const { return size_; }
    
    FORCEINLINE bool IsActive() const { return is_active_; }
    FORCEINLINE virtual void SetActive(bool active) { is_active_ = active; }

    FORCEINLINE bool IsFocused() const { return is_focused_; }

protected:
    friend class UIState;
    friend class UIContainer;
    
    UIElement();
    
    FORCEINLINE virtual void Init() {}
    FORCEINLINE virtual void Tick(float delta_time) {}
    FORCEINLINE virtual void Render(const Math::Vector2& parent_position) {}
    
    virtual UI::MouseEventResult OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta);
    virtual UI::MouseEventResult OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp);
    
    virtual bool OnScroll(const Math::Vector2& position, const Math::Vector2& delta);
    virtual bool OnKey(uint16_t key_code, bool is_pressed);
    virtual bool OnChar(wchar_t character);
    
    virtual void OnFocus(bool is_focused);

    Math::Vector2 position_;
    Math::Vector2 size_;

    bool is_active_;
    bool is_focused_;

    UIContainer* parent_;
    
};
