#include "pch.h"
#include "UIContainer.h"

UIContainer::UIContainer() :
    children_()
{
}

void UIContainer::Tick(float delta_time)
{
    UIElement::Tick(delta_time);

    for (auto& child : children_)
    {
        if (child && child->IsActive()) child->Tick(delta_time);
    }
}

void UIContainer::Render(const Math::Vector2& parent_position)
{
    UIElement::Render(parent_position);
    
    for (const auto& child : children_)
    {
        if (child && child->IsActive()) child->Render(parent_position);
    }
}

UI::MouseEventResult UIContainer::OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta)
{
    UI::MouseEventResult result = UIElement::OnMouseMotion(position, delta);

    for (uint32_t i = 0; i < children_.size(); ++i)
    {
        UIElement* child = children_[children_.size() - i - 1].get();
        if (child && child->IsActive() && child->IsInRange(position))
        {
            result = child->OnMouseMotion(position, delta);
            if (result.is_handled) return result;
        }
    }

    return result;
}

UI::MouseEventResult UIContainer::OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp)
{
    UI::MouseEventResult result = UIElement::OnMouseButton(position, button, is_pressed, timestamp);

    for (uint32_t i = 0; i < children_.size(); ++i)
    {
        UIElement* child = children_[children_.size() - i - 1].get();
        if (child && child->IsActive() && child->IsInRange(position))
        {
            result = child->OnMouseButton(position, button, is_pressed, timestamp);
            if (result.is_handled) return result;
        }
    }

    return result;
}

bool UIContainer::OnScroll(const Math::Vector2& position, const Math::Vector2& delta)
{
    for (uint32_t i = 0; i < children_.size(); ++i)
    {
        UIElement* child = children_[children_.size() - i - 1].get();
        if (child && child->IsActive() && child->IsInRange(position) && child->OnScroll(position, delta))
            return true;
    }
    
    return UIElement::OnScroll(position, delta);
}

bool UIContainer::OnKey(uint16_t key_code, bool is_pressed)
{
    return UIElement::OnKey(key_code, is_pressed);
}

bool UIContainer::OnChar(wchar_t character)
{
    return UIElement::OnChar(character);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIContainer>("UIContainer")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
