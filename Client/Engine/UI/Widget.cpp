#include "pch.h"
#include "Widget.h"

#include "Canvas.h"
#include "Logger.h"
#include "Level/World.h"
#include "Math/Color.h"
#include "Math/Rect.h"
#include "Windows/DX/Renderer.h"

Widget::Widget(const std::wstring& kName) :
    name_(kName),
    rect_(),
    position_(Math::Vector2::Zero()),
    size_({100.f, 100.f}),
    anchor_min_({.5f, .5f}),
    anchor_max_({.5f, .5f}),
    pivot_({.5f, .5f}),
    angle_(0.f),
    parent_(nullptr),
    children_(),
    has_begun_play_(false),
    is_focused_(false)
{
}

void Widget::SetAnchoredPosition(const Math::Vector2& kPosition)
{
    position_ = kPosition;
    if (has_begun_play_) UpdateRect();
}

void Widget::SetPosition(const Math::Vector2& kPosition)
{
    Canvas* canvas = Canvas::Get();
    const Type::uint32 canvas_width = canvas->width_;
    const Type::uint32 canvas_height = canvas->height_;

    const float scale_ratio = canvas->GetScaleRatio();

    float x = (kPosition.x - canvas_width * anchor_min_.x) / scale_ratio;
    float y = (kPosition.y - canvas_height * (1.f - anchor_min_.y)) / scale_ratio;
    position_ = {x, y};
    
    if (has_begun_play_) UpdateRect();
}

void Widget::SetSize(const Math::Vector2& kSize)
{
    size_ = kSize;
    if (has_begun_play_) UpdateRect();
}

void Widget::SetAnchorMin(const Math::Vector2& kAnchorMin)
{
    anchor_min_ = kAnchorMin;
    if (has_begun_play_) UpdateRect();
}

void Widget::SetAnchorMax(const Math::Vector2& kAnchorMax)
{
    anchor_max_ = kAnchorMax;
    if (has_begun_play_) UpdateRect();
}

void Widget::SetPivot(const Math::Vector2& kPivot)
{
    pivot_ = kPivot;
    if (has_begun_play_) UpdateRect();
}

void Widget::SetAnchors(const Math::Vector2& kAnchorMin, const Math::Vector2& kAnchorMax)
{
    anchor_min_ = kAnchorMin;
    anchor_max_ = kAnchorMax;
    if (has_begun_play_) UpdateRect();
}

