#include "pch.h"
#include "UIMenu.h"

#include "Asset/AssetManager.h"
#include "Audio/AudioManager.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UISlider.h"
#include "Windows/DX/UISprite.h"

UIMenu::UIMenu(const std::wstring& name) :
    UIContainer(name)
{
    SetAbsolutePosition({463.f, 84.f});
    SetSize({440.f, 600.f});
    
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");

    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);

    title_text_ = AddChild<UIText>(UIText::StaticClass(), L"TitleText");
    title_text_->SetRelativePosition({ 4.f, 4.f });
    title_text_->SetSize({ 432.f, 20.f });
    title_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    title_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    title_text_->SetColor(Math::Color::White);
    title_text_->SetText(L"게임 설정");

    master_text_ = AddChild<UIText>(UIText::StaticClass(), L"MasterText");
    master_text_->SetRelativePosition({ 4.f, 30.f });
    master_text_->SetSize({ 100.f, 20.f });
    master_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    master_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    master_text_->SetColor(Math::Color::White);
    master_text_->SetText(L"전체 사운드");

    bgm_text_ = AddChild<UIText>(UIText::StaticClass(), L"BGMText");
    bgm_text_->SetRelativePosition({ 4.f, 60.f });
    bgm_text_->SetSize({ 100.f, 20.f });
    bgm_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    bgm_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    bgm_text_->SetColor(Math::Color::White);
    bgm_text_->SetText(L"배경음악");

    se_text_ = AddChild<UIText>(UIText::StaticClass(), L"SEText");
    se_text_->SetRelativePosition({ 4.f, 90.f });
    se_text_->SetSize({ 100.f, 20.f });
    se_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    se_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    se_text_->SetColor(Math::Color::White);
    se_text_->SetText(L"일반 효과음");

    mob_se_text_ = AddChild<UIText>(UIText::StaticClass(), L"MobSEText");
    mob_se_text_->SetRelativePosition({ 4.f, 120.f });
    mob_se_text_->SetSize({ 100.f, 20.f });
    mob_se_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    mob_se_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    mob_se_text_->SetColor(Math::Color::White);
    mob_se_text_->SetText(L"몬스터 효과음");

    skill_se_text_ = AddChild<UIText>(UIText::StaticClass(), L"SkillSEText");
    skill_se_text_->SetRelativePosition({ 4.f, 150.f });
    skill_se_text_->SetSize({ 100.f, 20.f });
    skill_se_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    skill_se_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    skill_se_text_->SetColor(Math::Color::White);
    skill_se_text_->SetText(L"스킬 효과음");

    master_slider_ = AddChild<UISlider>(UISlider::StaticClass(), L"MasterSlider");
    master_slider_->SetRelativePosition({ 110.f, 32.5f });
    master_slider_->SetSize({ 256.f, 15.f });
    master_slider_->SetRange(0.f, 100.f);
    master_slider_->SetStep(1.f);
    master_slider_->SetValue(100.f);
    master_slider_->OnValueChanged(this, &UIMenu::OnMaster);

    bgm_slider_ = AddChild<UISlider>(UISlider::StaticClass(), L"BGMSlider");
    bgm_slider_->SetRelativePosition({ 110.f, 62.5f });
    bgm_slider_->SetSize({ 256.f, 15.f });
    bgm_slider_->SetRange(0.f, 100.f);
    bgm_slider_->SetStep(1.f);
    bgm_slider_->SetValue(100.f);
    bgm_slider_->OnValueChanged(this, &UIMenu::OnBGM);

    se_slider_ = AddChild<UISlider>(UISlider::StaticClass(), L"SESlider");
    se_slider_->SetRelativePosition({ 110.f, 92.5f });
    se_slider_->SetSize({ 256.f, 15.f });
    se_slider_->SetRange(0.f, 100.f);
    se_slider_->SetStep(1.f);
    se_slider_->SetValue(100.f);
    se_slider_->OnValueChanged(this, &UIMenu::OnSE);

    mob_se_slider_ = AddChild<UISlider>(UISlider::StaticClass(), L"MobSESlider");
    mob_se_slider_->SetRelativePosition({ 110.f, 122.5f });
    mob_se_slider_->SetSize({ 256.f, 15.f });
    mob_se_slider_->SetRange(0.f, 100.f);
    mob_se_slider_->SetStep(1.f);
    mob_se_slider_->SetValue(100.f);
    mob_se_slider_->OnValueChanged(this, &UIMenu::OnMobSE);

    skill_se_slider_ = AddChild<UISlider>(UISlider::StaticClass(), L"SkillSESlider");
    skill_se_slider_->SetRelativePosition({ 110.f, 152.5f });
    skill_se_slider_->SetSize({ 256.f, 15.f });
    skill_se_slider_->SetRange(0.f, 100.f);
    skill_se_slider_->SetStep(1.f);
    skill_se_slider_->SetValue(100.f);
    skill_se_slider_->OnValueChanged(this, &UIMenu::OnSkillSE);

    master_percent_text_ = AddChild<UIText>(UIText::StaticClass(), L"MasterPercentText");
    master_percent_text_->SetRelativePosition({ 376.f, 30.f });
    master_percent_text_->SetSize({ 60.f, 20.f });
    master_percent_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    master_percent_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    master_percent_text_->SetColor(Math::Color::White);
    master_percent_text_->SetText(L"100%");

    bgm_percent_text_ = AddChild<UIText>(UIText::StaticClass(), L"BGMPercentText");
    bgm_percent_text_->SetRelativePosition({ 376.f, 60.f });
    bgm_percent_text_->SetSize({ 60.f, 20.f });
    bgm_percent_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    bgm_percent_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    bgm_percent_text_->SetColor(Math::Color::White);
    bgm_percent_text_->SetText(L"100%");

    se_percent_text_ = AddChild<UIText>(UIText::StaticClass(), L"SEPercentText");
    se_percent_text_->SetRelativePosition({ 376.f, 90.f });
    se_percent_text_->SetSize({ 60.f, 20.f });
    se_percent_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    se_percent_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    se_percent_text_->SetColor(Math::Color::White);
    se_percent_text_->SetText(L"100%");

    mob_se_percent_text_ = AddChild<UIText>(UIText::StaticClass(), L"MobSEPercentText");
    mob_se_percent_text_->SetRelativePosition({ 376.f, 120.f });
    mob_se_percent_text_->SetSize({ 60.f, 20.f });
    mob_se_percent_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    mob_se_percent_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    mob_se_percent_text_->SetColor(Math::Color::White);
    mob_se_percent_text_->SetText(L"100%");

    skill_se_percent_text_ = AddChild<UIText>(UIText::StaticClass(), L"SkillSEPercentText");
    skill_se_percent_text_->SetRelativePosition({ 376.f, 150.f });
    skill_se_percent_text_->SetSize({ 60.f, 20.f });
    skill_se_percent_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    skill_se_percent_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    skill_se_percent_text_->SetColor(Math::Color::White);
    skill_se_percent_text_->SetText(L"100%");
}

