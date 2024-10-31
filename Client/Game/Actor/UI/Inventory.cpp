#include "pch.h"
#include "Inventory.h"

#include "Resource/ResourceManager.h"
#include "UI/Canvas.h"
#include "UI/Widget/Image.h"
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

    window_panel_ = canvas->AddWidget<Image>(L"InventoryPanel");
    window_panel_->AttachToWidget(window_caption_);
    window_panel_->SetSize({200.f, 300.f});
    window_panel_->SetTexture(window_panel_texture);
    window_panel_->SetDrawMode(DrawMode::kSliced);
    window_panel_->SetPivot({.5f, 1.f});
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