void Widget::SetAnchorPreset(AnchorPreset anchor, bool match_pivot)
{
    if (EnumHasAllFlags(anchor, AnchorPreset::kLeft | AnchorPreset::kTop)) SetAnchors({0.f, 1.f}, {0.f, 1.f});
    else if (EnumHasAllFlags(anchor, AnchorPreset::kRight | AnchorPreset::kTop)) SetAnchors({1.f, 1.f}, {1.f, 1.f});
    else if (EnumHasAllFlags(anchor, AnchorPreset::kMiddle | AnchorPreset::kCenter)) SetAnchors({.5f, .5f}, {.5f, .5f});
    else if (EnumHasAllFlags(anchor, AnchorPreset::kLeft | AnchorPreset::kBottom)) SetAnchors({0.f, 0.f}, {0.f, 0.f});
    else if (EnumHasAllFlags(anchor, AnchorPreset::kRight | AnchorPreset::kBottom)) SetAnchors({1.f, 0.f}, {1.f, 0.f});
    else if (EnumHasAllFlags(anchor, AnchorPreset::kStretch | AnchorPreset::kTop)) SetAnchors({0.f, 1.f}, {1.f, 1.f});
    else if (EnumHasAllFlags(anchor, AnchorPreset::kStretch | AnchorPreset::kCenter)) SetAnchors({0.f, .5f}, {1.f, .5f});
    else if (EnumHasAllFlags(anchor, AnchorPreset::kStretch | AnchorPreset::kBottom)) SetAnchors({0.f, 0.f}, {1.f, 0.f});
    else if (EnumHasAllFlags(anchor, AnchorPreset::kStretch | AnchorPreset::kLeft)) SetAnchors({0.f, 0.f}, {0.f, 1.f});
    else if (EnumHasAllFlags(anchor, AnchorPreset::kStretch | AnchorPreset::kMiddle)) SetAnchors({.5f, 0.f}, {.5f, 1.f});
    else if (EnumHasAllFlags(anchor, AnchorPreset::kStretch | AnchorPreset::kRight)) SetAnchors({1.f, 0.f}, {1.f, 1.f});
    else if (EnumHasAllFlags(anchor, AnchorPreset::kTop | AnchorPreset::kCenter)) SetAnchors({.5f, 1.f}, {.5f, 1.f});
    else if (EnumHasAllFlags(anchor, AnchorPreset::kBottom | AnchorPreset::kCenter)) SetAnchors({.5f, 0.f}, {.5f, 0.f});
    else if (EnumHasAllFlags(anchor, AnchorPreset::kLeft | AnchorPreset::kMiddle)) SetAnchors({0.f, .5f}, {0.f, .5f});
    else if (EnumHasAllFlags(anchor, AnchorPreset::kRight | AnchorPreset::kMiddle)) SetAnchors({1.f, .5f}, {1.f, .5f});
    else if (EnumHasAllFlags(anchor, AnchorPreset::kStretch)) SetAnchors({0.f, 0.f}, {1.f, 1.f});

    if (match_pivot)
    {
        if (EnumHasAnyFlags(anchor, AnchorPreset::kLeft)) pivot_.x = 0.f;
        else if (EnumHasAnyFlags(anchor, AnchorPreset::kRight)) pivot_.x = 1.f;
        else pivot_.x = .5f;

        if (EnumHasAnyFlags(anchor, AnchorPreset::kTop)) pivot_.y = 1.f;
        else if (EnumHasAnyFlags(anchor, AnchorPreset::kBottom)) pivot_.y = 0.f;
        else pivot_.y = .5f;

        if (has_begun_play_) UpdateRect();
    }
}

void Widget::AttachToWidget(Widget* parent)
{
    parent_ = parent;
    parent_->children_.push_back(this);
    if (has_begun_play_) UpdateRect();
}

void Widget::DetachFromWidget()
{
    if (!parent_) return;

    std::erase(parent_->children_, this);
    parent_ = nullptr;

    if (has_begun_play_) UpdateRect();
}

bool Widget::HitTest(const Math::Vector2& kPoint) const
{
    return Math::Rect::Contains(rect_, kPoint);
}

Math::Vector2 Widget::GetPosition() const
{
    Canvas* canvas = Canvas::Get();
    const Type::uint32 canvas_width = canvas->width_;
    const Type::uint32 canvas_height = canvas->height_;

    const float scale_ratio = canvas->GetScaleRatio();

    float x = canvas_width * anchor_min_.x + position_.x * scale_ratio;
    float y = canvas_height * (1.f - anchor_min_.y) + position_.y * scale_ratio;
    return {x, y};
}

Math::Vector2 Widget::GetPivotPosition() const
{
    return {rect_.x + rect_.width * pivot_.x, rect_.y + rect_.height * (1.f - pivot_.y)};
}

void Widget::BeginPlay()
{
    UpdateRect();
    has_begun_play_ = true;
    
    for (const auto& child : children_)
    {
        child->BeginPlay();
    }
}

void Widget::Tick(float delta_time)
{
    for (const auto& child : children_)
    {
        child->Tick(delta_time);
    }
}

void Widget::Render()
{
    if (is_focused_)
    {
        WindowsWindow* window = World::Get()->GetWindow();
        if (!window) return;

        Renderer* renderer = Renderer::Get();
        if (!renderer) return;
    
        Math::Vector2 pivot_position = GetPivotPosition();
        if (GetParent()) pivot_position = GetParent()->GetPivotPosition();

        renderer->DrawBox(window, rect_, pivot_position, Math::Color::Green, angle_, 1.f);
    }
    
    for (const auto& child : children_)
    {
        child->Render();
    }
}

