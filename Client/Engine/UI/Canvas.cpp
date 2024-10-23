#include "pch.h"
#include "Canvas.h"

#include "Logger.h"
#include "Widget.h"
#include "Event/Events.h"
#include "Input/Mouse.h"
#include "Widget/Button.h"
#include "Widget/TextBox.h"

Canvas::Canvas() :
    width_(0.f),
    height_(0.f),
    reference_resolution_width_(ProjectSettings::kCanvasReferenceWidth),
    reference_resolution_height_(ProjectSettings::kCanvasReferenceHeight),
    match_mode_(ProjectSettings::kMatchMode),
    widgets_(),
    hovered_widget_(nullptr),
    focused_widget_(nullptr)
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
    switch (kEvent.type)
    {
    case EventType::kKeyPressed:
        {
            if (focused_widget_) focused_widget_->OnKeyEvent(kEvent.key.key_code, true);
        }
        break;
    case EventType::kKeyReleased:
        {
            if (focused_widget_) focused_widget_->OnKeyEvent(kEvent.key.key_code, false);
        }
        break;
    case EventType::kText:
        {
            if (focused_widget_) focused_widget_->OnCharEvent(kEvent.text.character);
        }
        break;
    case EventType::kMousePressed:
        {
            if (kEvent.button.button == MouseButton::kLeft)
            {
                if (hovered_widget_)
                {
                    if (focused_widget_ != hovered_widget_)
                    {
                        if (focused_widget_) focused_widget_->OnBlur();
                        focused_widget_ = hovered_widget_;
                        focused_widget_->OnFocus();
                    }
                }
                else
                {
                    if (focused_widget_)
                    {
                        focused_widget_->OnBlur();
                        focused_widget_ = nullptr;
                    }
                }

                if (hovered_widget_)
                {
                    hovered_widget_->OnMousePressed();
                }
            }
        }
        break;
    case EventType::kMouseReleased:
        {
            if (kEvent.button.button == MouseButton::kLeft)
            {
                if (hovered_widget_ && focused_widget_ == hovered_widget_)
                {
                    hovered_widget_->OnMouseReleased();
                }
            }
        }
        break;
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
    if (!mouse) return;

    Math::Vector2 mouse_position = mouse->GetMousePosition();
    
    Widget* hovered_widget = nullptr;
    for (auto it = widgets_.rbegin(); it != widgets_.rend(); ++it)
    {
        Widget* widget = it->get();
        if (widget->rect_.Contains(mouse_position))
        {
            rttr::type type = rttr::type::get(*widget);
            if (type.is_derived_from<UI::Button>() || type.is_derived_from<UI::TextBox>())
            {
                if (!hovered_widget || widget->z_index_ > hovered_widget->z_index_)
                {
                    hovered_widget = widget;
                }
            }
        }
    }

    if (hovered_widget_ != hovered_widget)
    {
        if (hovered_widget_) hovered_widget_->OnMouseLeave();
        hovered_widget_ = hovered_widget;
        if (hovered_widget_) hovered_widget->OnMouseHover();
    }

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
    hovered_widget_ = nullptr;
    focused_widget_ = nullptr;
    widgets_.clear();
}
