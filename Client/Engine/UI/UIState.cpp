#include "pch.h"
#include "UIState.h"

UIState::UIState() :
    elements_()
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

    for (uint32_t i = 0; i < elements_.size(); ++i)
    {
        UIElement* element = elements_[elements_.size() - i - 1].get();
        if (element && element->IsActive() && element->IsInRange(position))
        {
            result = element->OnMouseButton(position, button, is_pressed, timestamp);
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
    return false;
}

bool UIState::OnChar(wchar_t character)
{
    return false;
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
