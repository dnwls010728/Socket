#include "pch.h"
#include "UIContainer.h"

#include "UIState.h"

void UIContainer::RemoveChild(UIElement* child)
{
    if (!child) return;

    child->is_pending_removal_ = true;
    pending_remove_children_.push(child);
    --child_count_;
}

void UIContainer::GetChildren(std::vector<UIElement*>& children) const
{
    for (const auto& child : children_)
    {
        auto* element = child.get();
        if (IsValid(element)) children.push_back(element);
    }
}

UIContainer::UIContainer(const std::wstring& name) :
    UIElement(name),
    children_(),
    active_children_(),
    pending_add_children_(),
    pending_remove_children_(),
    child_count_(0)
{
}

void UIContainer::Tick(float delta_time)
{
    UIElement::Tick(delta_time);

    for (auto& child : active_children_)
    {
        if (child && child->IsActive()) child->Tick(delta_time);
    }

    RemoveChildren();
}

void UIContainer::Render()
{
    UIElement::Render();

    for (const auto& child : active_children_)
    {
        if (child && child->IsActive()) child->Render();
    }
}

UIElement* UIContainer::RayCast(const Math::Vector2& position)
{
    for (uint32_t i = 0; i < active_children_.size(); ++i)
    {
        UIElement* child = active_children_[active_children_.size() - i - 1];
        if (child && child->IsActive() && child->IsInRange(position))
            return child->RayCast(position);
    }

    return UIElement::RayCast(position);
}

bool UIContainer::OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta)
{
    bool result = UIElement::OnMouseMotion(position, delta);

    for (uint32_t i = 0; i < active_children_.size(); ++i)
    {
        UIElement* child = active_children_[active_children_.size() - i - 1];
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
    for (uint32_t i = 0; i < active_children_.size(); ++i)
    {
        UIElement* child = active_children_[active_children_.size() - i - 1];
        if (child && child->IsActive() && child->IsInRange(position))
        {
            if (child->OnMouseButton(position, button, is_pressed, timestamp)) return true;
        }
    }

    return UIElement::OnMouseButton(position, button, is_pressed, timestamp);
}

bool UIContainer::OnScroll(const Math::Vector2& position, const Math::Vector2& delta)
{
    for (uint32_t i = 0; i < active_children_.size(); ++i)
    {
        UIElement* child = active_children_[active_children_.size() - i - 1];
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

        children_.push_back(std::unique_ptr<UIElement>(child));
        pending_add_children_.push(child);
        return child;
    }
    
    return nullptr;
}

void UIContainer::AddChildren()
{
    for (const auto& child : children_)
    {
        if (auto* container = dynamic_cast<UIContainer*>(child.get()))
            container->AddChildren();
    }
    
    while (!pending_add_children_.empty())
    {
        auto& child = pending_add_children_.front();
        child->Init();
        
        active_children_.push_back(child);
        pending_add_children_.pop();

        ++child_count_;
    }
}

void UIContainer::RemoveChildren()
{
    for (const auto& child : children_)
    {
        if (auto* container = dynamic_cast<UIContainer*>(child.get()))
            container->RemoveChildren();
    }
    
    while (!pending_remove_children_.empty())
    {
        auto* child = pending_remove_children_.front();
        pending_remove_children_.pop();

        if (auto* state = UI::Get()->GetState())
        {
            state->ClearFocus(child);
            state->ClearDrag(child);
        }

        {
            auto it = std::ranges::find(active_children_, child);
            if (it != active_children_.end()) active_children_.erase(it);
        }

        {
            auto it = std::ranges::find_if(children_, [&](const std::unique_ptr<UIElement>& e) { return e.get() == child; });
            if (it != children_.end()) children_.erase(it);
        }
    }
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
