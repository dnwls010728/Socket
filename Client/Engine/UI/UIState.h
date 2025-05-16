#pragma once
#include "UI.h"
#include "UIElement.h"
#include "Input/Mouse.h"
#include "Misc/EngineMacros.h"

class UIState
{
    GENERATED_BODY(UIState)

public:
    UIState();
    virtual ~UIState() = default;

protected:
    friend class UI;

    FORCEINLINE virtual void Init()
    {
    }

    virtual void Tick(float delta_time);
    virtual void Render();

    virtual UI::MouseEventResult OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta);
    virtual UI::MouseEventResult OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp);

    virtual bool OnScroll(const Math::Vector2& position, const Math::Vector2& delta);
    virtual bool OnKey(uint16_t key_code, bool is_pressed);
    virtual bool OnChar(wchar_t character);

    template <std::derived_from<UIElement> T>
    T* AddElement(const rttr::type& type, const Math::Vector2& position = Math::Vector2::Zero(), const Math::Vector2& size = Math::Vector2::Zero());

private:
    std::vector<std::unique_ptr<UIElement>> elements_;
};

template <std::derived_from<UIElement> T>
T* UIState::AddElement(const rttr::type& type, const Math::Vector2& position, const Math::Vector2& size)
{
    if (!type.is_derived_from<UIElement>()) return nullptr;

    rttr::variant var = type.create({ position, size });
    if (var.is_valid())
    {
        UIElement* element = var.get_value<UIElement*>();
        elements_.emplace_back(std::unique_ptr<UIElement>(element));

        rttr::type element_type = rttr::type::get<T>();
        if (type.is_derived_from(element_type))
        {
            return static_cast<T*>(element);
        }
    }
    
    return nullptr;
}
