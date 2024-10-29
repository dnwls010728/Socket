#include "pch.h"
#include "Canvas.h"

#include "Logger.h"
#include "Widget.h"

Canvas::Canvas() :
    width_(0.f),
    height_(0.f),
    reference_resolution_width_(ProjectSettings::kCanvasReferenceWidth),
    reference_resolution_height_(ProjectSettings::kCanvasReferenceHeight),
    match_mode_(ProjectSettings::kMatchMode),
    widgets_(),
    root_widget_(nullptr),
    hovered_widget_(nullptr),
    dragging_widget_(nullptr),
    previous_mouse_position_(Math::Vector2::Zero())
{
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
    for (const auto& child : widget->GetChildren())
    {
        Widget* result = RayCast(child, kPoint);
        if (result) return result;
    }

    if (widget->HitTest(kPoint)) return widget;
    return nullptr;
}

void Canvas::OnResize(Type::uint32 width, Type::uint32 height)
{
    width_ = width;
    height_ = height;

    for (const auto& widget : widgets_)
    {
        widget->UpdateRect();
    }
}

void Canvas::OnEvent(const Event& kEvent)
{
    const Type::uint32& type = kEvent.type;
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
    Mouse* mouse = Mouse::Get();
    Math::Vector2 mouse_position = mouse->GetMousePosition();
    
    if (root_widget_)
    {
        Widget* widget = RayCast(root_widget_, mouse_position);
        if (widget)
        {
            Logger::Print(L"HIT: %s", widget->name_.c_str());
        }
        
        root_widget_->Tick(delta_time);
    }
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
    widgets_.clear();
}
