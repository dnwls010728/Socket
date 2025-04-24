#include "pch.h"
#include "Map_Login.h"

#include <CustomPacket.h>

#include "GameInstance.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/UIManager.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/EditableTextBox.h"
#include "UI/Widget/ListBox.h"

Map_Login::Map_Login(const std::wstring& kName) :
    Level(kName),
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

void Map_Login::Load()
{
    Level::Load();

    GET_SESSION()->packet_handler.Add(this, &Map_Login::ProcessPackets);

    UI::Manager* ui_manager = UI::Manager::Get();
    if (ui_manager)
    {
        register_id_ = UI::EditableTextBox::Create(L"RegisterID");
        register_id_->SetPosition({ 400, 400 });
        register_id_->SetSize({ 200, 30 });
        register_id_->SetPlaceholder(L"아이디");

        register_password_ = UI::EditableTextBox::Create(L"RegisterPassword");
        register_password_->SetPosition({ 400, 440 });
        register_password_->SetSize({ 200, 30 });
        register_password_->SetPlaceholder(L"비밀번호");

        register_ = UI::Button::Create(L"Register");
        register_->SetPosition({ 400, 480 });
        register_->SetSize({ 200, 30 });
        register_->SetText(L"회원가입");
        register_->OnClick(this, &Map_Login::OnRegister);

        login_switch_ = UI::Button::Create(L"LoginSwitch");
        login_switch_->SetPosition({ 400, 520 });
        login_switch_->SetSize({ 200, 30 });
        login_switch_->SetText(L"로그인");
        login_switch_->OnClick(this, &Map_Login::OnLoginSwitch);
        
        login_id_ = UI::EditableTextBox::Create(L"LoginID");
        login_id_->SetPosition({ 400, 400 });
        login_id_->SetSize({ 200, 30 });
        login_id_->SetPlaceholder(L"아이디");

        login_password_ = UI::EditableTextBox::Create(L"LoginPassword");
        login_password_->SetPosition({ 400, 440 });
        login_password_->SetSize({ 200, 30 });
        login_password_->SetPlaceholder(L"비밀번호");

        login_ = UI::Button::Create(L"Login");
        login_->SetPosition({ 400, 480 });
        login_->SetSize({ 200, 30 });
        login_->SetText(L"로그인");
        login_->OnClick(this, &Map_Login::OnLogin);

        register_switch_ = UI::Button::Create(L"RegisterSwitch");
        register_switch_->SetPosition({ 400, 520 });
        register_switch_->SetSize({ 200, 30 });
        register_switch_->SetText(L"회원가입");
        register_switch_->OnClick(this, &Map_Login::OnRegisterSwitch);

        character_list_ = UI::ListBox::Create(L"CharacterList");
        character_list_->SetPosition({ 400, 400 });
        character_list_->SetSize({ 200, 300 });
        character_list_->OnDoubleClick(this, &Map_Login::OnCharacterSelect);

        ui_manager->AddToViewport(login_id_);
        ui_manager->AddToViewport(login_password_);
        ui_manager->AddToViewport(login_);
        ui_manager->AddToViewport(register_switch_);
    }
}

void Map_Login::Unload(EndPlayReason type)
{
    Level::Unload(type);
    
    UI::Manager* ui_manager = UI::Manager::Get();
    if (ui_manager)
    {
        if (ui_manager->IsInViewport(register_id_)) ui_manager->RemoveFromViewport(register_id_);
        if (ui_manager->IsInViewport(register_password_)) ui_manager->RemoveFromViewport(register_password_);
        if (ui_manager->IsInViewport(register_)) ui_manager->RemoveFromViewport(register_);
        if (ui_manager->IsInViewport(login_switch_)) ui_manager->RemoveFromViewport(login_switch_);
        
        if (ui_manager->IsInViewport(login_id_)) ui_manager->RemoveFromViewport(login_id_);
        if (ui_manager->IsInViewport(login_password_)) ui_manager->RemoveFromViewport(login_password_);
        if (ui_manager->IsInViewport(login_)) ui_manager->RemoveFromViewport(login_);
        if (ui_manager->IsInViewport(register_switch_)) ui_manager->RemoveFromViewport(register_switch_);

        if (ui_manager->IsInViewport(character_list_)) ui_manager->RemoveFromViewport(character_list_);
    }
    
    GET_SESSION()->packet_handler.Remove(this, &Map_Login::ProcessPackets);
}

void Map_Login::ProcessPackets(const std::shared_ptr<Net::IPacket>& packet)
{
    UI::Manager* ui_manager = UI::Manager::Get();
    
    switch (packet->GetPacketID())
    {
    case RegisterResponse::StaticPacketID:
        {
            RegisterResponse* response = static_cast<RegisterResponse*>(packet.get());
            Logger::Print(L"%s", response->message.c_str());

            if (response->is_success)
            {
                if (ui_manager)
                {
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
        }
        break;

    case LoginResponse::StaticPacketID:
        {
            LoginResponse* response = static_cast<LoginResponse*>(packet.get());
            Logger::Print(L"%s", response->message.c_str());

            if (response->is_success)
            {
                GET_SESSION()->SetState(SessionState::kLoggedIn);
                
                if (ui_manager)
                {
                    characters_ = response->characters;
                    for (const auto& character : characters_)
                    {
                        std::wstring name = character.name + L" (Lv. " + std::to_wstring(character.lv) + L")";
                        character_list_->AddItem(name, reinterpret_cast<uintptr_t>(&character));
                    }
                    
                    ui_manager->RemoveFromViewport(login_id_);
                    ui_manager->RemoveFromViewport(login_password_);
                    ui_manager->RemoveFromViewport(login_);
                    ui_manager->RemoveFromViewport(register_switch_);

                    ui_manager->AddToViewport(character_list_);
                }
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
                World::Get()->OpenLevel(std::to_wstring(character_info.map_unique_id));
            }
        }
        break;

    default:
        break;
    }
}

void Map_Login::OnRegister()
{
    if (register_id_->GetText().empty() || register_password_->GetText().empty()) return;

    RegisterRequest request;
    request.id = register_id_->GetText();
    request.password = register_password_->GetText();
    GET_SESSION()->SendPacket(request);
}

void Map_Login::OnLogin()
{
    if (login_id_->GetText().empty() || login_password_->GetText().empty()) return;

    LoginRequest request;
    request.id = login_id_->GetText();
    request.password = login_password_->GetText();
    GET_SESSION()->SendPacket(request);
}

void Map_Login::OnRegisterSwitch()
{
    UI::Manager* ui_manager = UI::Manager::Get();
    if (ui_manager)
    {
        ui_manager->RemoveFromViewport(login_id_);
        ui_manager->RemoveFromViewport(login_password_);
        ui_manager->RemoveFromViewport(login_);
        ui_manager->RemoveFromViewport(register_switch_);

        ui_manager->AddToViewport(register_id_);
        ui_manager->AddToViewport(register_password_);
        ui_manager->AddToViewport(register_);
        ui_manager->AddToViewport(login_switch_);
    }
}

void Map_Login::OnLoginSwitch()
{
    UI::Manager* ui_manager = UI::Manager::Get();
    if (ui_manager)
    {
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

void Map_Login::OnCharacterSelect(Type::uint64 user_data)
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

    registration::class_<Map_Login>("Map_Login")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
