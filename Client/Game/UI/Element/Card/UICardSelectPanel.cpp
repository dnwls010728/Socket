#include "pch.h"
#include "UICardSelectPanel.h"

#include <CustomPacket.h>

#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIText.h"
#include "UI/Element/Card/UICardSlot.h"
#include "Subsystems/DataSubsystem.h"
#include "Subsystems/SessionSubsystem.h"
#include "EngineSettings.h"
#include "UI/UIState.h"
#include "Windows\DX\UISprite.h"

UICardSelectPanel::UICardSelectPanel(const std::wstring& name) :
    UIContainer(name),
    is_showing_cards_(false),
    caption_(nullptr),
    background_(nullptr),
    cards_{}
{
    Math::Vector2 panel_size{800.f, 500.f};
    SetSize(panel_size);
    
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);
    background_->SetSize(panel_size);

    caption_ = AddChild<UIText>(UIText::StaticClass(), L"Caption");
    caption_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    caption_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    caption_->SetColor(Math::Color::White);
    caption_->SetIgnoreRayCast(true);
    caption_->SetFontSize(18.f);
    caption_->SetSize({panel_size.x, 80.f});
    caption_->SetRelativePosition({0.f, 0.f});
    caption_->SetText(L"카드를 선택하세요.");

    constexpr float start_x = 40.f;
    constexpr float start_y = 80.f;
    constexpr float spacing = 30.f;
    float card_width = (panel_size.x - start_x * 2.f - spacing * 2.f) / 3.f;
    float card_height = panel_size.y - start_y * 2.f;
    for (int i = 0; i < 3; ++i)
    {
        cards_[i] = AddChild<UICardSlot>(UICardSlot::StaticClass(), L"Card" + std::to_wstring(i));
        cards_[i]->SetSize({card_width, card_height});
        cards_[i]->SetRelativePosition({start_x + i * (card_width + spacing), start_y});
    }

    
}

void UICardSelectPanel::Init()
{
    UIContainer::Init();
}

void UICardSelectPanel::ShowCards(const std::vector<CardSelectInfo>& showing_cards)
{
    is_showing_cards_ = true;

    for (size_t i = 0; i < 3 && i < showing_cards.size(); ++i)
    {
        const CardData* data = DataSubsystem::Get()->GetCard(showing_cards[i].card_id);
        if (!data) continue;

        cards_[i]->SetCard(showing_cards[i]);
        cards_[i]->OnClick([this, card = showing_cards[i]]()
        {
            OnSelectCard(card);
        });
    }
}

void UICardSelectPanel::OnSelectCard(const CardSelectInfo& card)
{
    SelectCardResult result;
    result.card_id = card.card_id;
    SessionSubsystem::Get()->SendPacket(result);

    if (pending_card_queue_.empty())
    {
        UIState* state = UI::Get()->GetState();
        if (state) state->RemoveElement(this);
    }
    else
    {
        auto next_cards = pending_card_queue_.front();
        pending_card_queue_.pop();
        ShowCards(next_cards);
    }
}

void UICardSelectPanel::AddCards(const std::vector<CardSelectInfo>& cards)
{
    if (cards.empty()) return;
    if (cards.size() < 3) return;
    if (is_showing_cards_)
    {
        pending_card_queue_.push(cards);
    }
    else
    {
        ShowCards(cards);
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<UICardSelectPanel>("UICardSelectPanel")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
