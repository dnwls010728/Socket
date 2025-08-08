#pragma once
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class UINameTag : public UIContainer
{
    GENERATED_BODY(UINameTag, UIContainer)
    
public:
    UINameTag(const std::wstring& name);
    virtual ~UINameTag() override = default;

    void SetText(const std::wstring& text);
    void SetColor(const Math::Color& color) const;

protected:
    virtual void Init() override;
    virtual void Render() override;

private:
    UIText* text_;
    
};
