#include "pch.h"
#include "UIStatusBar.h"

#include "Windows/DX/Renderer.h"

UIStatusBar::UIStatusBar(const std::wstring& name) :
    UIContainer(name),
    lv_text_(nullptr)
{
}

void UIStatusBar::Init()
{
    UIContainer::Init();

    PublisherSubsystem::Get()->Subscribe(PublisherSubsystem::EventType::kLvChanged, this, &UIStatusBar::OnEvent);

    lv_text_ = AddChild<UIText>(UIText::StaticClass(), L"LvText");
    lv_text_->SetAbsolutePosition({10.f, 729.f});
    lv_text_->SetSize({100.f, 30.f});
    lv_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    lv_text_->SetFontSize(18.f);
    lv_text_->SetColor(Math::Color::White);
    lv_text_->SetText(L"Lv. 1");
}

void UIStatusBar::Render()
{
    UIContainer::Render();

    Renderer* renderer = Renderer::Get();

    renderer->DrawSolidBox({ 0.f, 759.f }, { 1366.f, 9.f }, Math::Color::Gray);

    float exp_ratio = .5f;
    renderer->DrawSolidBox({ 0.f, 759.f }, { 1366.f * exp_ratio, 9.f }, Math::Color::Green);
    
}

void UIStatusBar::OnEvent(const EventData& data)
{
    if (const auto* lv_data = dynamic_cast<const LvChangedEventData*>(&data))
    {
        lv_text_->SetText(L"Lv. " + std::to_wstring(lv_data->lv));
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
