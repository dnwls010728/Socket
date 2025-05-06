#include "pch.h"
#include "LoginMap.h"

#include <CustomPacket.h>

#include "GameInstance.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/UIManager.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/EditableTextBox.h"
#include "UI/Widget/Image.h"
#include "UI/Widget/ListBox.h"
#include "UI/Widget/TextBox.h"

LoginMap::LoginMap(const std::wstring& kName) :
    Level(kName),
    background_(nullptr),
    version_(nullptr),
    register_id_(nullptr),
    register_password_(nullptr),
    register_(nullptr),
    login_switch_(nullptr),
    login_id_(nullptr),
    login_password_(nullptr),
    login_(nullptr),
    register_switch_(nullptr),
    character_list_(nullptr),
    characters_()
{
}

void LoginMap::Load()
{
    Level::Load();

    GET_SESSION()->packet_handler.Add(this, &LoginMap::ProcessPackets);

    UI::Manager* ui_manager = UI::Manager::Get();

    background_ = UI::Image::Create(L"Background");
    background_->SetPosition({400.f, 300.f});
    background_->SetSize({800.f, 600.f});
    background_->SetTexture(L"\\UI\\LoginBackground.png");

    version_ = UI::TextBox::Create(L"Version");
    version_->SetPosition({10.f, 600.f});
    version_->SetSize({200.f, 30.f});
    version_->SetPivot({0.f, 0.f});
    version_->SetText(L"Ver: 0.0.1-development");

    register_id_ = UI::EditableTextBox::Create(L"RegisterID");
    register_id_->SetPosition({400.f, 400.f});
    register_id_->SetSize({200.f, 30.f});
    register_id_->SetPlaceholder(L"아이디");

    register_password_ = UI::EditableTextBox::Create(L"RegisterPassword");
    register_password_->SetPosition({400.f, 440.f});
    register_password_->SetSize({200.f, 30.f});
    register_password_->SetPlaceholder(L"비밀번호");

    register_ = UI::Button::Create(L"Register");
    register_->SetPosition({400.f, 480.f});
    register_->SetSize({200.f, 30.f});
    register_->SetText(L"회원가입");
    register_->OnClick(this, &LoginMap::OnRegister);

    login_switch_ = UI::Button::Create(L"LoginSwitch");
    login_switch_->SetPosition({400.f, 520.f});
    login_switch_->SetSize({200.f, 30.f});
    login_switch_->SetText(L"로그인");
    login_switch_->OnClick(this, &LoginMap::OnLoginSwitch);

    login_id_ = UI::EditableTextBox::Create(L"LoginID");
    login_id_->SetPosition({400.f, 400.f});
    login_id_->SetSize({200.f, 30.f});
    login_id_->SetPlaceholder(L"아이디");

    login_password_ = UI::EditableTextBox::Create(L"LoginPassword");
    login_password_->SetPosition({400.f, 440.f});
    login_password_->SetSize({200.f, 30.f});
    login_password_->SetPlaceholder(L"비밀번호");

    login_ = UI::Button::Create(L"Login");
    login_->SetPosition({400.f, 480.f});
    login_->SetSize({200.f, 30.f});
    login_->SetText(L"로그인");
    login_->OnClick(this, &LoginMap::OnLogin);

    register_switch_ = UI::Button::Create(L"RegisterSwitch");
    register_switch_->SetPosition({400.f, 520.f});
    register_switch_->SetSize({200.f, 30.f});
    register_switch_->SetText(L"회원가입");
    register_switch_->OnClick(this, &LoginMap::OnRegisterSwitch);

    character_list_ = UI::ListBox::Create(L"CharacterList");
    character_list_->SetPosition({400.f, 400.f});
    character_list_->SetSize({200.f, 300.f});
    character_list_->OnDoubleClick(this, &LoginMap::OnCharacterSelect);

    ui_manager->AddToViewport(background_);
    ui_manager->AddToViewport(version_);
    ui_manager->AddToViewport(login_id_);
    ui_manager->AddToViewport(login_password_);
    ui_manager->AddToViewport(login_);
    ui_manager->AddToViewport(register_switch_);
}

void LoginMap::Unload(EndPlayReason type)
{
    Level::Unload(type);

    UI::Manager* ui_manager = UI::Manager::Get();
    
    ui_manager->RemoveFromViewport(background_);
    
    ui_manager->RemoveFromViewport(version_);

    ui_manager->RemoveFromViewport(register_id_);
    ui_manager->RemoveFromViewport(register_password_);
    ui_manager->RemoveFromViewport(register_);
    ui_manager->RemoveFromViewport(login_switch_);

    ui_manager->RemoveFromViewport(login_id_);
    ui_manager->RemoveFromViewport(login_password_);
    ui_manager->RemoveFromViewport(login_);
    ui_manager->RemoveFromViewport(register_switch_);

    ui_manager->RemoveFromViewport(character_list_);

    GET_SESSION()->packet_handler.Remove(this, &LoginMap::ProcessPackets);
}

