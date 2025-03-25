#include "pch.h"
#include "UIManager.h"

#include "Logger.h"
#include "Widget.h"
#include "Event/Events.h"

UI::Manager::Manager() :
    widgets_(),
    last_mouse_position_(Math::Vector2::Zero())
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

    if (kType == static_cast<Type::uint32>(EventType::kMouseMotion))
    {
        const Math::Vector2 kMousePosition = {kEvent.motion.x, kEvent.motion.y};
        const Math::Vector2 kMouseDelta = kMousePosition - last_mouse_position_;

        bool is_handled = false;
        for (Type::uint64 i = 0; i < widgets_.size(); ++i)
        {
            Widget* widget = widgets_[widgets_.size() - i - 1].get();
            
            const std::vector<std::shared_ptr<Widget>>& kChildren = widget->GetChildren();
            for (Type::uint64 j = 0; j < kChildren.size(); ++j)
            {
                Widget* child = kChildren[kChildren.size() - j - 1].get();

                bool is_result = child->Contains(kMousePosition);
                bool is_previous_result = child->Contains(kMousePosition - kMouseDelta);
                
                if (is_result && !is_previous_result) is_handled |= child->OnMouseEnter();
                if (!is_result && is_previous_result)
                {
                    is_handled |= child->OnMouseLeave();
                    break;
                }

                if (is_result || is_previous_result) is_handled |= child->OnMouseMotion(kMousePosition, kMouseDelta);
                if (is_handled) break;
            }

            if (is_handled) break;

            bool is_result = widget->Contains(kMousePosition);
            bool is_previous_result = widget->Contains(kMousePosition - kMouseDelta);

            if (is_result && !is_previous_result) is_handled |= widget->OnMouseEnter();
            if (!is_result && is_previous_result)
            {
                widget->OnMouseLeave();
                break;
            }

            if (is_result || is_previous_result) is_handled |= widget->OnMouseMotion(kMousePosition, kMouseDelta);
            if (is_handled) break;
        }
        
        last_mouse_position_ = kMousePosition;
    }
    else if (kType & static_cast<Type::uint32>(EventType::kMouseChanged))
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
