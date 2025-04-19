#include "pch.h"
#include "OneMap.h"

#include <CustomPacket.h>

#include "GameInstance.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/UIManager.h"
#include "UI/Widget/EditableTextBox.h"
#include "UI/Widget/ListBox.h"

OneMap::OneMap(const std::wstring& kName) :
    Level(kName),
    chat_list_(nullptr),
    chat_input_(nullptr)
{
}

void OneMap::Load()
{
    Level::Load();

    Logger::Print(L"OneMap::Load()");
    
    SessionSubsystem* subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (subsystem) subsystem->packet_handler.Add(this, &OneMap::ProcessPackets);

    chat_list_ = UI::ListBox::Create(L"ChatList");
    chat_list_->SetPosition({ 0.f, 570.f });
    chat_list_->SetSize({ 200.f, 200.f });
    chat_list_->SetPivot({0.f, 0.f});

    chat_input_ = UI::EditableTextBox::Create(L"ChatInput");
    chat_input_->SetPosition({ 0.f, 600.f });
    chat_input_->SetSize({ 200.f, 30.f });
    chat_input_->SetPlaceholder(L"Enter your message");
    chat_input_->SetPivot({0.f, 0.f});
    chat_input_->OnReturn(this, &OneMap::OnChatInput);

    UI::Manager::Get()->AddToViewport(chat_list_);
    UI::Manager::Get()->AddToViewport(chat_input_);
}

void OneMap::Unload(EndPlayReason type)
{
    Level::Unload(type);
    
    SessionSubsystem* subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (subsystem) subsystem->packet_handler.Remove(this, &OneMap::ProcessPackets);
}

void OneMap::ProcessPackets(const std::shared_ptr<Net::IPacket>& packet)
{
    switch (packet->GetPacketID())
    {
    case ChatMessageReceivePacket::StaticPacketID:
        {
            ChatMessageReceivePacket* chat_message_packet = static_cast<ChatMessageReceivePacket*>(packet.get());
            chat_list_->AddItem(chat_message_packet->message);
        }
        break;
        
    default:
        break;
    }
}

void OneMap::OnChatInput(const std::wstring& kText)
{
    SessionSubsystem* subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (!subsystem) return;
    
    chat_list_->AddItem(kText);

    ChatMessagePacket packet;
    packet.message = kText;
    subsystem->SendPacket(packet);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<OneMap>("OneMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
