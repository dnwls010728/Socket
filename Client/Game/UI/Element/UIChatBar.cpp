#include "pch.h"
#include "UIChatBar.h"

#include "Asset/AssetManager.h"
#include "UI/Element/UIEditableText.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/UISprite.h"

UIChatBar::UIChatBar(const std::wstring& name) :
    UIContainer(name)
{
    size_ = { 300.f, 75.f };
    
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");

    input_background_ = AddChild<UIImage>(UIImage::StaticClass(), L"InputBackground");
    input_background_->SetSize({ 300.f, 50.f });
    input_background_->SetSprite(panel_sprite, L"Panel_0");
    input_background_->SetDrawMode(UIImage::DrawMode::kSliced);

    chat_background_ = AddChild<UIImage>(UIImage::StaticClass(), L"ChatBackground");
    chat_background_->SetRelativePosition({ 0.f, 50.f });
    chat_background_->SetSize({ 300.f, 25.f });
    chat_background_->SetSprite(panel_sprite, L"Panel_0");
    chat_background_->SetDrawMode(UIImage::DrawMode::kSliced);
    chat_background_->SetIgnoreRayCast(true);

    input_text_ = AddChild<UIEditableText>(UIEditableText::StaticClass(), L"InputText");
    input_text_->SetRelativePosition({ 4.f, 50.f });
    input_text_->SetSize({ 292.f, 25.f });
    input_text_->SetText(L"Type your message here...");
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIChatBar>("UIChatBar")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
