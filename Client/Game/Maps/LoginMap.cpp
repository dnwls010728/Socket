#include "pch.h"
#include "LoginMap.h"

#include <CustomPacket.h>

#include "GameInstance.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/UIManager.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/EditableTextBox.h"
#include "UI/Widget/ListBox.h"

LoginMap::LoginMap(const std::wstring& kName) :
    Level(kName),
    register_id_(nullptr),
    register_password_(nullptr),
    register_(nullptr),
    login_switch_(nullptr),
    login_id_(nullptr),
    login_password_(nullptr),
    login_(nullptr),
    register_switch_(nullptr),
    character_list_(nullptr)
{
}

void LoginMap::Load()
{
    Level::Load();

    SessionSubsystem* subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (subsystem) subsystem->packet_handler.Add(this, &LoginMap::ProcessPackets);

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
        register_->OnClick(this, &LoginMap::OnRegister);

        login_switch_ = UI::Button::Create(L"LoginSwitch");
        login_switch_->SetPosition({ 400, 520 });
        login_switch_->SetSize({ 200, 30 });
        login_switch_->SetText(L"로그인");
        login_switch_->OnClick(this, &LoginMap::OnLoginSwitch);
        
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
        login_->OnClick(this, &LoginMap::OnLogin);

        register_switch_ = UI::Button::Create(L"RegisterSwitch");
        register_switch_->SetPosition({ 400, 520 });
        register_switch_->SetSize({ 200, 30 });
        register_switch_->SetText(L"회원가입");
        register_switch_->OnClick(this, &LoginMap::OnRegisterSwitch);

        character_list_ = UI::ListBox::Create(L"CharacterList");
        character_list_->SetPosition({ 400, 400 });
        character_list_->SetSize({ 200, 300 });

        ui_manager->AddToViewport(login_id_);
        ui_manager->AddToViewport(login_password_);
        ui_manager->AddToViewport(login_);
        ui_manager->AddToViewport(register_switch_);
    }
}

void LoginMap::Unload(EndPlayReason type)
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
    }
    
    SessionSubsystem* subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (subsystem) subsystem->packet_handler.Remove(this, &LoginMap::ProcessPackets);
}

void LoginMap::ProcessPackets(std::shared_ptr<Net::IPacket> packet)
{
    UI::Manager* ui_manager = UI::Manager::Get();

    SessionSubsystem* subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    
    switch (packet->GetPacketID())
    {
    case RegisterPacketAck::StaticPacketID:
        {
            RegisterPacketAck* register_response = static_cast<RegisterPacketAck*>(packet.get());
            if (!register_response->result) Logger::Print(L"%s", register_response->message.c_str());
            else
            {
                Logger::Print(L"%s", register_response->message.c_str());

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
        
    case LoginPacketAck::StaticPacketID:
        {
            LoginPacketAck* login_response = static_cast<LoginPacketAck*>(packet.get());
            if (login_response->result)
            {
                if (subsystem)
                {
                    subsystem->SetAccountUniqueID(login_response->account_unique_id);
                    subsystem->SetLoggedIn(true);
                }

                if (ui_manager)
                {
                    const std::vector<CharacterInfo>& characters = login_response->characters;
                    for (const auto& character : characters)
                    {
                        std::wstring name = character.character_name + L" (Lv." + std::to_wstring(character.character_lv) + L")";
                        character_list_->AddItem(name, character.character_unique_id);
                    }
                    
                    ui_manager->RemoveFromViewport(login_id_);
                    ui_manager->RemoveFromViewport(login_password_);
                    ui_manager->RemoveFromViewport(login_);
                    ui_manager->RemoveFromViewport(register_switch_);

                    ui_manager->AddToViewport(character_list_);
                }
            }
            
            Logger::Print(L"%s", login_response->message.c_str());
        }
        break;

    default: break;
    }
}

void LoginMap::OnRegister()
{
    if (register_id_->GetText().empty() || register_password_->GetText().empty()) return;
    
    SessionSubsystem* subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (!subsystem) return;

    RegisterPacketReq register_packet_reqest;
    register_packet_reqest.id = register_id_->GetText();
    register_packet_reqest.password = register_password_->GetText();
    subsystem->SendPacket(register_packet_reqest);
}

void LoginMap::OnLogin()
{
    if (login_id_->GetText().empty() || login_password_->GetText().empty()) return;

    SessionSubsystem* subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (!subsystem) return;

    LoginPacketReq login_request;
    login_request.id = login_id_->GetText();
    login_request.password = login_password_->GetText();
    subsystem->SendPacket(login_request);
}

void LoginMap::OnRegisterSwitch()
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

void LoginMap::OnLoginSwitch()
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

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<LoginMap>("LoginMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
