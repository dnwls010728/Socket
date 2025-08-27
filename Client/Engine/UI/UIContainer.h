#pragma once
#include "UIElement.h"

class UIContainer : public UIElement
{
    GENERATED_BODY(UIContainer, UIElement)
    
public:
    virtual ~UIContainer() override = default;
    
    template <std::derived_from<UIElement> T>
    T* AddChild(const rttr::type& type, const std::wstring& name);

    virtual void RemoveChild(UIElement* child);

    virtual void SetActive(bool active) override;

    FORCEINLINE const std::vector<std::unique_ptr<UIElement>>& GetChildren() const { return children_; }

protected:
    UIContainer(const std::wstring& name);

    virtual void Init() override;
    virtual void Uninit() override;
    virtual void Tick(float delta_time) override;
    virtual void Render() override;

    virtual UIElement* RayCast(const Math::Vector2& position) override;
    
    virtual bool OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp) override;
    virtual bool OnScroll(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnKey(uint32_t scancode, bool is_pressed) override;
    virtual bool OnChar(wchar_t character) override;

    virtual void MakeDirty() override;

    virtual UIElement* AddChild_Internal(const rttr::type& type, const std::wstring& name);

    std::vector<std::unique_ptr<UIElement>> children_;
    
    std::queue<std::unique_ptr<UIElement>> pending_add_children_;
    std::queue<UIElement*> pending_remove_children_;

private:
    friend class UIState;

    void AddChildren();
    void RemoveChildren();
    
};

template <std::derived_from<UIElement> T>
T* UIContainer::AddChild(const rttr::type& type, const std::wstring& name)
{
    return dynamic_cast<T*>(AddChild_Internal(type, name));
}
