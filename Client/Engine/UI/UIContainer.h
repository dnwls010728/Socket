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

    void GetChildren(std::vector<UIElement*>& children) const;

    FORCEINLINE uint64_t GetChildCount() const { return child_count_; }

protected:
    UIContainer(const std::wstring& name);
    
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
    std::vector<UIElement*> active_children_;
    
    std::queue<UIElement*> pending_add_children_;
    std::queue<UIElement*> pending_remove_children_;

private:
    friend class UIState;

    void AddChildren();
    void RemoveChildren();
    
    uint64_t child_count_;
    
};

template <std::derived_from<UIElement> T>
T* UIContainer::AddChild(const rttr::type& type, const std::wstring& name)
{
    return dynamic_cast<T*>(AddChild_Internal(type, name));
}
