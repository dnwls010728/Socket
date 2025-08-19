#include "pch.h"
#include "UILoginState.h"

#include "Asset/AssetManager.h"
#include "Element/CharacterSelect/UICharacterSelect.h"
#include "Element/UILogin.h"
#include "UI/Element/Slider.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIText.h"
#include "Windows/WindowsApplication.h"
#include "Windows/DX/UISprite.h"

UILoginState::UILoginState()
{
    UISprite* sprite = AssetManager::Get()->Load<UISprite>(L"UI\\LoginBackground.png");
    UISprite* button_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\ButtonSheet.png");

    UIImage* image = AddElement<UIImage>(UIImage::StaticClass(), L"LoginBackground");
    image->SetAbsolutePosition({-170.f, -256.f});
    image->SetSize({1536.f, 1024.f});
    image->SetSprite(sprite, L"LoginBackground_0");
    image->SetIgnoreRayCast(true);

    AddElement<UILogin>(UILogin::StaticClass(), L"Login");

    UIText* version_text = AddElement<UIText>(UIText::StaticClass(), L"VersionText");
    version_text->SetAbsolutePosition({10.f, 748.f});
    version_text->SetSize({100.f, 20.f});
    version_text->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    version_text->SetText(L"Ver. 0.0.1");

    UIButton* exit_button = AddElement<UIButton>(UIButton::StaticClass(), L"ExitButton");
    exit_button->SetAbsolutePosition({1256.f, 718.f});
    exit_button->SetSize({100.f, 40.f});;
    exit_button->SetSprite(UIButton::State::kNormal, button_sprite, L"ButtonSheet_0");
    exit_button->SetSprite(UIButton::State::kHover, button_sprite, L"ButtonSheet_1");
    exit_button->SetSprite(UIButton::State::kPressed, button_sprite, L"ButtonSheet_2");
    exit_button->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    exit_button->SetDrawMode(UIImage::DrawMode::kSliced);
    exit_button->SetTextColor(Math::Color::White);
    exit_button->SetText(L"끝내기");
    exit_button->OnClick(this, &UILoginState::OnExit);

    Slider* volume_slider = AddElement<Slider>(Slider::StaticClass(), L"VolumeSlider");
    volume_slider->SetAbsolutePosition({10.f, 10.f});
    volume_slider->SetSize({200.f, 20.f});
}

void UILoginState::Init()
{
    UIState::Init();

}

void UILoginState::OnExit()
{
    WindowsApplication::Get()->QuitApplication();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UILoginState>("UILoginState")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
