#include "pch.h"
#include "Slot.h"

#include <random>

#include "Resource/ResourceManager.h"
#include "UI/Canvas.h"
#include "UI/Widget/Image.h"
#include "UI/Widget/Text.h"
#include "Windows/DX/UITexture.h"

Slot::Slot(Widget* parent, const Math::Vector2& kPosition, int slot_index) :
    slot_index_(slot_index)
{
    UITexture* slot_texture = ResourceManager::Get()->Load<UITexture>(L"Sprites\\UI\\WindowPanel.png");

    Canvas* canvas = Canvas::Get();

    slot_image_ = canvas->AddWidget<Image>(L"SlotImage");
    slot_image_->AttachToWidget(parent);
    slot_image_->SetAnchoredPosition(kPosition);
    slot_image_->SetSize({64.f, 64.f});
    slot_image_->SetAnchorPreset(AnchorPreset::kLeft | AnchorPreset::kTop, true);
    slot_image_->SetRayCastTarget(true);
    slot_image_->SetTexture(slot_texture);
    slot_image_->SetDrawMode(DrawMode::kSliced);

    item_image_ = canvas->AddWidget<Image>(L"ItemImage");
    item_image_->AttachToWidget(slot_image_);
    item_image_->SetSize({0.f, 0.f});
    item_image_->SetAnchorPreset(AnchorPreset::kStretch);

    // 테스트
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, 100);
    int random = dis(gen);

    item_count_text_ = canvas->AddWidget<Text>(L"ItemCountText");
    item_count_text_->AttachToWidget(slot_image_);
    item_count_text_->SetAnchoredPosition({-16.f, -12.f});
    item_count_text_->SetAnchorPreset(AnchorPreset::kRight | AnchorPreset::kBottom, true);
    item_count_text_->SetAlignment(Text::kLowerRight);
    item_count_text_->SetColor(Math::Color::Black);
    item_count_text_->SetFontFamily(L"Nanum12");
    item_count_text_->SetText(std::to_wstring(random));
}
