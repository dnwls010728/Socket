#include "pch.h"
#include "UIMenu.h"

#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/UISprite.h"

UIMenu::UIMenu(const std::wstring& name) :
    UIContainer(name)
{
    SetAbsolutePosition({583.f, 234.f});
    SetSize({200.f, 300.f});
    
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");

    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);

    title_text_ = AddChild<UIText>(UIText::StaticClass(), L"TitleText");
    title_text_->SetRelativePosition({ 4.f, 4.f });
    title_text_->SetSize({ 192.f, 20.f });
    title_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    title_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    title_text_->SetColor(Math::Color::White);
    title_text_->SetText(L"메뉴");
}

void UIMenu::Init()
{
    background_->SetSize(GetSize());
    
    UIContainer::Init();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIMenu>("UIMenu")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
