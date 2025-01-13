#include "pch.h"
#include "Canvas.h"

#include "Widget.h"

UI::Canvas::Canvas()
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
