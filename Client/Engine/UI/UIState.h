#pragma once
#include "UIElement.h"
#include "Input/Mouse.h"
#include "Misc/EngineMacros.h"
#include "UIContainer.h"

struct UIElementActivation
{
    UIElement* element;
    bool is_active;
};

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

    template <std::derived_from<UIElement> T>
    T* FindElementShallow(const std::wstring& name);

    void RemoveElement(UIElement* element);
    void SetFocus(UIElement* element);
    
    void ClearFocus(UIElement* element);
    void ClearDrag(UIElement* element);
    
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

    virtual bool OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta);
    virtual bool OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp);
    virtual bool OnScroll(const Math::Vector2& position, const Math::Vector2& delta);
    virtual bool OnKey(uint32_t scancode, bool is_pressed);
    virtual bool OnChar(wchar_t character);

private:
    friend class UI;
    friend class UIElement;

    void ActivateElement(UIElement* element, bool is_active);
    void AddElements();
    void RemoveElements();
    void ActivateElements();
    void UpdateFocus(UIElement* element);
    
    template <std::derived_from<UIElement> T>
    T* FindElement_Internal(UIElement* element, const std::wstring& name);
    
    std::vector<std::unique_ptr<UIElement>> elements_;
    std::vector<UIElement*> active_elements_;
    std::vector<UIElement*> focus_path_;

    bool is_initialized_;
    bool is_dragging_;
    bool has_begun_drag_;

    uint64_t element_count_;

    UIElement* dragging_element_;

    std::queue<UIElement*> pending_add_elements_;
    std::queue<UIElement*> pending_remove_elements_;
    std::queue<UIElementActivation> pending_activations_;
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
        elements_.push_back(std::unique_ptr<UIElement>(element));
        pending_add_elements_.push(element);

        return dynamic_cast<T*>(element);
    }
    
    return nullptr;
}

template <std::derived_from<UIElement> T>
T* UIState::FindElementShallow(const std::wstring& name)
{
    for (uint32_t i = 0; i < elements_.size(); ++i)
    {
        UIElement* element = elements_[elements_.size() - i - 1].get();
        
        if (element->GetName() == name)
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
        std::vector<UIElement*> children;
        container->GetChildren(children);
        
        for (uint32_t i = 0; i < children.size(); ++i)
        {
            UIElement* child = children[children.size() - i - 1];
            if (child)
            {
                T* found_element = FindElement_Internal<T>(child, name);
                if (found_element) return found_element;
            }
        }
    }
    
    return nullptr;
}
