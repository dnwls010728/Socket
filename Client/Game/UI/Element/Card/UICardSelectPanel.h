#pragma once
#include "UI/UIContainer.h"
#include <array>
#include <vector>

#include "UI/Element/CharacterSelect/UICharacterSelect.h"

class UIText;
class UIImage;
class UICardSlot;

class UICardSelectPanel : public UIContainer
{
    GENERATED_BODY(UICardSelectPanel, UIContainer)
public:
    UICardSelectPanel(const std::wstring& name);
    virtual ~UICardSelectPanel() override = default;

    void AddCards(const std::vector<CardSelectInfo>& cards);

protected:
    virtual void Init() override;

    virtual void ShowCards(const std::vector<CardSelectInfo>& showing_cards);
    virtual void OnSelectCard(const CardSelectInfo& card);

private:
    bool is_showing_cards_;
    UIText* caption_;
    UIImage* background_;
    UICardSlot* cards_[3];

    std::queue<std::vector<CardSelectInfo>> pending_card_queue_;
};
