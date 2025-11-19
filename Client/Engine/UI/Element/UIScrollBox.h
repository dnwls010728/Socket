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
    
    template <std::derived_from<UIElement> T>
    T* AddItem(const rttr::type& type, const std::wstring& name);

    void RemoveItem(UIElement* item);
    void SetVerticalAlignment(VerticalAlignment alignment);
    void SetSpacing(float spacing);

    FORCEINLINE void SetScrollStep(float step) { scroll_step_ = step; }

    void UpdateLayout();
protected:
    virtual void Tick(float delta_time) override;
    virtual void Render() override;

    virtual bool OnScroll(const Math::Vector2& position, const Math::Vector2& delta) override;

private:
    UIContainer* content_;

    VerticalAlignment vertical_alignment_;

    float min_offset_;
    float max_offset_;
    float scroll_offset_;
    float scroll_step_;
    float spacing_;

    bool is_dirty_;
    
};

template <std::derived_from<UIElement> T>
T* UIScrollBox::AddItem(const rttr::type& type, const std::wstring& name)
{
    T* element = content_->AddChild<T>(type, name);
    is_dirty_ = true;
    return element;
}
