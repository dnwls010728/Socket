#include "pch.h"
#include "UIManager.h"

#include "Widget.h"
#include "Event/Events.h"
#include "Windows/DX/Renderer.h"

UI::Manager::Manager() :
    widgets_(),
    focused_widget_(),
    dragging_widget_(),
    last_mouse_position_(Math::Vector2::Zero()),
    is_dragging_(false),
    has_begun_drag_(false)
{
}

void UI::Manager::AddToViewport(const std::shared_ptr<Widget>& kWidget)
{
    if (!kWidget) return;
    widgets_.push_back(kWidget);
}

void UI::Manager::RemoveFromViewport(const std::shared_ptr<Widget>& kWidget)
{
    if (!kWidget) return;
    std::erase(widgets_, kWidget);
}

void UI::Manager::SetFocus(const std::shared_ptr<Widget>& kWidget)
{
    if (const std::shared_ptr<Widget> widget_ptr = focused_widget_.lock()) widget_ptr->OnFocus(false);

    focused_widget_ = kWidget;
    if (kWidget) kWidget->OnFocus(true);
}

bool UI::Manager::IsInViewport(const std::shared_ptr<Widget>& kWidget)
{
    return std::ranges::find(widgets_, kWidget) != widgets_.end();
}

std::shared_ptr<UI::Widget> UI::Manager::RayCast(const Math::Vector2& kPosition) const
{
    for (Type::uint64 i = 0; i < widgets_.size(); ++i)
    {
        Widget* widget = widgets_[widgets_.size() - i - 1].get();
        if (widget->Contains(kPosition)) return widget->GetSharedThis();
    }
    
    return nullptr;
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
        widget->Render(Renderer::Get(), World::Get()->GetWindow());
    }
}

void UI::Manager::OnEvent(const Event& kEvent)
{
    const Type::uint32& kType = kEvent.type;

    if (kType == static_cast<Type::uint32>(EventType::kMouseMotion))
    {
        const Math::Vector2 kMousePosition = {kEvent.motion.x, kEvent.motion.y};
        const Math::Vector2 kMouseDelta = kMousePosition - last_mouse_position_;

        bool is_handled = false;
        if (is_dragging_)
        {
            std::shared_ptr<Widget> dragging_widget = dragging_widget_.lock();
            if (dragging_widget)
            {
                if (!has_begun_drag_)
                {
                    is_handled |= dragging_widget->OnDragBegin(kMousePosition);
                    has_begun_drag_ = true;
                }
                else is_handled |= dragging_widget->OnDrag(kMousePosition, kMouseDelta);
            }
        }

        if (!is_handled)
        {
            for (Type::uint64 i = 0; i < widgets_.size(); ++i)
            {
                Widget* widget = widgets_[widgets_.size() - i - 1].get();

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
        }
        
        last_mouse_position_ = kMousePosition;
    }
    else if (kType & static_cast<Type::uint32>(EventType::kMouseChanged))
    {
        const MouseButtonEvent& kButton = kEvent.button;
        const Math::Vector2& kMousePosition = {kButton.x, kButton.y};

        if (kButton.button == MouseButton::kLeft && !kButton.is_pressed)
        {
            std::shared_ptr<Widget> dragging_widget = dragging_widget_.lock();
            if (is_dragging_ && dragging_widget)
            {
                std::shared_ptr<Widget> drop_widget = RayCast(kMousePosition);
                if (drop_widget && drop_widget != dragging_widget)
                {
                    drop_widget->OnDrop(dragging_widget, kMousePosition);
                }

                if (has_begun_drag_)
                {
                    dragging_widget->OnDragEnd(kMousePosition);
                    has_begun_drag_ = false;
                }

                is_dragging_ = false;
                dragging_widget_.reset();
            }
        }
        else if (kButton.button == MouseButton::kLeft && kButton.is_pressed)
        {
            if (!is_dragging_)
            {
                std::shared_ptr<Widget> drag_widget = RayCast(kMousePosition);
                dragging_widget_ = drag_widget;
                is_dragging_ = drag_widget != nullptr;
            }
        }

        bool is_handled = false;
        for (Type::uint64 i = 0; i < widgets_.size(); ++i)
        {
            Widget* widget = widgets_[widgets_.size() - i - 1].get();
            if (widget->Contains(kMousePosition) && widget->OnMouseButton(kMousePosition, kButton.button, kButton.is_pressed, kButton.timestamp))
            {
                is_handled = true;
                break;
            }
        }

        if (!is_handled) SetFocus(nullptr);
    }
    else if (kType == static_cast<Type::uint32>(EventType::kMouseWheel))
    {
        const MouseWheelEvent& kWheel = kEvent.wheel;
        const Math::Vector2 kMousePosition = {kWheel.mouse_x, kWheel.mouse_y};
        const Math::Vector2 kMouseDelta = {kWheel.x, kWheel.y};

        for (Type::uint64 i = 0; i < widgets_.size(); ++i)
        {
            Widget* widget = widgets_[widgets_.size() - i - 1].get();
            if (widget->Contains(kMousePosition) && widget->OnScroll(kMousePosition, kMouseDelta)) break;
        }
    }
    else if (kType & static_cast<Type::uint32>(EventType::kKeyChanged))
    {
        const KeyboardEvent& kKey = kEvent.key;
        if (std::shared_ptr<Widget> widget = focused_widget_.lock())
        {
            if (widget->IsFocused() && widget->OnKey(kKey.key_code, kKey.is_repeat))
            {
            }
        }
    }
    else if (kType == static_cast<Type::uint32>(EventType::kText))
    {
        if (std::shared_ptr<Widget> widget = focused_widget_.lock())
        {
            if (widget->IsFocused() && widget->OnChar(kEvent.text.character))
            {
            }
        }
    }
}
