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

    virtual void SetAbsolutePosition(const Math::Vector2& position);
    const Math::Vector2& GetAbsolutePosition();

    virtual void SetRelativePosition(const Math::Vector2& position);

    bool IsDescendantOf(UIElement* ancestor) const;
    
    FORCEINLINE const std::wstring& GetName() const { return name_; }

    FORCEINLINE const Math::Vector2& GetRelativePosition() const { return relative_position_; }

    FORCEINLINE virtual void SetSize(const Math::Vector2& size) { size_ = size; }
    FORCEINLINE const Math::Vector2& GetSize() const { return size_; }
    
    FORCEINLINE bool IsActive() const { return is_active_; }
    FORCEINLINE virtual void SetActive(bool active) { is_active_ = active; }

    FORCEINLINE bool IsFocused() const { return is_focused_; }

    FORCEINLINE void SetIgnoreRayCast(bool ignore) { is_ignore_raycast = ignore; }
protected:
    friend class UIState;
    friend class UIContainer;
    
    UIElement(const std::wstring& name);

    FORCEINLINE virtual void Uninit() {}
    FORCEINLINE virtual void Tick(float delta_time) {}
    FORCEINLINE virtual void Render() {}

    virtual void Init();
    
    virtual UIElement* RayCast(const Math::Vector2& position);
    
    virtual UI::MouseEventResult OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta);
    virtual UI::MouseEventResult OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp);

    virtual bool OnMouseEnter();
    virtual bool OnMouseLeave();
    virtual bool OnDragBegin(const Math::Vector2& position);
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta);
    virtual bool OnDragEnd(const Math::Vector2& position);
    virtual bool OnDrop(const Math::Vector2& position, UIElement* target);
    virtual bool OnScroll(const Math::Vector2& position, const Math::Vector2& delta);
    virtual bool OnKey(uint16_t key_code, bool is_pressed);
    virtual bool OnChar(wchar_t character);
    
    virtual void OnFocus(bool is_focused);

    void UpdateAbsolutePosition();

    FORCEINLINE virtual void MakeDirty() { is_dirty_ = true; }

    std::wstring name_;
    
    Math::Vector2 relative_position_;
    Math::Vector2 absolute_position_;
    Math::Vector2 size_;

    bool is_dirty_;
    bool has_initialized_;
    bool is_active_;
    bool is_focused_;
    bool is_ignore_raycast;

    UIContainer* parent_;
    
};
