#include "pch.h"
#include "UIContainer.h"

void UIContainer::RemoveChild(UIElement* child)
{
    for (auto it = children_.begin(); it != children_.end(); ++it)
    {
        if (it->get() == child)
        {
            if (is_initialized_) child->Uninit();
            children_.erase(it);
            return;
        }
    }
}

void UIContainer::SetActive(bool active)
{
    // for (uint32_t i = 0; i < children_.size(); ++i)
    // {
    //     UIElement* child = children_[children_.size() - i - 1].get();
    //     if (child) child->SetActive(active);
    // }
    
    UIElement::SetActive(active);
}

UIContainer::UIContainer(const std::wstring& name) :
    UIElement(name),
    children_()
{
}

void UIContainer::Init()
{
    for (auto& child : children_)
    {
        if (child) child->Init();
    }
    
    UIElement::Init();
}

void UIContainer::Uninit()
{
    for (auto& child : children_)
    {
        if (child) child->Uninit();
    }
    
    UIElement::Uninit();
}

void UIContainer::Tick(float delta_time)
{
    UIElement::Tick(delta_time);

    for (auto& child : children_)
    {
        if (child && child->IsActive()) child->Tick(delta_time);
    }
}

void UIContainer::Render()
{
    UIElement::Render();

    for (const auto& child : children_)
    {
        if (child && child->IsActive()) child->Render();
    }
}

UIElement* UIContainer::RayCast(const Math::Vector2& position)
{
    for (uint32_t i = 0; i < children_.size(); ++i)
    {
        UIElement* child = children_[children_.size() - i - 1].get();
        if (child && child->IsActive() && child->IsInRange(position))
            return child->RayCast(position);
    }

    return UIElement::RayCast(position);
}

bool UIContainer::OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta)
{
    bool result = UIElement::OnMouseMotion(position, delta);

    for (uint32_t i = 0; i < children_.size(); ++i)
    {
        UIElement* child = children_[children_.size() - i - 1].get();
        if (!child || !child->IsActive()) continue;
        
        bool is_in_range = child->IsInRange(position);
        bool was_in_range = child->IsInRange(position - delta);

        if (is_in_range && !was_in_range) result |= child->OnMouseEnter();
        if (!is_in_range && was_in_range)
        {
            result |= child->OnMouseLeave();
            if (result) return result;
        }

        if (is_in_range || was_in_range)
            result |= child->OnMouseMotion(position, delta);

        if (result) return result;
    }

    return result;
}

bool UIContainer::OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp)
{
    for (uint32_t i = 0; i < children_.size(); ++i)
    {
        UIElement* child = children_[children_.size() - i - 1].get();
        if (child && child->IsActive() && child->IsInRange(position))
        {
            if (child->OnMouseButton(position, button, is_pressed, timestamp)) return true;
        }
    }

    return UIElement::OnMouseButton(position, button, is_pressed, timestamp);
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

bool UIContainer::OnKey(uint32_t scancode, bool is_pressed)
{
    return UIElement::OnKey(scancode, is_pressed);
}

bool UIContainer::OnChar(wchar_t character)
{
    return UIElement::OnChar(character);
}

void UIContainer::MakeDirty()
{
    UIElement::MakeDirty();

    for (auto& child : children_)
    {
        if (child) child->MakeDirty();
    }
}

UIElement* UIContainer::AddChild_Internal(const rttr::type& type, const std::wstring& name)
{
    rttr::variant var = type.create({ name });
    if (var.is_valid())
    {
        UIElement* child = var.get_value<UIElement*>();
        child->parent_ = this;
        if (is_initialized_) child->Init();
        
        children_.push_back(std::unique_ptr<UIElement>(child));
        return child;
    }
    
    return nullptr;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIContainer>("UIContainer")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
