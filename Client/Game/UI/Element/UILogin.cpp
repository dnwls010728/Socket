#include "pch.h"
#include "UILogin.h"

#include <CustomPacket.h>

#include "Asset/AssetManager.h"
#include <shellapi.h>
#include "Subsystems/SessionSubsystem.h"
#include "UI/UIPopup.h"
#include "UI/Element/UIEditableText.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/UISprite.h"

UILogin::UILogin(const std::wstring& name) :
    UIContainer(name)
{
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    UISprite* button_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\ButtonSheet.png");

    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);

    id_background_ = AddChild<UIImage>(UIImage::StaticClass(), L"IDBackground");
    id_background_->SetRelativePosition({30.f, 32.f});
    id_background_->SetSize({278.f, 44.f});
    id_background_->SetSprite(panel_sprite, L"Panel_0");
    id_background_->SetDrawMode(UIImage::DrawMode::kSliced);
    id_background_->SetIgnoreRayCast(true);

    password_background_ = AddChild<UIImage>(UIImage::StaticClass(), L"PasswordBackground");
    password_background_->SetRelativePosition({30.f, 78.f});
    password_background_->SetSize({278.f, 44.f});
    password_background_->SetSprite(panel_sprite, L"Panel_0");
    password_background_->SetDrawMode(UIImage::DrawMode::kSliced);
    password_background_->SetIgnoreRayCast(true);

    input_id_ = AddChild<UIEditableText>(UIEditableText::StaticClass(), L"InputID");
    input_id_->SetRelativePosition({40.f, 42.f});
    input_id_->SetSize({258.f, 24.f});
    input_id_->SetPlaceholderText(L"아이디");

    input_password_ = AddChild<UIEditableText>(UIEditableText::StaticClass(), L"InputPassword");
    input_password_->SetRelativePosition({40.f, 88.f});
    input_password_->SetSize({258.f, 24.f});
    input_password_->SetPlaceholderText(L"비밀번호");
    input_password_->SetContentType(UIEditableText::ContentType::kPassword);

    login_button_ = AddChild<UIButton>(UIButton::StaticClass(), L"LoginButton");
    login_button_->SetRelativePosition({30.f, 134.f});
    login_button_->SetSize({278.f, 53.f});
    login_button_->SetSprite(UIButton::State::kNormal, button_sprite, L"ButtonSheet_0");
    login_button_->SetSprite(UIButton::State::kHover, button_sprite, L"ButtonSheet_1");
    login_button_->SetSprite(UIButton::State::kPressed, button_sprite, L"ButtonSheet_2");
    login_button_->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    login_button_->SetDrawMode(UIImage::DrawMode::kSliced);
    login_button_->SetTextColor(Math::Color::White);
    login_button_->SetText(L"로그인");
    login_button_->OnClick(this, &UILogin::OnLogin);

    register_button_ = AddChild<UIButton>(UIButton::StaticClass(), L"RegisterButton");
    register_button_->SetRelativePosition({30.f, 194.f});
    register_button_->SetSize({80.f, 20.f});
    register_button_->SetTextColor(Math::Color::White);
    register_button_->SetText(L"회원가입");
    register_button_->OnClick(this, &UILogin::OnRegister);
}

void UILogin::SetLoginDisabled(bool is_disabled) const
{
    login_button_->SetDisabled(is_disabled);
}

void UILogin::Init()
{
    background_->SetSize(GetSize());
    
    UIContainer::Init();
}

bool UILogin::OnKey(uint16_t key_code, bool is_pressed)
{
    if (is_pressed)
    {
        switch (key_code)
        {
        case VK_TAB:
            {
                if (input_id_->IsFocused()) UI::Get()->SetFocus(input_password_);
                else if (input_password_->IsFocused()) UI::Get()->SetFocus(input_id_);
                return true;
            }

        case VK_RETURN:
            {
                OnLogin();
                return true;
            }
        }
    }
    
    return false;
}

void UILogin::OnLogin()
{
    if (input_id_->GetText().empty())
    {
        UIPopup::ShowPopup(L"아이디를 입력해 주세요.", PopupOption::OK, [&](std::wstring input_text, PopupOption option)
        {
            return true;
        });
        return;
    }

    if (input_password_->GetText().empty())
    {
        UIPopup::ShowPopup(L"비밀번호를 입력해 주세요.", PopupOption::OK, [&](std::wstring input_text, PopupOption option)
        {
            return true;
        });
        return;
    }

    LoginRequest request;
    // request.id = input_id_->GetText();
    // request.password = input_password_->GetText();
    request.id = L"master001";
    request.password = L"12345";
    SessionSubsystem::Get()->SendPacket(request);
    
    login_button_->SetDisabled(true);
}

void UILogin::OnRegister()
{
    ShellExecute(nullptr, nullptr, L"http://58.79.118.105:9100", nullptr, nullptr, SW_SHOW);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UILogin>("UILogin")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
