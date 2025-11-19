#pragma once
#include "UI/Element/UIButton.h"
#include "CommonObject.h"

struct CardData;
class UIText;
class CardSelectInfo;

class UICardSlot : public UIButton
{
    GENERATED_BODY(UICardSlot, UIButton)
public:
    UICardSlot(const std::wstring& name);
    virtual ~UICardSlot() override = default;
    virtual void SetSize(const Math::Vector2& size) override;

    void SetCard(const CardSelectInfo& card);

    inline uint32_t GetCardID() const { return card_id_; }
protected:
    virtual void Init() override;
    virtual void UpdateLayout(const Math::Vector2& size);
private:
    UIText* desc_;
    UIText* name_;
    UIImage* icon_;
    
    uint32_t card_id_;
};
