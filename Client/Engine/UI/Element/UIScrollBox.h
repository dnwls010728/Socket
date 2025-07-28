#pragma once
#include "UIMask.h"

class UIScrollBox : public UIMask
{
    GENERATED_BODY(UIScrollBox, UIMask)
    
public:
    enum class VerticalAlignment : uint8_t
    {
        kTop = 0,
        kBottom
    };
    
    UIScrollBox(const std::wstring& name);
    virtual ~UIScrollBox() override = default;

    virtual void Init() override;
    
    template <std::derived_from<UIElement> T>
    T* AddItem(const rttr::type& type, const std::wstring& name);

    void RemoveItem(UIElement* item);

protected:
    virtual void Tick(float delta_time) override;
    virtual void Render() override;

private:
    void UpdateLayout() const;
    
    UIContainer* content_;

    VerticalAlignment vertical_alignment_;

    float spacing_;

    bool dirty_;
    
};

template <std::derived_from<UIElement> T>
T* UIScrollBox::AddItem(const rttr::type& type, const std::wstring& name)
{
    T* element = content_->AddChild<T>(type, name);
    dirty_ = true;
    return element;
}
