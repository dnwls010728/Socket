#include "pch.h"
#include "UIState.h"

#include "UI/UIContainer.h"

UIState::UIState() :
    elements_(),
    focus_path_(),
    is_dragging_(false),
    has_begun_drag_(false),
    dragging_element_(nullptr)
{
}

void UIState::Tick(float delta_time)
{
    for ( uint32_t i = 0; i < elements_.size(); ++i )
    {
        UIElement* element = elements_[i].get();
        if (element && element->IsActive())
            element->Tick(delta_time);
    }
}

void UIState::Render()
{
    for (uint32_t i = 0; i < elements_.size(); ++i)
    {
        UIElement* element = elements_[i].get();
        if (element && element->IsActive())
            element->Render();
    }
}

UI::MouseEventResult UIState::OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta)
{
    UI::MouseEventResult result = { false, UI::CursorState::kIdle };

    if (is_dragging_ && dragging_element_)
    {
        if (has_begun_drag_)
        {
            result.is_handled = dragging_element_->OnDrag(position, delta);
            if (result.is_handled) return result;
        }
        else
        {
            result.is_handled = dragging_element_->OnDragBegin(position);
            has_begun_drag_ = true;
            if (result.is_handled) return result;
        }
    }

    for (uint32_t i = 0; i < elements_.size(); ++i)
    {
        UIElement* element = elements_[elements_.size() - i - 1].get();
        if (element && element->IsActive() && element->IsInRange(position))
        {
            result = element->OnMouseMotion(position, delta);
            if (result.is_handled) return result;
        }
    }

    return result;
}

UI::MouseEventResult UIState::OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp)
{
    UI::MouseEventResult result = { false, UI::CursorState::kIdle };

    if (is_dragging_ && button == MouseButton::kLeft && !is_pressed)
    {
        if (dragging_element_)
        {
            UIElement* target_element = FindElement(position);
            if (target_element && target_element != dragging_element_)
            {
                result.is_handled |= target_element->OnDrop(position, dragging_element_);
            }

            if (has_begun_drag_)
            {
                result.is_handled |= dragging_element_->OnDragEnd(position);
                has_begun_drag_ = false;
            }
        }
        
        is_dragging_ = false;
        dragging_element_ = nullptr;
    }
    else if (!is_dragging_ && button == MouseButton::kLeft && is_pressed)
    {
        dragging_element_ = FindElement(position);
        is_dragging_ = dragging_element_ != nullptr;
        if (!is_dragging_) UpdateFocus(nullptr);
    }

    for (uint32_t i = 0; i < elements_.size(); ++i)
    {
        UIElement* element = elements_[elements_.size() - i - 1].get();
        if (element && element->IsActive() && element->IsInRange(position))
        {
            UI::MouseEventResult temp_result = element->OnMouseButton(position, button, is_pressed, timestamp);
            result.is_handled |= temp_result.is_handled;
            result.cursor_state = temp_result.cursor_state;
            if (result.is_handled) return result;
        }
    }
    
    return result;
}

bool UIState::OnScroll(const Math::Vector2& position, const Math::Vector2& delta)
{
    for ( uint32_t i = 0; i < elements_.size(); ++i )
    {
        UIElement* element = elements_[elements_.size() - i - 1].get();
        if (element && element->IsActive() && element->IsInRange(position) && element->OnScroll(position, delta))
            return true;
    }
    
    return false;
}

bool UIState::OnKey(uint16_t key_code, bool is_pressed)
{
    for (uint32_t i = 0; i < focus_path_.size(); ++i)
    {
        UIElement* element = focus_path_[focus_path_.size() - i - 1];
        if (element && element->IsFocused() && element->OnKey(key_code, is_pressed))
            return true;
    }

    return false;
}

bool UIState::OnChar(wchar_t character)
{
    for (uint32_t i = 0; i < focus_path_.size(); ++i)
    {
        UIElement* element = focus_path_[focus_path_.size() - i - 1];
        if (element && element->IsFocused() && element->OnChar(character))
            return true;
    }

    return false;
}

UIElement* UIState::FindElement(const Math::Vector2& position) const
{
    for (uint32_t i = 0; i < elements_.size(); ++i)
    {
        UIElement* element = elements_[elements_.size() - i - 1].get();
        if (element && element->IsActive() && element->IsInRange(position))
            return element->FindElement(position);
    }

    return nullptr;
}

void UIState::UpdateFocus(UIElement* element)
{
    for (const auto& e : focus_path_)
    {
        if (!e->IsFocused()) continue;
        e->OnFocus(false);
    }

    focus_path_.clear();

    if (element)
    {
        focus_path_.push_back(element);
        
        UIContainer* parent = element->parent_;
        while (parent)
        {
            focus_path_.push_back(parent);
            parent = parent->parent_;
        }

        for (auto it = focus_path_.rbegin(); it != focus_path_.rend(); ++it)
        {
            (*it)->OnFocus(true);
        }
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIState>("UIState")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
