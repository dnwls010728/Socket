#include "pch.h"
#include "ZeroMap.h"

#include <CustomPacket.h>

#include "GameInstance.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/UIManager.h"
#include "UI/Widget/EditableTextBox.h"
#include "UI/Widget/ListBox.h"

ZeroMap::ZeroMap(const std::wstring& kName) :
    Level(kName),
    chat_list_(nullptr),
    chat_input_(nullptr)
{
}

void ZeroMap::Load()
{
    Level::Load();

    Logger::Print(L"ZeroMap::Load()");
    
    SessionSubsystem* subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (subsystem) subsystem->packet_handler.Add(this, &ZeroMap::ProcessPackets);

    chat_list_ = UI::ListBox::Create(L"ChatList");
    chat_list_->SetPosition({ 0.f, 570.f });
    chat_list_->SetSize({ 200.f, 200.f });
    chat_list_->SetPivot({0.f, 0.f});

    chat_input_ = UI::EditableTextBox::Create(L"ChatInput");
    chat_input_->SetPosition({ 0.f, 600.f });
    chat_input_->SetSize({ 200.f, 30.f });
    chat_input_->SetPlaceholder(L"Enter your message");
    chat_input_->SetPivot({0.f, 0.f});
    chat_input_->OnReturn(this, &ZeroMap::OnChatInput);

    UI::Manager::Get()->AddToViewport(chat_list_);
    UI::Manager::Get()->AddToViewport(chat_input_);
}

void ZeroMap::Unload(EndPlayReason type)
{
    Level::Unload(type);
    
    SessionSubsystem* subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (subsystem) subsystem->packet_handler.Remove(this, &ZeroMap::ProcessPackets);
}

void ZeroMap::ProcessPackets(const std::shared_ptr<Net::IPacket>& packet)
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

void ZeroMap::OnChatInput(const std::wstring& kText)
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

    registration::class_<ZeroMap>("ZeroMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
