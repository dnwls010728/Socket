#include "pch.h"
#include "InGameUISubsystem.h"

#include <CustomPacket.h>

#include "GameInstance.h"
#include "SessionSubsystem.h"
#include "UI/MiniMap.h"
#include "UI/UIManager.h"
#include "UI/Widget/EditableTextBox.h"
#include "UI/Widget/ListBox.h"

InGameUISubsystem::InGameUISubsystem() :
    chat_input_(nullptr),
    mini_map_(nullptr)
{
}

void InGameUISubsystem::Init()
{
    GameInstanceSubsystem::Init();

    chat_input_ = UI::EditableTextBox::Create(L"ChatInput");
    chat_input_->SetPosition({ 0.f, 600.f });
    chat_input_->SetSize({ 300.f, 20.f });
    chat_input_->SetPivot({ 0.f, 0.f });
    chat_input_->OnReturn(this, &InGameUISubsystem::OnChatInputReturn);

    mini_map_ = UI::MiniMap::Create(L"MiniMap");
    mini_map_->SetPosition({ 0.f, 0.f });
    mini_map_->SetPivot({ 0.f, 1.f });
    
}

void InGameUISubsystem::Deinit()
{
    GameInstanceSubsystem::Deinit();

    chat_input_->RemoveFromViewport();
    mini_map_->RemoveFromViewport();
    
}

void InGameUISubsystem::ShowChatUI()
{
    chat_input_->AddToViewport();
}

void InGameUISubsystem::HideChatUI()
{
    chat_input_->RemoveFromViewport();
}

void InGameUISubsystem::ShowMiniMap()
{
    mini_map_->AddToViewport();
}

void InGameUISubsystem::HideMiniMap()
{
    mini_map_->RemoveFromViewport();
}

void InGameUISubsystem::OnChatInputReturn(const std::wstring& text)
{
    if (text.empty())
    {
        UI::Manager::Get()->SetFocus(nullptr);
        return;
    }

    ChatMessagePacket chat_message_packet;
    chat_message_packet.message = text;
    GET_SESSION()->SendPacket(chat_message_packet);

    chat_input_->SetText(L"");
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<InGameUISubsystem>("InGameUISubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