void Widget::UpdateRect()
{
    Type::uint32 parent_width;
    Type::uint32 parent_height;
    
    Math::Vector2 parent_position = {0.f, 0.f};

    Canvas* canvas = Canvas::Get();
    const float scale_ratio = canvas->GetScaleRatio();

    if (parent_)
    {
        parent_width = parent_->rect_.width;
        parent_height = parent_->rect_.height;
        parent_position = {parent_->rect_.x, parent_->rect_.y};
    }
    else
    {
        parent_width = canvas->width_;
        parent_height = canvas->height_;
    }

    float left;
    float top;
    float right;
    float bottom;

    if (anchor_min_.x == anchor_max_.x)
    {
        left = parent_width * anchor_min_.x + position_.x * scale_ratio + parent_position.x;
        right = size_.x * scale_ratio;
    }
    else
    {
        left = parent_width * anchor_min_.x + position_.x * scale_ratio + parent_position.x;
        right = (anchor_max_.x - anchor_min_.x) * parent_width - position_.x * scale_ratio - size_.x;
    }

    if (anchor_min_.y == anchor_max_.y)
    {
        float anchored_min_y = parent_height * (1.f - anchor_min_.y) + parent_position.y;
        if (anchor_min_.y == 0.f) anchored_min_y = parent_height + parent_position.y;

        top = anchored_min_y + position_.y * scale_ratio;
        bottom = size_.y * scale_ratio;
    }
    else
    {
        float anchored_max_y = parent_height * (1.f - anchor_max_.y) + parent_position.y;
        if (anchor_max_.y == 0.f) anchored_max_y = 0.f + parent_position.y;

        top = anchored_max_y + position_.y * scale_ratio;
        bottom = (anchor_max_.y - anchor_min_.y) * parent_height - position_.y * scale_ratio - size_.y * scale_ratio;
    }

    const float pivot_x = right * pivot_.x;

    float pivot_y = bottom * pivot_.y;
    if (pivot_y == 0.f) pivot_y = bottom;
    else if (pivot_y == bottom) pivot_y = 0.f;

    if (anchor_min_.x == anchor_max_.x) left -= pivot_x;
    if (anchor_min_.y == anchor_max_.y) top -= pivot_y;

    rect_ = {left, top, right, bottom};

    for (Widget* child : children_)
    {
        child->UpdateRect();
    }
}

bool Widget::OnFocus(bool is_focused)
{
    is_focused_ = is_focused;
    return false;
}

bool Widget::OnMouseEnter()
{
    return false;
}

bool Widget::OnMouseLeave()
{
    return false;
}

bool Widget::OnMouseMotion(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    bool is_handled = false;
    for (auto it = children_.rbegin(); it != children_.rend(); ++it)
    {
        Widget* child = *it;

        bool is_result = child->HitTest(kPosition);
        bool is_previous_result = child->HitTest(kPosition - kDelta);

        if (is_result != is_previous_result)
        {
            is_handled |= child->OnMouseEnter();
            is_handled |= child->OnMouseLeave();
        }

        if (is_result || is_previous_result)
            is_handled |= child->OnMouseMotion(kPosition, kDelta);
    }

    return is_handled;
}

bool Widget::OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed)
{
    for (auto it = children_.rbegin(); it != children_.rend(); ++it)
    {
        Widget* child = *it;
        if (child->HitTest(kPosition) && child->OnMouseButton(kPosition, button, is_pressed))
            return true;
    }

    if (button == MouseButton::kLeft && is_pressed && !is_focused_) Canvas::Get()->SetWidgetFocus(this);
    return false;
}

bool Widget::OnBeginDrag(const Math::Vector2& kPosition)
{
    return false;
}

bool Widget::OnDrag(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    return false;
}

bool Widget::OnEndDrag(const Math::Vector2& kPosition)
{
    return false;
}

bool Widget::OnDrop(const Math::Vector2& kPosition, const Widget* kWidget)
{
    return false;
}

bool Widget::OnScroll(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    for (auto it = children_.rbegin(); it != children_.rend(); ++it)
    {
        Widget* child = *it;
        if (child->HitTest(kPosition) && child->OnScroll(kPosition, kDelta))
            return true;
    }
    
    return false;
}

bool Widget::OnKey(Type::uint16 key_code, bool is_pressed)
{
    return false;
}

bool Widget::OnChar(wchar_t character)
{
    return false;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Widget>("Widget")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
