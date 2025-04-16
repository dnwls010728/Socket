#include "pch.h"
#include "LoginMap.h"

#include <CustomPacket.h>

#include "GameInstance.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/UIManager.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/EditableTextBox.h"

LoginMap::LoginMap(const std::wstring& kName) :
    Level(kName),
    login_id_(nullptr),
    login_password_(nullptr),
    login_(nullptr)
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

        ui_manager->AddToViewport(login_id_);
        ui_manager->AddToViewport(login_password_);
        ui_manager->AddToViewport(login_);
    }
}

void LoginMap::Unload(EndPlayReason type)
{
    Level::Unload(type);
    
    UI::Manager* ui_manager = UI::Manager::Get();
    if (ui_manager)
    {
        if (ui_manager->IsInViewport(login_id_)) ui_manager->RemoveFromViewport(login_id_);
        if (ui_manager->IsInViewport(login_password_)) ui_manager->RemoveFromViewport(login_password_);
        if (ui_manager->IsInViewport(login_)) ui_manager->RemoveFromViewport(login_);
    }
    
    SessionSubsystem* subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (subsystem) subsystem->packet_handler.Remove(this, &LoginMap::ProcessPackets);
}

void LoginMap::ProcessPackets(std::shared_ptr<Net::IPacket> packet)
{
    switch (packet->GetPacketID())
    {
    case LoginPacketAck::StaticPacketID:
        {
            LoginPacketAck* login_response = static_cast<LoginPacketAck*>(packet.get());
            if (login_response->result) Logger::Print(L"%s", login_response->message.c_str());
            else Logger::Print(L"로그인 실패: %s", login_response->message.c_str());
        }
        break;

    default: break;
    }
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

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<LoginMap>("LoginMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
