#include "pch.h"
#include "InGameUISubsystem.h"

#include "UI/UIManager.h"
#include "UI/Widget/EditableTextBox.h"
#include "UI/Widget/ListBox.h"

InGameUISubsystem::InGameUISubsystem() :
    chat_history_(nullptr),
    chat_input_(nullptr)
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
    
}

void InGameUISubsystem::Deinit()
{
    GameInstanceSubsystem::Deinit();

    UI::Manager* ui_manager = UI::Manager::Get();

    if (ui_manager->IsInViewport(chat_history_)) ui_manager->RemoveFromViewport(chat_history_);
    if (ui_manager->IsInViewport(chat_input_)) ui_manager->RemoveFromViewport(chat_input_);
    
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
