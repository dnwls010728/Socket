#include "pch.h"
#include "UICharacterSlot.h"

#include "UICharacterSelect.h"
#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIText.h"
#include "Windows/DX/UISprite.h"

UICharacterSlot::UICharacterSlot(const std::wstring& name) :
    UIContainer(name),
    character_select_(nullptr),
    slot_id_(0),
    character_id_(0),
    last_time_(0.),
    timer_(0.f),
    frame_index_(0)
{
    SetSize({ 200.f, 228.f });
    
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    
    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);

    character_ = AddChild<UIImage>(UIImage::StaticClass(), L"Character");
    character_->SetRelativePosition({ 10.f, 10.f });
    character_->SetSize({ 180.f, 140.f });
    character_->SetActive(false);
    character_->SetIgnoreRayCast(true);

    empty_text_ = AddChild<UIText>(UIText::StaticClass(), L"EmptyText");
    empty_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    empty_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    empty_text_->SetColor(Math::Color::White);
    empty_text_->SetText(L"EMPTY");
    empty_text_->SetIgnoreRayCast(true);

    lv_text_ = AddChild<UIText>(UIText::StaticClass(), L"LvText");
    lv_text_->SetRelativePosition({ 10.f, 160.f });
    lv_text_->SetSize({ 180.f, 20.f });
    lv_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    lv_text_->SetColor(Math::Color::White);
    lv_text_->SetActive(false);
    lv_text_->SetIgnoreRayCast(true);

    name_text_ = AddChild<UIText>(UIText::StaticClass(), L"NameText");
    name_text_->SetRelativePosition({ 10.f, 180.f });
    name_text_->SetSize({ 180.f, 20.f });
    name_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    name_text_->SetColor(Math::Color::White);
    name_text_->SetActive(false);
    name_text_->SetIgnoreRayCast(true);

    color_code_text_ = AddChild<UIText>(UIText::StaticClass(), L"ColorCodeText");
    color_code_text_->SetRelativePosition({ 10.f, 200.f });
    color_code_text_->SetSize({ 180.f, 20.f });
    color_code_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    color_code_text_->SetColor(Math::Color::White);
    color_code_text_->SetActive(false);
    color_code_text_->SetIgnoreRayCast(true);
}

void UICharacterSlot::InitSlot(UICharacterSelect* character_select, uint32_t slot_id, const CharacterProfile& profile)
{
    character_select_ = character_select;
    slot_id_ = slot_id;
    
    if (profile.character_id == 0) return;
    character_id_ = profile.character_id;

    character_->SetActive(true);
    empty_text_->SetActive(false);
    lv_text_->SetActive(true);
    name_text_->SetActive(true);
    color_code_text_->SetActive(true);

    UISprite* character_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\UIPlayerSheet.png");
    character_->SetSprite(character_sprite, L"UIPlayerSheet_0");
    character_->SetColor(Math::Color::HexToColor(profile.body_color));

    int32_t lv = profile.stats.lv;
    lv_text_->SetText(L"레벨 " + std::to_wstring(lv));
    
    name_text_->SetText(profile.gm_level > 0 ? L"[GM]" + profile.name : profile.name);
    color_code_text_->SetText(L"색상코드 #" + profile.body_color);
}

void UICharacterSlot::ClearSlot()
{
    character_->SetSprite(nullptr, L"");
    lv_text_->SetText(L"");
    name_text_->SetText(L"");
    color_code_text_->SetText(L"");

    character_->SetActive(false);
    empty_text_->SetActive(true);
    lv_text_->SetActive(false);
    name_text_->SetActive(false);
    color_code_text_->SetActive(false);

    character_id_ = 0;
}

void UICharacterSlot::Init()
{
    background_->SetSize(GetSize());
    empty_text_->SetSize(GetSize());
    
    UIContainer::Init();
}

void UICharacterSlot::Tick(float delta_time)
{
    UIContainer::Tick(delta_time);
    if (slot_id_ == 0 || character_id_ == 0) return;

    timer_ += delta_time;
    if (timer_ >= 1.f / 10.f)
    {
        UISprite* character_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\UIPlayerSheet.png");

        frame_index_ = (frame_index_ + 1) % 7;
        character_->SetSprite(character_sprite, L"UIPlayerSheet_" + std::to_wstring(frame_index_));
        
        timer_ = 0.f;
    }
}

bool UICharacterSlot::OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp)
{
    UIContainer::OnMouseButton(position, button, is_pressed, timestamp);
    if (slot_id_ == 0 || character_id_ == 0) return false;

    if (is_pressed && button == MouseButton::kLeft)
    {
        character_select_->OnSlotSelected(slot_id_);

        if (timestamp - last_time_ < .2f)
        {
            character_select_->OnCharacterSelected();
            last_time_ = 0.f;
            return true;
        }

        last_time_ = timestamp;
        return true;
    }
    
    return false;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UICharacterSlot>("UICharacterSlot")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
