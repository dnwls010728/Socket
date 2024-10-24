#include "pch.h"
#include "Canvas.h"

#include "Widget.h"

Canvas::Canvas() :
    width_(0.f),
    height_(0.f),
    reference_resolution_width_(ProjectSettings::kCanvasReferenceWidth),
    reference_resolution_height_(ProjectSettings::kCanvasReferenceHeight),
    match_mode_(ProjectSettings::kMatchMode),
    widgets_()
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
