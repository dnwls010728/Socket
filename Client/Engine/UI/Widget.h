#pragma once
#include <vector>
#include <rttr/registration>

#include "Event/Events.h"
#include "Math/Rect.h"
#include "Math/Vector2.h"
#include "Misc/DelegateMacros.h"
#include "Misc/EngineMacros.h"
#include "rttr/registration_friend.h"

namespace UI
{
    enum AnchorPresets
    {
        kLeft = (0x01<<0),
        kRight = (0x01<<1),
        kTop = (0x01<<2),
        kBottom = (0x01<<3),
        kCenter = (0x01<<4),
        kMiddle = (0x01<<5),
        kStretch = (0x01<<6)
    };
}

class Widget : public std::enable_shared_from_this<Widget>
{
    SHADER_CLASS_HELPER(Widget)
    GENERATED_BODY(Widget)
    
public:
    Widget(const std::wstring& kName);
    virtual ~Widget() = default;

    void SetAnchoredPosition(const Math::Vector2& kPosition);
    void SetPosition(const Math::Vector2& kPosition);
    void SetSize(const Math::Vector2& kSize);
    void SetAnchorMin(const Math::Vector2& kAnchorMin);
    void SetAnchorMax(const Math::Vector2& kAnchorMax);
    void SetPivot(const Math::Vector2& kPivot);
    void SetAnchors(const Math::Vector2& kAnchorMin, const Math::Vector2& kAnchorMax);
    void SetAnchorPreset(Type::uint16 anchor, bool match_pivot = false);
    void AttachToWidget(Widget* parent);
    void DetachFromWidget();
    
    Math::Vector2 GetPivotPosition() const;
    
    FORCEINLINE float GetAngle() const { return angle_; }
    
    FORCEINLINE Widget* GetParent() const { return parent_; }
    FORCEINLINE const std::vector<Widget*>& GetChildren() const { return children_; }
    
    FORCEINLINE bool IsFocused() const { return is_focused_; }

protected:
    friend class Canvas;

    FORCEINLINE virtual void OnMousePressed() {}
    FORCEINLINE virtual void OnMouseReleased() {}
    FORCEINLINE virtual void OnMouseHover() {}
    FORCEINLINE virtual void OnMouseLeave() {}
    FORCEINLINE virtual void OnKeyEvent(Type::uint16 key_code, bool is_pressed) {}
    FORCEINLINE virtual void OnCharEvent(wchar_t character) {}
    FORCEINLINE virtual void BeginPlay() {}
    FORCEINLINE virtual void Tick(float delta_time) {}
    FORCEINLINE virtual void Render() {}
    
    virtual void OnFocus();
    virtual void OnBlur();
    
    virtual void UpdateRect();
    
    static Type::uint32 next_z_index_;

    std::wstring name_;

    Math::Rect rect_;
    
    Math::Vector2 position_;
    Math::Vector2 size_;
    Math::Vector2 anchor_min_;
    Math::Vector2 anchor_max_;
    Math::Vector2 pivot_;

    float angle_;

    Widget* parent_;
    std::vector<Widget*> children_;

    Type::uint32 z_index_;

    bool can_interact_;
    bool is_focused_;
    
};
