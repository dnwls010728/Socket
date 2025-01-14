#include "pch.h"
#include "Canvas.h"

#include "Widget.h"

UI::Canvas::Canvas() :
    widgets_(),
    pending_widgets_(),
    pending_detach_widgets_()
{
}

void UI::Canvas::OnEvent(const Event& kEvent)
{
}

void UI::Canvas::Render()
{
    for (const auto& widget : widgets_)
    {
        widget->Render();
    }
}

void UI::Canvas::AttachWidget(const std::shared_ptr<Widget>& widget)
{
    pending_widgets_.push_back(widget);
}

void UI::Canvas::DetachWidget(const std::shared_ptr<Widget>& widget)
{
    pending_detach_widgets_.push_back(widget);
}

void UI::Canvas::AttachWidgets()
{
    for (const auto& widget : pending_widgets_)
    {
        widgets_.push_back(widget);
    }
    pending_widgets_.clear();
}

void UI::Canvas::DetachWidgets()
{
    for (const auto& widget : pending_detach_widgets_)
    {
        std::erase(widgets_, widget);
    }
    pending_detach_widgets_.clear();
}
