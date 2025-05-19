#pragma once
#include "UIElement.h"

class UIContainer : public UIElement
{
    GENERATED_BODY(UIContainer, UIElement)
    
public:
    virtual ~UIContainer() override = default;
    
    template <std::derived_from<UIElement> T>
    T* AddChild(const rttr::type& type);

    virtual void SetActive(bool active) override;

protected:
    UIContainer();

    virtual void Tick(float delta_time) override;
    virtual void Render() override;

    virtual UIElement* FindElement(const Math::Vector2& position) override;
    
    virtual UI::MouseEventResult OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual UI::MouseEventResult OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp) override;
    
    virtual bool OnScroll(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnKey(uint16_t key_code, bool is_pressed) override;
    virtual bool OnChar(wchar_t character) override;

private:
    std::vector<std::unique_ptr<UIElement>> children_;
    
};

template <std::derived_from<UIElement> T>
T* UIContainer::AddChild(const rttr::type& type)
{
    if (!type.is_derived_from(T::StaticClass())) return nullptr;

    rttr::variant var = type.create();
    if (var.is_valid())
    {
        UIElement* child = var.get_value<UIElement*>();
        child->parent_ = this;
        children_.push_back(std::unique_ptr<UIElement>(child));

        rttr::type child_type = rttr::type::get<T>();
        if (type.is_derived_from(child_type))
        {
            child->Init();
            return static_cast<T*>(child);
        }
    }
    
    return nullptr;
}
