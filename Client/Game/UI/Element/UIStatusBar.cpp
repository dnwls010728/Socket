#include "pch.h"
#include "UIStatusBar.h"

#include "Subsystems/DataSubsystem.h"
#include "Subsystems/PlayerSubsystem.h"
#include "Windows/DX/Renderer.h"

UIStatusBar::UIStatusBar(const std::wstring& name) :
    UIContainer(name),
    hp_(0),
    max_hp_(0),
    exp_(0),
    max_exp_(0)
{
    lv_text_ = AddChild<UIText>(UIText::StaticClass(), L"LvText");
    lv_text_->SetAbsolutePosition({583.f, 739.f});
    lv_text_->SetSize({200.f, 20.f});
    lv_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    lv_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    lv_text_->SetFontSize(16.f);
    lv_text_->SetColor(Math::Color::Orange);
    lv_text_->SetText(L"Lv. 1 Player Name");

    hp_text_ = AddChild<UIText>(UIText::StaticClass(), L"HPText");
    hp_text_->SetAbsolutePosition({583.f, 709.f});
    hp_text_->SetSize({200.f, 30.f});
    hp_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    hp_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    hp_text_->SetColor(Math::Color::White);
    hp_text_->SetText(L"0 / 0");

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

    PublisherSubsystem::Get()->Subscribe(PublisherSubsystem::EventType::kHPChanged, this, &UIStatusBar::OnEvent);
    PublisherSubsystem::Get()->Subscribe(PublisherSubsystem::EventType::kExpChanged, this, &UIStatusBar::OnEvent);
    PublisherSubsystem::Get()->Subscribe(PublisherSubsystem::EventType::kLvChanged, this, &UIStatusBar::OnEvent);

    PlayerSubsystem* player_subsystem = PlayerSubsystem::Get();

    lv_text_->SetText(L"Lv. " + std::to_wstring(player_subsystem->GetLv()) + L" " + player_subsystem->GetName());
    
    hp_ = player_subsystem->GetHP();
    max_hp_ = player_subsystem->GetMaxHP();

    hp_text_->SetText(std::to_wstring(hp_) + L" / " + std::to_wstring(max_hp_));

    exp_ = player_subsystem->GetExp();
    max_exp_ = DataSubsystem::Get()->GetExp(player_subsystem->GetLv());

    float exp_ratio = 100.f * static_cast<float>(exp_) / static_cast<float>(max_exp_);
    exp_text_->SetText(std::to_wstring(exp_) + L" [" + std::to_wstring(static_cast<int>(exp_ratio)) + L"%]");
}

void UIStatusBar::Uninit()
{
    UIContainer::Uninit();

    PublisherSubsystem::Get()->Unsubscribe(PublisherSubsystem::EventType::kHPChanged, this, &UIStatusBar::OnEvent);
    PublisherSubsystem::Get()->Unsubscribe(PublisherSubsystem::EventType::kExpChanged, this, &UIStatusBar::OnEvent);
    PublisherSubsystem::Get()->Unsubscribe(PublisherSubsystem::EventType::kLvChanged, this, &UIStatusBar::OnEvent);
}

void UIStatusBar::Render()
{
    Renderer* renderer = Renderer::Get();

    renderer->DrawRoundBox({ 583.f, 712.f }, { 200.f, 24.f }, Math::Color::Gray);

    float hp_ratio = static_cast<float>(hp_) / static_cast<float>(max_hp_);
    renderer->DrawSolidRoundBox({ 583.f, 712.f }, { 200.f * hp_ratio, 24.f }, Math::Color::Red);
    
    renderer->DrawSolidBox({ 0.f, 759.f }, { 1366.f, 9.f }, Math::Color::Gray);

    float exp_ratio = static_cast<float>(exp_) / static_cast<float>(max_exp_);
    renderer->DrawSolidBox({ 0.f, 759.f }, { 1366.f * exp_ratio, 9.f }, Math::Color::Green);
    
    UIContainer::Render();

    renderer->DrawStringWithOutline(
        L"Hello, World!", { 583.f, 300.f }, { 200.f, 30.f }, Math::Color::White, L"NanumBarunGothic", 16.f,
        DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER
    );
    
}

void UIStatusBar::OnEvent(const EventData& data)
{
    if (const auto* lv_data = dynamic_cast<const LvChangedEventData*>(&data))
    {
        max_exp_ = DataSubsystem::Get()->GetExp(lv_data->lv);
        lv_text_->SetText(L"Lv. " + std::to_wstring(lv_data->lv) + L" " + PlayerSubsystem::Get()->GetName());
    }
    else if (const auto* exp_data = dynamic_cast<const ExpChangedEventData*>(&data))
    {
        exp_ = exp_data->exp;;

        float exp_ratio = 100.f * static_cast<float>(exp_) / static_cast<float>(max_exp_);
        exp_text_->SetText(std::to_wstring(exp_) + L" [" + std::to_wstring(static_cast<int>(exp_ratio)) + L"%]");
    }
    else if (const auto* hp_data = dynamic_cast<const HPChangedEventData*>(&data))
    {
        hp_ = hp_data->hp;
        max_hp_ = hp_data->max_hp;

        hp_text_->SetText(std::to_wstring(hp_) + L" / " + std::to_wstring(max_hp_));
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