void UIMenu::Init()
{
    background_->SetSize(GetSize());
    
    UIContainer::Init();
}

void UIMenu::Render()
{
    Renderer* renderer = Renderer::Get();
    
    Math::Vector2 absolute_position = GetAbsolutePosition();
    {
        Math::Vector2 position = absolute_position + Math::Vector2(110.f, 35.f);
        renderer->DrawSolidRoundBox(position, {256.f, 10.f}, Math::Color::Gray, 4.f);
    }

    {
        Math::Vector2 position = absolute_position + Math::Vector2(110.f, 65.f);
        renderer->DrawSolidRoundBox(position, {256.f, 10.f}, Math::Color::Gray, 4.f);
    }

    {
        Math::Vector2 position = absolute_position + Math::Vector2(110.f, 95.f);
        renderer->DrawSolidRoundBox(position, {256.f, 10.f}, Math::Color::Gray, 4.f);
    }

    {
        Math::Vector2 position = absolute_position + Math::Vector2(110.f, 125.f);
        renderer->DrawSolidRoundBox(position, {256.f, 10.f}, Math::Color::Gray, 4.f);
    }

    {
        Math::Vector2 position = absolute_position + Math::Vector2(110.f, 155.f);
        renderer->DrawSolidRoundBox(position, {256.f, 10.f}, Math::Color::Gray, 4.f);
    }
    
    UIContainer::Render();
}

void UIMenu::OnMaster(float value)
{
    int32_t volume = static_cast<int32_t>(value);
    master_percent_text_->SetText(std::to_wstring(volume) + L"%");
    AudioManager::Get()->SetMasterVolume(volume);
}

void UIMenu::OnBGM(float value)
{
    int32_t volume = static_cast<int32_t>(value);
    bgm_percent_text_->SetText(std::to_wstring(volume) + L"%");
    AudioManager::Get()->SetVolume(ChannelGroup::kBGM, volume);
}

void UIMenu::OnSE(float value)
{
    int32_t volume = static_cast<int32_t>(value);
    se_percent_text_->SetText(std::to_wstring(volume) + L"%");
    AudioManager::Get()->SetVolume(ChannelGroup::kSE, volume);
}

void UIMenu::OnMobSE(float value)
{
    int32_t volume = static_cast<int32_t>(value);
    mob_se_percent_text_->SetText(std::to_wstring(volume) + L"%");
    AudioManager::Get()->SetVolume(ChannelGroup::kMobSE, volume);
}

void UIMenu::OnSkillSE(float value)
{
    int32_t volume = static_cast<int32_t>(value);
    skill_se_percent_text_->SetText(std::to_wstring(volume) + L"%");
    AudioManager::Get()->SetVolume(ChannelGroup::kSkillSE, volume);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIMenu>("UIMenu")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
