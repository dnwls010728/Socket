#include "pch.h"
#include "LoginMap.h"

#include "UI/UIManager.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/EditableTextBox.h"
#include "UI/Widget/TextBox.h"

LoginMap::LoginMap(const std::wstring& kName) 
    : Level(kName),
    login_id_(nullptr),
    login_password_(nullptr),
    login_(nullptr),
    register_(nullptr),
    register_id_(nullptr),
    register_password_(nullptr),
    register_password_confirm_(nullptr),
    register_message_(nullptr),
    register_ok_(nullptr),
    register_cancel_(nullptr)
{
}

void LoginMap::Load()
{
    Level::Load();

    UI::Manager* ui_manager = UI::Manager::Get();

#pragma region 로그인
    login_id_ = UI::EditableTextBox::Create(L"Login ID");
    login_id_->SetPosition({ 400.f, 400.f });
    login_id_->SetSize({ 200.f, 30.f });
    login_id_->SetPlaceholder(L"아이디");

    login_password_ = UI::EditableTextBox::Create(L"Password");
    login_password_->SetPosition({ 400.f, 430.f });
    login_password_->SetSize({ 200.f, 30.f });
    login_password_->SetPlaceholder(L"비밀번호");

    login_ = UI::Button::Create(L"Login");
    login_->SetPosition({ 540.f, 415.f });
    login_->SetSize({ 60.f, 60.f });
    login_->SetText(L"로그인");

    register_ = UI::Button::Create(L"Register");
    register_->SetPosition({ 400.f, 470.f });
    register_->SetSize({ 200.f, 30.f });
    register_->SetText(L"회원가입");
    register_->OnClick(this, &LoginMap::OnShowRegister);

    ui_manager->AddToViewport(login_id_);
    ui_manager->AddToViewport(login_password_);
    ui_manager->AddToViewport(login_);
    ui_manager->AddToViewport(register_);
#pragma endregion

#pragma region 회원가입
    register_id_ = UI::EditableTextBox::Create(L"Register ID");
    register_id_->SetPosition({ 400.f, 340.f });
    register_id_->SetSize({ 200.f, 30.f });
    register_id_->SetPlaceholder(L"아이디");

    register_password_ = UI::EditableTextBox::Create(L"Register Password");
    register_password_->SetPosition({ 400.f, 370.f });
    register_password_->SetSize({ 200.f, 30.f });
    register_password_->SetPlaceholder(L"비밀번호");

    register_password_confirm_ = UI::EditableTextBox::Create(L"Register Confirm");
    register_password_confirm_->SetPosition({ 400.f, 400.f });
    register_password_confirm_->SetSize({ 200.f, 30.f });
    register_password_confirm_->SetPlaceholder(L"비밀번호 확인");
    register_password_confirm_->OnValueChanged(this, &LoginMap::OnPasswordConfirm);

    register_message_ = UI::TextBox::Create(L"Register Message");
    register_message_->SetPosition({ 400.f, 430.f });
    register_message_->SetSize({ 300.f, 30.f });
    register_message_->SetText(L"");
    register_message_->SetColor(Math::Color::Orange);

    register_ok_ = UI::Button::Create(L"Register OK");
    register_ok_->SetPosition({ 400.f, 470.f });
    register_ok_->SetSize({ 200.f, 30.f });
    register_ok_->SetText(L"회원가입");

    register_cancel_ = UI::Button::Create(L"Register Cancel");
    register_cancel_->SetPosition({ 400.f, 500.f });
    register_cancel_->SetSize({ 200.f, 30.f });
    register_cancel_->SetText(L"뒤로");
    register_cancel_->OnClick(this, &LoginMap::OnShowLogin);
#pragma endregion
    
}

void LoginMap::OnShowRegister()
{
    UI::Manager* ui_manager = UI::Manager::Get();
    
#pragma region 로그인
    ui_manager->RemoveFromViewport(login_id_);
    ui_manager->RemoveFromViewport(login_password_);
    ui_manager->RemoveFromViewport(login_);
    ui_manager->RemoveFromViewport(register_);
#pragma endregion
    
#pragma region 회원가입
    ui_manager->AddToViewport(register_id_);
    ui_manager->AddToViewport(register_password_);
    ui_manager->AddToViewport(register_password_confirm_);
    ui_manager->AddToViewport(register_message_);
    ui_manager->AddToViewport(register_ok_);
    ui_manager->AddToViewport(register_cancel_);
#pragma endregion
}

void LoginMap::OnShowLogin()
{
    UI::Manager* ui_manager = UI::Manager::Get();
    
#pragma region 로그인
    ui_manager->AddToViewport(login_id_);
    ui_manager->AddToViewport(login_password_);
    ui_manager->AddToViewport(login_);
    ui_manager->AddToViewport(register_);
#pragma endregion
    
#pragma region 회원가입
    ui_manager->RemoveFromViewport(register_id_);
    ui_manager->RemoveFromViewport(register_password_);
    ui_manager->RemoveFromViewport(register_password_confirm_);
    ui_manager->RemoveFromViewport(register_message_);
    ui_manager->RemoveFromViewport(register_ok_);
    ui_manager->RemoveFromViewport(register_cancel_);
#pragma endregion
}

void LoginMap::OnPasswordConfirm(const std::wstring& kValue)
{
    if (register_password_->GetText() == register_password_confirm_->GetText())
    {
        register_message_->SetText(L"비밀번호가 일치합니다.");
    }
    else
    {
        register_message_->SetText(L"비밀번호가 일치하지 않습니다.");
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;
    
    registration::class_<LoginMap>("LoginMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
