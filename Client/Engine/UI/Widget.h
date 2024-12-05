#pragma once
#include <vector>

#include "Event/Events.h"
#include "Math/Rect.h"
#include "Math/Vector2.h"
#include "Misc/DelegateMacros.h"
#include "Misc/EngineMacros.h"
#include "rttr/registration_friend.h"

class Widget;

DECLARE_DELEGATE(BeginDragDelegate, const Math::Vector2&)
DECLARE_DELEGATE(DragDelegate, const Math::Vector2&, const Math::Vector2&)
DECLARE_DELEGATE(EndDragDelegate, const Math::Vector2&)
DECLARE_DELEGATE(DropDelegate, const Math::Vector2&, const Widget*)

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

    FORCEINLINE const std::wstring& GetName() const { return name_; }

    FORCEINLINE const Math::Rect& GetRect() const { return rect_; }

    FORCEINLINE const Math::Vector2& GetAnchoredPosition() const { return position_; }

    FORCEINLINE const Math::Vector2& GetSize() const { return size_; }
    
    FORCEINLINE float GetAngle() const { return angle_; }
    
    FORCEINLINE Widget* GetParent() const { return parent_; }
    FORCEINLINE const std::vector<Widget*>& GetChildren() const { return children_; }

    FORCEINLINE bool HasBegunPlay() const { return has_begun_play_; }
    FORCEINLINE bool IsActive() const { return is_active_; }
    FORCEINLINE bool IsFocused() const { return is_focused_; }

    BeginDragDelegate BeginDragHandler;
    DragDelegate DragHandler;
    EndDragDelegate EndDragHandler;
    DropDelegate DropHandler;

protected:
    friend class Canvas;

    virtual void BeginPlay();
    virtual void Tick(float delta_time);
    virtual void Render();
    virtual void OnWidgetAttached(Widget* child);
    virtual void UpdateRect();

    // Input Events
    virtual bool OnFocus(bool is_focused);
    virtual bool OnMouseEnter();
    virtual bool OnMouseLeave();
    virtual bool OnMouseMotion(const Math::Vector2& kPosition, const Math::Vector2& kDelta);
    virtual bool OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed);
    virtual bool OnBeginDrag(const Math::Vector2& kPosition);
    virtual bool OnDrag(const Math::Vector2& kPosition, const Math::Vector2& kDelta);
    virtual bool OnEndDrag(const Math::Vector2& kPosition);
    virtual bool OnDrop(const Math::Vector2& kPosition, const Widget* kWidget);
    virtual bool OnScroll(const Math::Vector2& kPosition, const Math::Vector2& kDelta);
    virtual bool OnKey(Type::uint16 key_code, bool is_pressed);
    virtual bool OnChar(wchar_t character);

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
    bool is_active_;
    bool is_focused_;
    
};