void LoginMap::ProcessPackets(const std::shared_ptr<Net::IPacket>& packet)
{
    switch (packet->GetPacketID())
    {
    case RegisterResponse::StaticPacketID:
        {
            RegisterResponse* response = static_cast<RegisterResponse*>(packet.get());
            Logger::Print(L"%s", response->message.c_str());

            if (response->is_success)
            {
                UI::Manager* ui_manager = UI::Manager::Get();

                ui_manager->RemoveFromViewport(register_id_);
                ui_manager->RemoveFromViewport(register_password_);
                ui_manager->RemoveFromViewport(register_);
                ui_manager->RemoveFromViewport(login_switch_);

                ui_manager->AddToViewport(login_id_);
                ui_manager->AddToViewport(login_password_);
                ui_manager->AddToViewport(login_);
                ui_manager->AddToViewport(register_switch_);
            }
        }
        break;

    case LoginResponse::StaticPacketID:
        {
            LoginResponse* response = static_cast<LoginResponse*>(packet.get());
            Logger::Print(L"%s", response->message.c_str());

            if (response->is_success)
            {
                GET_SESSION()->SetState(SessionState::kLoggedIn);

                characters_ = response->characters;
                for (const auto& character : characters_)
                {
                    std::wstring name = character.name + L" (Lv. " + std::to_wstring(character.lv) + L")";
                    character_list_->AddItem(name, reinterpret_cast<uintptr_t>(&character));
                }

                UI::Manager* ui_manager = UI::Manager::Get();

                ui_manager->RemoveFromViewport(login_id_);
                ui_manager->RemoveFromViewport(login_password_);
                ui_manager->RemoveFromViewport(login_);
                ui_manager->RemoveFromViewport(register_switch_);

                ui_manager->AddToViewport(character_list_);
            }
        }
        break;

    case SelectCharacterResponse::StaticPacketID:
        {
            SelectCharacterResponse* response = static_cast<SelectCharacterResponse*>(packet.get());
            if (response->is_success)
            {
                GET_SESSION()->SetState(SessionState::kInGame);

                const CharacterInfo& character_info = response->character_info;
                GET_SESSION()->SetCharacterInfo(character_info);
                World::Get()->OpenLevel(L"InGame");
            }
        }
        break;

    default:
        break;
    }
}

void LoginMap::OnRegister()
{
    if (register_id_->GetText().empty() || register_password_->GetText().empty()) return;

    RegisterRequest request;
    request.id = register_id_->GetText();
    request.password = register_password_->GetText();
    GET_SESSION()->SendPacket(request);
}

void LoginMap::OnLogin()
{
    if (login_id_->GetText().empty() || login_password_->GetText().empty()) return;

    LoginRequest request;
    request.id = login_id_->GetText();
    request.password = login_password_->GetText();
    GET_SESSION()->SendPacket(request);
}

void LoginMap::OnRegisterSwitch()
{
    UI::Manager* ui_manager = UI::Manager::Get();

    ui_manager->RemoveFromViewport(login_id_);
    ui_manager->RemoveFromViewport(login_password_);
    ui_manager->RemoveFromViewport(login_);
    ui_manager->RemoveFromViewport(register_switch_);

    ui_manager->AddToViewport(register_id_);
    ui_manager->AddToViewport(register_password_);
    ui_manager->AddToViewport(register_);
    ui_manager->AddToViewport(login_switch_);
}

void LoginMap::OnLoginSwitch()
{
    UI::Manager* ui_manager = UI::Manager::Get();

    ui_manager->RemoveFromViewport(register_id_);
    ui_manager->RemoveFromViewport(register_password_);
    ui_manager->RemoveFromViewport(register_);
    ui_manager->RemoveFromViewport(login_switch_);

    ui_manager->AddToViewport(login_id_);
    ui_manager->AddToViewport(login_password_);
    ui_manager->AddToViewport(login_);
    ui_manager->AddToViewport(register_switch_);
}

void LoginMap::OnCharacterSelect(Type::uint64 user_data)
{
    CharacterInfo* character = reinterpret_cast<CharacterInfo*>(user_data);
    if (!character) return;

    SelectCharacterRequest request;
    request.unique_id = character->unique_id;
    GET_SESSION()->SendPacket(request);
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
