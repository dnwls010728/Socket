#include "pch.h"
#include "UIChatBar.h"

#include <CustomPacket.h>

#include "Asset/AssetManager.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/Element/UIEditableText.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIScrollBox.h"
#include "Windows/DX/UISprite.h"

UIChatBar::UIChatBar(const std::wstring& name) :
    UIContainer(name)
{
    SetSize({ 300.f, 75.f });
    
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");

    chat_background_ = AddChild<UIImage>(UIImage::StaticClass(), L"ChatBackground");
    chat_background_->SetSize({ 300.f, 50.f });
    chat_background_->SetSprite(panel_sprite, L"Panel_0");
    chat_background_->SetDrawMode(UIImage::DrawMode::kSliced);

    target_background_ = AddChild<UIImage>(UIImage::StaticClass(), L"TargetBackground");
    target_background_->SetRelativePosition({ 0.f, 50.f });
    target_background_->SetSize({ 80.f, 25.f });
    target_background_->SetSprite(panel_sprite, L"Panel_0");
    target_background_->SetDrawMode(UIImage::DrawMode::kSliced);
    
    input_background_ = AddChild<UIImage>(UIImage::StaticClass(), L"InputBackground");
    input_background_->SetRelativePosition({ 80.f, 50.f });
    input_background_->SetSize({ 220.f, 25.f });
    input_background_->SetSprite(panel_sprite, L"Panel_0");
    input_background_->SetDrawMode(UIImage::DrawMode::kSliced);
    input_background_->SetIgnoreRayCast(true);

    scroll_box_ = AddChild<UIScrollBox>(UIScrollBox::StaticClass(), L"ScrollBox");
    scroll_box_->SetRelativePosition({ 4.f, 4.f });
    scroll_box_->SetSize({ 292.f, 42.f });
    scroll_box_->SetVerticalAlignment(UIScrollBox::VerticalAlignment::kBottom);
    scroll_box_->SetScrollStep(20.f);

    target_text_ = AddChild<UIText>(UIText::StaticClass(), L"TargetText");
    target_text_->SetRelativePosition({ 4.f, 50.f });
    target_text_->SetSize({ 72.f, 25.f });
    target_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    target_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    target_text_->SetColor(Math::Color::White);
    target_text_->SetText(L"모두에게");

    input_text_ = AddChild<UIEditableText>(UIEditableText::StaticClass(), L"InputText");
    input_text_->SetRelativePosition({ 84.f, 50.f });
    input_text_->SetSize({ 212.f, 25.f });
    input_text_->SetCharacterLimit(128);
    input_text_->OnReturn(this, &UIChatBar::OnReturn);
}

void UIChatBar::FocusInput()
{
    UI::Get()->SetFocus(input_text_);
}

void UIChatBar::AddMessage(const std::wstring& message) const
{
    UIText* text = scroll_box_->AddItem<UIText>(UIText::StaticClass(), L"Text");
    text->SetSize({ 292.f, 20.f });
    text->SetColor(Math::Color::White);
    text->SetText(message);

    // 텍스트 크기에 맞춰서 높이를 조정
    text->SetSize({ text->GetSize().x, text->GetTotalLineHeight() });
}

void UIChatBar::OnReturn()
{
    if (input_text_->GetText().empty())
    {
        UI::Get()->SetFocus(nullptr);
        return;
    }

    ChatMessagePacket chat_message_packet;
    chat_message_packet.message = input_text_->GetText();
    SessionSubsystem::Get()->SendPacket(chat_message_packet);

    input_text_->SetText(L"");
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIChatBar>("UIChatBar")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
