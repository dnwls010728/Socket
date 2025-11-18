#pragma once
#include "UI/UIContainer.h"

#include "Misc/Function.h"

class UIImage;
class UIText;
class UIItemTooltip;

class UIShopItemRow : public UIContainer
{
    GENERATED_BODY(UIShopItemRow, UIContainer)

public:
    UIShopItemRow(const std::wstring& name);
    virtual ~UIShopItemRow() override = default;

    void SetItem(uint32_t item_id);
    void SetPriceText(const std::wstring& text) const;
    void SetCountText(const std::wstring& text) const;
    void SetDoubleClickHandler(const Function<void(void)>& handler);
    void Clear();

protected:
    virtual void Init() override;

    virtual bool OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp) override;
    virtual bool OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnMouseEnter() override;
    virtual bool OnMouseLeave() override;

private:
    static std::wstring FormatCurrency(int32_t value);
    
    UIImage* background_;
    UIImage* icon_;
    UIText* name_text_;
    UIText* price_text_;
    UIText* count_text_;

    UIItemTooltip* tooltip_;

    uint32_t item_id_;
    double last_click_time_;

    Function<void(void)> double_click_event_;
};
