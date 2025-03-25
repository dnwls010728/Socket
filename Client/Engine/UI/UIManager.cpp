#include "pch.h"
#include "UIManager.h"

#include "Logger.h"
#include "Widget.h"
#include "Event/Events.h"

UI::Manager::Manager()
{
}

void UI::Manager::Tick(float delta_time)
{
    for (Type::uint64 i = 0; i < widgets_.size(); ++i)
    {
        Widget* widget = widgets_[i].get();
        widget->Tick(delta_time);
    }
}

void UI::Manager::Render()
{
    for (Type::uint64 i = 0; i < widgets_.size(); ++i)
    {
        Widget* widget = widgets_[widgets_.size() - i - 1].get();
        widget->Render();
    }
}

void UI::Manager::AddWidget(std::shared_ptr<Widget> widget)
{
    widgets_.push_back(widget);
    Logger::Print(L"New widget added.");
}

void UI::Manager::RemoveWidget(std::shared_ptr<Widget> widget)
{
    // TODO: 테스트 필요
    for (auto& x : widgets_)
    {
        if (x == widget)
        {
            x = widgets_.back();
            widgets_.pop_back();
            break;
        }
    }
}

void UI::Manager::OnEvent(const Event& kEvent)
{
    const Type::uint32& kType = kEvent.type;

    Logger::Print(L"UI Manager received event: %d", kType);
}
