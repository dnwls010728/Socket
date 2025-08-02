#include "pch.h"
#include "UICharacterCreate.h"

#include "Asset/AssetManager.h"
#include "UI/Element/ColorPicker/UIColorPicker.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/UISprite.h"

UICharacterCreate::UICharacterCreate(const std::wstring& name) :
    UIContainer(name)
{
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    UISprite* button_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\ButtonSheet.png");
    UISprite* character_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\UIPlayerSheet.png");

    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);

    title_text_ = AddChild<UIText>(UIText::StaticClass(), L"TitleText");
    title_text_->SetRelativePosition({10.f, 10.f});
    title_text_->SetSize({200.f, 20.f});
    title_text_->SetFontSize(18);
    title_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    title_text_->SetColor(Math::Color::White);
    title_text_->SetText(L"캐릭터 생성");

    character_ = AddChild<UIImage>(UIImage::StaticClass(), L"Character");
    character_->SetRelativePosition({10.f, 40.f});
    character_->SetSize({192.f, 168.f});
    character_->SetSprite(character_sprite, L"UIPlayerSheet_0");

    color_picker_ = AddChild<UIColorPicker>(UIColorPicker::StaticClass(), L"ColorPicker");
    color_picker_->SetRelativePosition({212.f, 40.f});
    color_picker_->OnValueChanged(this, &UICharacterCreate::OnColorChanged);
}

void UICharacterCreate::Init()
{
    background_->SetSize(GetSize());
    
    UIContainer::Init();
}

void UICharacterCreate::Render()
{
    Renderer* renderer = Renderer::Get();
    UIContainer::Render();

    renderer->DrawBox(character_->GetAbsolutePosition(), character_->GetSize(), Math::Color::White, 2.f);
}

void UICharacterCreate::OnColorChanged(Math::Color color)
{
    character_->SetColor(color);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UICharacterCreate>("UICharacterCreate")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
