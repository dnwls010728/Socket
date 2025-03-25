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

void UI::Manager::AddWidget(const std::shared_ptr<Widget>& widget)
{
    widgets_.push_back(widget);
}

void UI::Manager::RemoveWidget(const std::shared_ptr<Widget>& widget)
{
    std::erase(widgets_, widget);
}

void UI::Manager::OnEvent(const Event& kEvent)
{
    const Type::uint32& kType = kEvent.type;

    if (kType & static_cast<Type::uint32>(EventType::kMouseChanged))
    {
        const MouseButtonEvent& kButton = kEvent.button;
        const Math::Vector2& kMousePosition = {kButton.x, kButton.y};

        bool is_handled = false;
        for (Type::uint64 i = 0; i < widgets_.size(); ++i)
        {
            Widget* widget = widgets_[widgets_.size() - i - 1].get();
            
            const std::vector<std::shared_ptr<Widget>>& kChildren = widget->GetChildren();
            for (Type::uint64 j = 0; j < kChildren.size(); ++j)
            {
                Widget* child = kChildren[kChildren.size() - j - 1].get();
                if (child->Contains(kMousePosition) && child->OnMouseButton(kMousePosition, kButton.button, kButton.is_pressed))
                {
                    is_handled = true;
                    break;
                }
            }

            if (is_handled) break;
            if (widget->Contains(kMousePosition) && widget->OnMouseButton(kMousePosition, kButton.button, kButton.is_pressed))
            {
                break;
            }
        }
    }
}
