#include "pch.h"
#include "Inventory.h"

#include "Resource/ResourceManager.h"
#include "UI/Canvas.h"
#include "UI/Widget/Image.h"
#include "UI/Widget/Text.h"
#include "Windows/DX/UITexture.h"

Inventory::Inventory(const std::wstring& kName) :
    Actor(kName)
{
    Canvas* canvas = Canvas::Get();

    UITexture* window_caption_texture = ResourceManager::Get()->Load<UITexture>(L"Sprites\\UI\\WindowCaption.png");
    window_caption_texture->SetSlice9Rect({ 4.f, 4.f, 24.f, 17.f });
    
    UITexture* window_panel_texture = ResourceManager::Get()->Load<UITexture>(L"Sprites\\UI\\WindowPanel.png");
    window_panel_texture->SetSlice9Rect({ 18.f, 18.f, 28.f, 28.f });

    window_caption_ = canvas->AddWidget<Image>(L"InventoryCaption");
    window_caption_->AttachToWidget(canvas->GetRootWidget());
    window_caption_->SetSize({200.f, 32.f});
    window_caption_->SetRayCastTarget(true);
    window_caption_->SetTexture(window_caption_texture);
    window_caption_->SetDrawMode(DrawMode::kSliced);
    window_caption_->OnDrag.Add([=](const Math::Vector2& kDelta)
    {
        Math::Vector2 position = window_caption_->GetAnchoredPosition();
        window_caption_->SetAnchoredPosition(position + kDelta);
    });

    Text* caption_text = canvas->AddWidget<Text>(L"InventoryCaptionText");
    caption_text->AttachToWidget(window_caption_);
    caption_text->SetAnchorPreset(AnchorPreset::kStretch);
    caption_text->SetSize({0.f, 7.f});
    caption_text->SetFontFamily(L"Nanum12");
    caption_text->SetText(L"ITEM INVENTORY");
    caption_text->SetColor(Math::Color::Black);
    caption_text->SetAlignment(Text::kMiddleCenter);

    Image* window_panel = canvas->AddWidget<Image>(L"InventoryPanel");
    window_panel->AttachToWidget(window_caption_);
    window_panel->SetAnchoredPosition({0.f, -7.f});
    window_panel->SetSize({228.f, 300.f});
    window_panel->SetTexture(window_panel_texture);
    window_panel->SetDrawMode(DrawMode::kSliced);
    window_panel->SetPivot({.5f, 1.f});
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Inventory>("Inventory")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
