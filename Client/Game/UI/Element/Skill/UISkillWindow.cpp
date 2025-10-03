#include "pch.h"
#include "UISkillWindow.h"

#include "UISkillTooltip.h"
#include "Asset/AssetManager.h"
#include "Subsystems/PlayerSubsystem.h"
#include "UI/Element/UIButton.h"
#include "UI/Element/Skill/UISkillSlot.h"
#include "UI/Element/UIScrollBox.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIText.h"
#include "Windows/DX/UISprite.h"

UISkillWindow::UISkillWindow(const std::wstring& name) :
    UIContainer(name),
    scroll_box_(nullptr),
    slots_(),
    dragging_icon_(nullptr)
{
    SetSize({ 212.f, 300.f });

    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    UISprite* button_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\ButtonSheet.png");

    UIImage* background = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background->SetSprite(panel_sprite, L"Panel_0");
    background->SetDrawMode(UIImage::DrawMode::kSliced);
    background->SetSize(GetSize());
    background->SetIgnoreRayCast(true);

    UIText* title = AddChild<UIText>(UIText::StaticClass(), L"Title");
    title->SetRelativePosition({ 8.f, 8.f });
    title->SetSize({ 160.f, 24.f });
    title->SetColor(Math::Color::White);
    title->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    title->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    title->SetText(L"스킬 목록");
    title->SetIgnoreRayCast(true);

    scroll_box_ = AddChild<UIScrollBox>(UIScrollBox::StaticClass(), L"ScrollBox");
    scroll_box_->SetRelativePosition({ 8.f, 32.f });
    scroll_box_->SetSize({ 204.f, 256.f });
    scroll_box_->SetScrollStep(36.f);
    scroll_box_->SetSpacing(4.f);

    dragging_icon_ = AddChild<UIImage>(UIImage::StaticClass(), L"DraggingIcon");
    dragging_icon_->SetActive(false);
    dragging_icon_->SetIgnoreRayCast(true);

    tooltip_ = AddChild<UISkillTooltip>(UISkillTooltip::StaticClass(), L"SkillTooltip");
    tooltip_->SetActive(false);
}

void UISkillWindow::Init()
{
    UIContainer::Init();

    PublisherSubsystem* publisher = PublisherSubsystem::Get();
    publisher->Subscribe(PublisherSubsystem::EventType::kSkillsUpdated, this, &UISkillWindow::OnSkillUpdate);

    PlayerSubsystem* player = PlayerSubsystem::Get();
    Refresh(player->GetSkillList());
}

void UISkillWindow::Uninit()
{
    PublisherSubsystem* publisher = PublisherSubsystem::Get();
    publisher->Unsubscribe(PublisherSubsystem::EventType::kSkillsUpdated, this, &UISkillWindow::OnSkillUpdate);

    UIContainer::Uninit();
}

void UISkillWindow::Refresh(const std::vector<SkillInfo>& skills)
{
    size_t required_slots = skills.size();
    while (slots_.size() < required_slots)
    {
        UISkillSlot* slot = scroll_box_->AddItem<UISkillSlot>(UISkillSlot::StaticClass(), L"SkillSlot" + std::to_wstring(slots_.size()));
        
        slot->SetOwner(this);
        slots_.push_back(slot);
    }

    for (size_t i = 0; i < slots_.size(); ++i)
    {
        if (i < required_slots)
        {
            slots_[i]->SetSkill(skills[i].skill_id, skills[i].level, skills[i].cooldown);
        }
        else
        {
            slots_[i]->Reset();
        }
    }
    
}

void UISkillWindow::OnSkillUpdate(const EventData& data)
{
    if (const auto* skills = dynamic_cast<const SkillListUpdatedData*>(&data))
    {
        Refresh(skills->skills);
    }
}

bool UISkillWindow::OnDragBegin(const Math::Vector2&) { return true; }
bool UISkillWindow::OnDrag(const Math::Vector2&, const Math::Vector2& delta)
{
    Math::Vector2 p = GetRelativePosition() + delta;
    SetRelativePosition(p);
    return true;
}   
bool UISkillWindow::OnDragEnd(const Math::Vector2&) { return true; }

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<UISkillWindow>("UISkillWindow")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}

