#pragma once
#include "UI.h"
#include "UIElement.h"
#include "Input/Mouse.h"
#include "Misc/EngineMacros.h"
#include "UIContainer.h"

class UIState
{
    GENERATED_BODY(UIState)

public:
    UIState();
    virtual ~UIState() = default;
    
    template <std::derived_from<UIElement> T>
    T* AddElement(const rttr::type& type, const std::wstring& name);

    template <std::derived_from<UIElement> T>
    T* FindElement(const std::wstring& name);

    void RemoveElement(UIElement* element);
    void SetFocus(UIElement* element);
    
    UIElement* RayCast(const Math::Vector2& position) const;

    bool IsFocused() const;
    bool IsEditingText() const;

    void PostTask(Function<void()> task);
protected:
    friend class UI;

    virtual void Init();
    virtual void Uninit();
    virtual void Tick(float delta_time);
    virtual void Render();

    virtual UI::MouseEventResult OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta);
    virtual UI::MouseEventResult OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp);

    virtual bool OnScroll(const Math::Vector2& position, const Math::Vector2& delta);
    virtual bool OnKey(uint32_t scancode, bool is_pressed);
    virtual bool OnChar(wchar_t character);

private:
    friend class UIElement;

    void ProcessPending();
    void UpdateFocus(UIElement* element);
    
    template <std::derived_from<UIElement> T>
    T* FindElement_Internal(UIElement* element, const std::wstring& name);
    
    std::vector<std::unique_ptr<UIElement>> elements_;
    std::vector<UIElement*> pending_elements_;
    std::vector<UIElement*> focus_path_;

    bool is_initialized_;
    bool is_dragging_;
    bool has_begun_drag_;

    UIElement* dragging_element_;

    std::queue<Function<void()>> pending_tasks_;
};

template <std::derived_from<UIElement> T>
T* UIState::AddElement(const rttr::type& type, const std::wstring& name)
{
    if (!type.is_derived_from<UIElement>()) return nullptr;

    rttr::variant var = type.create({ name });
    if (var.is_valid())
    {
        UIElement* element = var.get_value<UIElement*>();
        elements_.emplace_back(std::unique_ptr<UIElement>(element));
        pending_elements_.push_back(element);

        return dynamic_cast<T*>(element);
    }
    
    return nullptr;
}

template <std::derived_from<UIElement> T>
T* UIState::FindElement(const std::wstring& name)
{
    for (uint32_t i = 0; i < elements_.size(); ++i)
    {
        UIElement* element = elements_[elements_.size() - i - 1].get();
        
        T* found_element = FindElement_Internal<T>(element, name);
        if (found_element) return found_element;
    }

    return nullptr;
}

template <std::derived_from<UIElement> T>
T* UIState::FindElement_Internal(UIElement* element, const std::wstring& name)
{
    if (element->GetName() == name)
        return dynamic_cast<T*>(element);

    UIContainer* container = dynamic_cast<UIContainer*>(element);
    if (container)
    {
        const std::vector<std::unique_ptr<UIElement>>& children = container->GetChildren();
        for (uint32_t i = 0; i < children.size(); ++i)
        {
            UIElement* child = children[children.size() - i - 1].get();
            if (child)
            {
                T* found_element = FindElement_Internal<T>(child, name);
                if (found_element) return found_element;
            }
        }
    }
    
    return nullptr;
}
