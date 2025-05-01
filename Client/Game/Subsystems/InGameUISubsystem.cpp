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
    chat_history_(nullptr),
    chat_input_(nullptr),
    mini_map_(nullptr)
{
}

void InGameUISubsystem::Init()
{
    GameInstanceSubsystem::Init();
    
    chat_history_ = UI::ListBox::Create(L"ChatHistory");
    chat_history_->SetPosition({ 0.f, 570.f });
    chat_history_->SetSize({ 300.f, 100.f });
    chat_history_->SetPivot({ 0.f, 0.f });

    chat_input_ = UI::EditableTextBox::Create(L"ChatInput");
    chat_input_->SetPosition({ 0.f, 600.f });
    chat_input_->SetSize({ 300.f, 30.f });
    chat_input_->SetPivot({ 0.f, 0.f });
    chat_input_->OnReturn(this, &InGameUISubsystem::OnChatInputReturn);

    mini_map_ = UI::MiniMap::Create(L"MiniMap");
    mini_map_->SetPosition({ 0.f, 0.f });
    mini_map_->SetPivot({ 0.f, 1.f });
    
}

void InGameUISubsystem::Deinit()
{
    GameInstanceSubsystem::Deinit();

    UI::Manager* ui_manager = UI::Manager::Get();

    ui_manager->RemoveFromViewport(chat_history_);
    ui_manager->RemoveFromViewport(chat_input_);
    ui_manager->RemoveFromViewport(mini_map_);
    
}

void InGameUISubsystem::ShowChatUI()
{
    UI::Manager* ui_manager = UI::Manager::Get();
    
    ui_manager->AddToViewport(chat_history_);
    ui_manager->AddToViewport(chat_input_);
}

void InGameUISubsystem::HideChatUI()
{
    UI::Manager* ui_manager = UI::Manager::Get();
    
    ui_manager->RemoveFromViewport(chat_history_);
    ui_manager->RemoveFromViewport(chat_input_);
}

void InGameUISubsystem::ShowMiniMap()
{
    UI::Manager* ui_manager = UI::Manager::Get();
    ui_manager->AddToViewport(mini_map_);
}

void InGameUISubsystem::HideMiniMap()
{
    UI::Manager* ui_manager = UI::Manager::Get();
    ui_manager->RemoveFromViewport(mini_map_);
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
