#include "pch.h"
#include "UIStatusBar.h"

#include "imgui/imgui.h"
#include "Math/Math.h"
#include "Subsystems/DataSubsystem.h"
#include "Subsystems/PlayerSubsystem.h"
#include "Windows/DX/Renderer.h"

UIStatusBar::UIStatusBar(const std::wstring& name) :
    UIContainer(name),
    hp_(0),
    max_hp_(0),
    exp_(0),
    max_exp_(0),
    timer_(0.f),
    hp_effect_ratio_(0.f)
{
    lv_text_ = AddChild<UIText>(UIText::StaticClass(), L"LvText");
    lv_text_->SetAbsolutePosition({583.f, 739.f});
    lv_text_->SetSize({200.f, 20.f});
    lv_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    lv_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    lv_text_->SetFontSize(16.f);
    lv_text_->SetColor(Math::Color::White);
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

    PublisherSubsystem::Get()->Subscribe(PublisherSubsystem::EventType::kStatUpdated, this, &UIStatusBar::OnEvent);

    PlayerSubsystem* player_subsystem = PlayerSubsystem::Get();

    lv_text_->SetText(L"Lv. " + std::to_wstring(player_subsystem->GetLv()) + L" " + player_subsystem->GetName());
    
    hp_ = player_subsystem->GetHP();
    max_hp_ = player_subsystem->GetMaxHP();

    hp_text_->SetText(std::to_wstring(hp_) + L" / " + std::to_wstring(max_hp_));

    exp_ = player_subsystem->GetExp();
    max_exp_ = DataSubsystem::Get()->GetExp(player_subsystem->GetLv());

    float exp_ratio = 100.f * static_cast<float>(exp_) / static_cast<float>(max_exp_);
    if (!Math::IsValid(exp_ratio)) exp_ratio = 0.f;
    
    exp_text_->SetText(std::to_wstring(exp_) + L" [" + std::to_wstring(static_cast<int>(exp_ratio)) + L"%]");
}

void UIStatusBar::Uninit()
{
    UIContainer::Uninit();

    PublisherSubsystem::Get()->Unsubscribe(PublisherSubsystem::EventType::kStatUpdated, this, &UIStatusBar::OnEvent);
}

void UIStatusBar::Tick(float delta_time)
{
    UIContainer::Tick(delta_time);

    float hp_ratio = static_cast<float>(hp_) / static_cast<float>(max_hp_);
    
    if (timer_ < 1.f)
    {
        float t = timer_ / 1.f;
        t = t * t * (3.f - 2.f * t);
            
        hp_effect_ratio_ = Math::Lerp(hp_effect_ratio_, hp_ratio, t);
        timer_ += delta_time;
    }
    else hp_effect_ratio_ = hp_ratio;
}

void UIStatusBar::Render()
{
    Renderer* renderer = Renderer::Get();

    renderer->DrawSolidRoundBox({ 583.f, 712.f }, { 200.f, 24.f }, Math::Color::Gray);

    renderer->DrawSolidRoundBox({ 583.f, 712.f }, { 200.f * hp_effect_ratio_, 24.f }, Math::Color::White);

    float hp_ratio = static_cast<float>(hp_) / static_cast<float>(max_hp_);
    renderer->DrawSolidRoundBox({ 583.f, 712.f }, { 200.f * hp_ratio, 24.f }, Math::Color::Red);
    
    renderer->DrawSolidBox({ 0.f, 759.f }, { 1366.f, 9.f }, Math::Color::Gray);

    float exp_ratio = static_cast<float>(exp_) / static_cast<float>(max_exp_);
    renderer->DrawSolidBox({ 0.f, 759.f }, { 1366.f * exp_ratio, 9.f }, Math::Color::Green);
    
    UIContainer::Render();
    
}

void UIStatusBar::OnEvent(const EventData& data)
{
    if (const auto* stat_update = dynamic_cast<const StatUpdateData*>(&data))
    {
        switch (stat_update->stat)
        {
        case PlayerStat::kLv:
            {
                lv_text_->SetText(L"Lv. " + std::to_wstring(stat_update->value) + L" " + PlayerSubsystem::Get()->GetName());
                max_exp_ = DataSubsystem::Get()->GetExp(stat_update->value);
            }
            break;
        case PlayerStat::kHP:
            {
                hp_ = stat_update->value;
                hp_text_->SetText(std::to_wstring(hp_) + L" / " + std::to_wstring(max_hp_));
                timer_ = 0.f;
            }
            break;
        case PlayerStat::kMaxHP:
            {
                max_hp_ = stat_update->value;
                hp_text_->SetText(std::to_wstring(hp_) + L" / " + std::to_wstring(max_hp_));
                timer_ = 0.f;
            }
            break;
        case PlayerStat::kExp:
            {
                exp_ = stat_update->value;
                float exp_ratio = 100.f * static_cast<float>(exp_) / static_cast<float>(max_exp_);
                if (!Math::IsValid(exp_ratio)) exp_ratio = 0.f;
                exp_text_->SetText(std::to_wstring(exp_) + L" [" + std::to_wstring(static_cast<int>(exp_ratio)) + L"%]");
            }
            break;
        default:
            break;
        }
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
