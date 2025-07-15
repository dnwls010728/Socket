#pragma once
#include "UIElement.h"

class UIContainer : public UIElement
{
    GENERATED_BODY(UIContainer, UIElement)
    
public:
    virtual ~UIContainer() override = default;
    
    template <std::derived_from<UIElement> T>
    T* AddChild(const rttr::type& type, const std::wstring& name);

    virtual void SetActive(bool active) override;

    FORCEINLINE const std::vector<std::unique_ptr<UIElement>>& GetChildren() const { return children_; }

protected:
    UIContainer(const std::wstring& name);

    virtual void Init() override;
    virtual void Uninit() override;
    virtual void Tick(float delta_time) override;
    virtual void Render() override;

    virtual UIElement* RayCast(const Math::Vector2& position) override;
    
    virtual UI::MouseEventResult OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual UI::MouseEventResult OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp) override;

    virtual bool OnScroll(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnKey(uint16_t key_code, bool is_pressed) override;
    virtual bool OnChar(wchar_t character) override;

    std::vector<std::unique_ptr<UIElement>> children_;
    
};

template <std::derived_from<UIElement> T>
T* UIContainer::AddChild(const rttr::type& type, const std::wstring& name)
{
    if (!type.is_derived_from(T::StaticClass())) return nullptr;

    rttr::variant var = type.create({ name });
    if (var.is_valid())
    {
        UIElement* child = var.get_value<UIElement*>();
        child->parent_ = this;
        if (has_initialized_) child->Init();
        
        children_.push_back(std::unique_ptr<UIElement>(child));

        return dynamic_cast<T*>(child);
    }
    
    return nullptr;
}
