#include "pch.h"
#include "UIState.h"

#include "Element/UIEditableText.h"
#include "UI/UIContainer.h"

UIState::UIState() :
    elements_(),
    focus_path_(),
    is_initialized_(false),
    is_dragging_(false),
    has_begun_drag_(false),
    dragging_element_(nullptr)
{
}

void UIState::RemoveElement(UIElement* element)
{
    if (!element) return;
    
    bool touches_focus = std::any_of(focus_path_.begin(), focus_path_.end(),
        [&](UIElement* f){
            return f == element || f->IsDescendantOf(element);
        });
    if (touches_focus) {
        UpdateFocus(nullptr);
    }
    
    if (dragging_element_ &&
        (dragging_element_ == element || dragging_element_->IsDescendantOf(element)))
    {
        if (has_begun_drag_) {
            dragging_element_->OnDragEnd({});
            has_begun_drag_ = false;
        }
        is_dragging_ = false;
        dragging_element_ = nullptr;
    }
    
    if (!pending_elements_.empty()) {
        pending_elements_.erase(
            std::remove(pending_elements_.begin(), pending_elements_.end(), element),
            pending_elements_.end()
        );
    }
    for (auto it = elements_.begin(); it != elements_.end(); ++it)
    {
        if (it->get() == element)
        {
            if (is_initialized_) element->Uninit();
            elements_.erase(it);
            break;
        }
    }
}

void UIState::SetFocus(UIElement* element)
{
    UpdateFocus(element);
}

UIElement* UIState::RayCast(const Math::Vector2& position) const
{
    for (uint32_t i = 0; i < elements_.size(); ++i)
    {
        UIElement* element = elements_[elements_.size() - i - 1].get();
        if (element && element->IsActive() && element->IsInRange(position))
            return element->RayCast(position);
    }

    return nullptr;
}

bool UIState::IsFocused() const
{
    return !focus_path_.empty();
}

bool UIState::IsEditingText() const
{
    for (const auto& element : focus_path_)
    {
        rttr::type element_type = rttr::type::get(*element);
        if (element_type == UIEditableText::StaticClass() ||
           element_type.is_derived_from(UIEditableText::StaticClass()))
            return true;
    }

    return false;
}

void UIState::PostTask(Function<void()> task)
{
    pending_tasks_.push(std::move(task));
}

void UIState::Init()
{
    ProcessPending();
    is_initialized_ = true;
}


void UIState::Uninit()
{
    for ( uint32_t i = 0; i < elements_.size(); ++i )
    {
        UIElement* element = elements_[i].get();
        if (element) element->Uninit();
    }
}

void UIState::Tick(float delta_time)
{
    ProcessPending();
    
    while (!pending_tasks_.empty())
    {
        pending_tasks_.front()();
        pending_tasks_.pop();
    }
    
    for ( uint32_t i = 0; i < elements_.size(); ++i )
    {
        UIElement* element = elements_[i].get();
        if (element && element->is_initialized_ && element->IsActive())
            element->Tick(delta_time);
    }
}

void UIState::Render()
{
    for (uint32_t i = 0; i < elements_.size(); ++i)
    {
        UIElement* element = elements_[i].get();
        if (element && element->is_initialized_ && element->IsActive())
            element->Render();
    }
}

bool UIState::OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta)
{
    bool result = false;

    if (is_dragging_ && dragging_element_)
    {
        if (has_begun_drag_)
        {
            result = dragging_element_->OnDrag(position, delta);
            if (result) return result;
        }
        else
        {
            result = dragging_element_->OnDragBegin(position);
            has_begun_drag_ = true;
            if (result) return result;
        }
    }

    for (uint32_t i = 0; i < elements_.size(); ++i)
    {
        UIElement* element = elements_[elements_.size() - i - 1].get();
        if (!element || !element->IsActive()) continue;

        bool is_in_range = element->IsInRange(position);
        bool was_in_range = element->IsInRange(position - delta);

        if (is_in_range && !was_in_range) result |= element->OnMouseEnter();
        if (!is_in_range && was_in_range)
        {
            result |= element->OnMouseLeave();
            if (result) return result;
        }

        if (is_in_range || was_in_range)
        {
            result |= element->OnMouseMotion(position, delta);;
        }

        if (result) return result;
    }

    return result;
}

bool UIState::OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp)
{
    bool result = false;
    
    if (is_dragging_ && button == MouseButton::kLeft && !is_pressed)
    {
        if (dragging_element_)
        {
            UIElement* target_element = RayCast(position);
            if (target_element && target_element != dragging_element_)
            {
                result |= target_element->OnDrop(position, dragging_element_);
            }

            if (has_begun_drag_)
            {
                result |= dragging_element_->OnDragEnd(position);
                has_begun_drag_ = false;
            }
        }
        
        is_dragging_ = false;
        dragging_element_ = nullptr;
    }
    else if (!is_dragging_ && button == MouseButton::kLeft && is_pressed)
    {
        dragging_element_ = RayCast(position);
        is_dragging_ = dragging_element_ != nullptr;
        if (!is_dragging_) UpdateFocus(nullptr);
    }

    for (uint32_t i = 0; i < elements_.size(); ++i)
    {
        UIElement* element = elements_[elements_.size() - i - 1].get();
        if (element && element->IsActive() && element->IsInRange(position))
        {
            if (element->OnMouseButton(position, button, is_pressed, timestamp)) return true;
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

bool UIState::OnKey(uint32_t scancode, bool is_pressed)
{
    for (uint32_t i = 0; i < focus_path_.size(); ++i)
    {
        UIElement* element = focus_path_[focus_path_.size() - i - 1];
        if (element && element->IsFocused() && element->OnKey(scancode, is_pressed))
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

void UIState::ProcessPending()
{
    for (auto* element : pending_elements_)
    {
        if (!element || element->is_initialized_) continue;
        element->Init();
    }
    
    pending_elements_.clear();
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
