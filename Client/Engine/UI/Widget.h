#pragma once
#include <vector>
#include <rttr/registration>

#include "Event/Events.h"
#include "Math/Rect.h"
#include "Math/Vector2.h"
#include "Misc/DelegateMacros.h"
#include "Misc/EngineMacros.h"
#include "rttr/registration_friend.h"

class Widget;

DECLARE_DELEGATE(OnWidgetEvent)
DECLARE_DELEGATE(OnDragEvent, const Math::Vector2&)
DECLARE_DELEGATE(OnDropEvent, const Math::Vector2&)

enum class AnchorPreset : Type::uint16
{
    kLeft = (0x01<<0),
    kRight = (0x01<<1),
    kTop = (0x01<<2),
    kBottom = (0x01<<3),
    kCenter = (0x01<<4),
    kMiddle = (0x01<<5),
    kStretch = (0x01<<6)
};

ENUM_CLASS_FLAGS(AnchorPreset)

enum class DrawMode : Type::uint8
{
    kSimple,
    kSliced
};

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
    void SetAnchorPreset(AnchorPreset anchor, bool match_pivot = false);
    void AttachToWidget(Widget* parent);
    void DetachFromWidget();

    bool HitTest(const Math::Vector2& kPoint) const;

    Math::Vector2 GetPosition() const;
    
    Math::Vector2 GetPivotPosition() const;

    FORCEINLINE const Math::Rect& GetRect() const { return rect_; }

    FORCEINLINE const Math::Vector2& GetAnchoredPosition() const { return position_; }
    
    FORCEINLINE float GetAngle() const { return angle_; }
    
    FORCEINLINE Widget* GetParent() const { return parent_; }
    FORCEINLINE const std::vector<Widget*>& GetChildren() const { return children_; }

    FORCEINLINE bool HasBegunPlay() const { return has_begun_play_; }

    FORCEINLINE void SetRayCastTarget(bool value) { is_ray_cast_target_ = value; }
    FORCEINLINE bool IsRayCastTarget() const { return is_ray_cast_target_; }

    FORCEINLINE bool IsFocused() const { return is_focused_; }

    OnWidgetEvent OnMousePressed;
    OnWidgetEvent OnMouseReleased;

    OnDragEvent OnDragStart;
    OnDragEvent OnDrag;
    OnDragEvent OnDragEnd;

    OnDropEvent OnDrop;

protected:
    friend class Canvas;

    virtual void BeginPlay();
    virtual void Tick(float delta_time);
    virtual void Render();
    virtual void UpdateRect();
    virtual void OnFocusChanged(bool is_focused);
    virtual void OnInputKey(Type::uint16 key_code, bool is_pressed);
    virtual void OnInputText(wchar_t character);

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

    bool has_begun_play_;
    bool is_ray_cast_target_;
    bool is_focused_;
    
};
