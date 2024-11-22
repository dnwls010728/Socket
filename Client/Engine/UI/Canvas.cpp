#include "pch.h"
#include "Canvas.h"

#include "Logger.h"
#include "Widget.h"
#include "Widget/ScrollBox.h"

Canvas::Canvas() :
    width_(0.f),
    height_(0.f),
    reference_resolution_width_(ProjectSettings::kCanvasReferenceWidth),
    reference_resolution_height_(ProjectSettings::kCanvasReferenceHeight),
    match_mode_(ProjectSettings::kMatchMode),
    widgets_(),
    root_widget_(nullptr),
    mouse_position_(Math::Vector2::Zero())
{
}

Widget* Canvas::FindWidget(const std::wstring& kName)
{
    for (const auto& widget : widgets_)
    {
        if (widget->name_ == kName) return widget.get();
    }

    return nullptr;
}

void Canvas::GetWidgets(std::vector<Widget*>& widgets) const
{
    for (const auto& widget : widgets_)
    {
        widgets.push_back(widget.get());
    }
}

float Canvas::GetScaleRatio() const
{
    const float width_ratio = width_ / static_cast<float>(reference_resolution_width_);
    const float height_ratio = height_ / static_cast<float>(reference_resolution_height_);

    return width_ratio * (1.f - match_mode_) + height_ratio * match_mode_;
}

Widget* Canvas::RayCast(Widget* widget, const Math::Vector2& kPoint)
{
    if (!widget) return nullptr;
    for (auto it = widget->children_.rbegin(); it != widget->children_.rend(); ++it)
    {
        Widget* child = *it;
        if (child->HitTest(kPoint)) return RayCast(child, kPoint);
    }

    if (widget->HitTest(kPoint)) return widget;
    return nullptr;
}

Widget* Canvas::FindWidgetParentOfType(Widget* widget, const rttr::type& kType)
{
    if (!widget) return nullptr;
    
    Widget* parent = widget->parent_;
    if (!parent) return nullptr;

    rttr::type parent_type = rttr::type::get(*parent);
    if (parent_type.is_derived_from(kType)) return parent;
    return FindWidgetParentOfType(parent, kType);
}

bool Canvas::IsWidgetType(Widget* widget, const rttr::type& kType)
{
    if (!widget) return false;

    rttr::type widget_type = rttr::type::get(*widget);
    return widget_type.is_derived_from(kType);
}

void Canvas::OnEvent(const Event& kEvent)
{
    const Type::uint32& type = kEvent.type;

    if (type == static_cast<Type::uint32>(EventType::kWindowSize))
    {
        width_ = kEvent.window.data1;
        height_ = kEvent.window.data2;

        for (const auto& widget : widgets_)
        {
            widget->UpdateRect();
        }
    }
    else if (type == static_cast<Type::uint32>(EventType::kMouseMotion))
    {
        Math::Vector2 mouse_position = {kEvent.motion.x, kEvent.motion.y};
        Math::Vector2 mouse_delta = mouse_position - mouse_position_;

        if (root_widget_)
        {
            root_widget_->OnMouseMotion(mouse_position, mouse_delta);
        }
        
        mouse_position_ = mouse_position;
    }
    else if (type & static_cast<Type::uint32>(EventType::kMousePressed | EventType::kMouseReleased))
    {
        const MouseButtonEvent& kButton = kEvent.button;
        if (root_widget_)
        {
            root_widget_->OnMouseButton({kButton.x, kButton.y}, kButton.button, kButton.is_pressed);
        }
    }
    else if (type == static_cast<Type::uint32>(EventType::kMouseWheel))
    {
        const MouseWheelEvent& kWheel = kEvent.wheel;
        if (root_widget_)
        {
            root_widget_->OnScroll({kWheel.mouse_x, kWheel.mouse_y}, {kWheel.x, kWheel.y});
        }
    }
}

void Canvas::BeginPlay()
{
    if (root_widget_)
    {
        root_widget_->BeginPlay();
    }
}

void Canvas::Tick(float delta_time)
{
}

void Canvas::Render()
{
    if (root_widget_)
    {
        root_widget_->Render();
    }
}

void Canvas::Clear()
{
    mouse_position_ = Math::Vector2::Zero();
    
    root_widget_ = nullptr;
    
    widgets_.clear();
}
