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

    if (type == static_cast<Type::uint32>(EventType::kMousePressed))
    {
        if (kEvent.button.button == MouseButton::kLeft)
        {
            if (hovered_widget_)
            {
                previous_mouse_position_ = {kEvent.button.x, kEvent.button.y};
                
                dragging_widget_ = hovered_widget_;
                dragging_widget_->on_drag_start.Execute(previous_mouse_position_);
            }
        }
    }
    else if (type == static_cast<Type::uint32>(EventType::kMouseReleased))
    {
        if (dragging_widget_)
        {
            previous_mouse_position_ = {kEvent.button.x, kEvent.button.y};
            
            dragging_widget_->on_drag_end.Execute(previous_mouse_position_);
            dragging_widget_ = nullptr;
        }
    }
    else if (type == static_cast<Type::uint32>(EventType::kMouseMotion))
    {
        if (dragging_widget_)
        {
            const Math::Vector2 mouse_position = {kEvent.motion.x, kEvent.motion.y};
            
            Math::Vector2 delta = mouse_position - previous_mouse_position_;
            previous_mouse_position_ = mouse_position;
            
            dragging_widget_->on_drag.Execute(delta);
        }
    }
}

void Canvas::BeginPlay()
{
    for (const auto& ui : widgets_)
    {
        ui->BeginPlay();
    }
}

void Canvas::Tick(float delta_time)
{
    Mouse* mouse = Mouse::Get();
    const Math::Vector2 mouse_position = mouse->GetMousePosition();
    
    Widget* hovered_widget = nullptr;
    for (const auto& widget : widgets_)
    {
        if (Math::Rect::Contains(widget->rect_, mouse_position))
        {
            if (!hovered_widget) hovered_widget = widget.get();
            else if (widget->z_index_ > hovered_widget->z_index_) hovered_widget = widget.get();
        }
    }

    hovered_widget_ = hovered_widget;
    
    for (const auto& ui : widgets_)
    {
        ui->Tick(delta_time);
    }
}

void Canvas::Render()
{
    for (const auto& ui : widgets_)
    {
        ui->Render();
    }
}

void Canvas::Clear()
{
    widgets_.clear();
}
