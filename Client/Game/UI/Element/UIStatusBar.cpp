#include "pch.h"
#include "UIStatusBar.h"

#include "Subsystems/DataSubsystem.h"
#include "Windows/DX/Renderer.h"

UIStatusBar::UIStatusBar(const std::wstring& name) :
    UIContainer(name),
    lv_text_(nullptr),
    exp_(0),
    max_exp_(0)
{
    lv_text_ = AddChild<UIText>(UIText::StaticClass(), L"LvText");
    lv_text_->SetAbsolutePosition({10.f, 729.f});
    lv_text_->SetSize({100.f, 30.f});
    lv_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    lv_text_->SetFontSize(18.f);
    lv_text_->SetColor(Math::Color::White);
    lv_text_->SetText(L"Lv. 1");

    exp_text_ = AddChild<UIText>(UIText::StaticClass(), L"ExpText");
    exp_text_->SetAbsolutePosition({583.f, 759.f});
    exp_text_->SetSize({200.f, 9.f});
    exp_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    exp_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    exp_text_->SetColor(Math::Color::White);
    exp_text_->SetText(L"0 [0%]");
}

void UIStatusBar::Init()
{
    UIContainer::Init();

    PublisherSubsystem::Get()->Subscribe(PublisherSubsystem::EventType::kExpChanged, this, &UIStatusBar::OnEvent);
    PublisherSubsystem::Get()->Subscribe(PublisherSubsystem::EventType::kLvChanged, this, &UIStatusBar::OnEvent);
}

void UIStatusBar::Uninit()
{
    UIContainer::Uninit();

    PublisherSubsystem::Get()->Unsubscribe(PublisherSubsystem::EventType::kExpChanged, this, &UIStatusBar::OnEvent);
    PublisherSubsystem::Get()->Unsubscribe(PublisherSubsystem::EventType::kLvChanged, this, &UIStatusBar::OnEvent);
}

void UIStatusBar::Render()
{
    Renderer* renderer = Renderer::Get();

    renderer->DrawSolidBox({ 0.f, 759.f }, { 1366.f, 9.f }, Math::Color::Gray);

    float exp_ratio = static_cast<float>(exp_) / static_cast<float>(max_exp_);
    renderer->DrawSolidBox({ 0.f, 759.f }, { 1366.f * exp_ratio, 9.f }, Math::Color::Green);
    
    UIContainer::Render();
    
}

void UIStatusBar::OnEvent(const EventData& data)
{
    if (const auto* lv_data = dynamic_cast<const LvChangedEventData*>(&data))
    {
        max_exp_ = DataSubsystem::Get()->GetExp(lv_data->lv);
        lv_text_->SetText(L"Lv. " + std::to_wstring(lv_data->lv));
    }
    else if (const auto* exp_data = dynamic_cast<const ExpChangedEventData*>(&data))
    {
        exp_ = exp_data->exp;;

        float exp_ratio = 100.f * static_cast<float>(exp_) / static_cast<float>(max_exp_);
        exp_text_->SetText(std::to_wstring(exp_) + L" [" + std::to_wstring(static_cast<int>(exp_ratio)) + L"%]");
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIStatusBar>("UIStatusBar")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
