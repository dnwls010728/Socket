#include "pch.h"
#include "UICharacterCreate.h"

#include <CustomPacket.h>

#include "Asset/AssetManager.h"
#include "CharacterSelect/UICharacterSelect.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/UIPopup.h"
#include "UI/UIState.h"
#include "UI/Element/UIButton.h"
#include "UI/Element/UIEditableText.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/ColorPicker/UIColorPicker.h"
#include "Windows/DX/UISprite.h"

UICharacterCreate::UICharacterCreate(const std::wstring& name) :
    UIContainer(name),
    timer_(0.f),
    frame_index_(0)
{
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    UISprite* button_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\ButtonSheet.png");
    UISprite* character_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\UIPlayerSheet.png");

    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetRelativePosition({234.f, 0.f});
    background_->SetSize({354.f , 363.f});
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);

    title_text_ = AddChild<UIText>(UIText::StaticClass(), L"TitleText");
    title_text_->SetRelativePosition({244.f, 10.f});
    title_text_->SetSize({334.f, 20.f});
    title_text_->SetFontSize(18);
    title_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    title_text_->SetColor(Math::Color::White);
    title_text_->SetText(L"캐릭터 생성");

    character_ = AddChild<UIImage>(UIImage::StaticClass(), L"Character");
    character_->SetRelativePosition({10.f, 40.f});
    character_->SetSize({224, 200.f});
    character_->SetSprite(character_sprite, L"UIPlayerSheet_0");
    
    color_picker_ = AddChild<UIColorPicker>(UIColorPicker::StaticClass(), L"ColorPicker");
    color_picker_->SetRelativePosition({244.f, 40.f});
    color_picker_->OnValueChanged(this, &UICharacterCreate::OnColorChanged);

    color_text_ = AddChild<UIText>(UIText::StaticClass(), L"ColorText");
    color_text_->SetRelativePosition({494.f, 134.f});
    color_text_->SetSize({84.f, 20.f});
    color_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    color_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    color_text_->SetColor(Math::Color::White);
    color_text_->SetText(L"#FFFFFF");

    name_background_ = AddChild<UIImage>(UIImage::StaticClass(), L"NameBackground");
    name_background_->SetRelativePosition({244.f, 250.f});
    name_background_->SetSize({224.f, 40.f});
    name_background_->SetSprite(panel_sprite, L"Panel_0");
    name_background_->SetDrawMode(UIImage::DrawMode::kSliced);
    name_background_->SetIgnoreRayCast(true);

    name_input_ = AddChild<UIEditableText>(UIEditableText::StaticClass(), L"NameInput");
    name_input_->SetRelativePosition({254.f, 260.f});
    name_input_->SetSize({204.f, 20.f});
    name_input_->SetPlaceholderText(L"최대 12글자");
    name_input_->SetContentType(UIEditableText::ContentType::kAlphanumeric);
    name_input_->SetCharacterLimit(12);
    name_input_->OnValueChanged([&](const std::wstring& text) { confirm_button_->SetDisabled(true); });

    check_name_button_ = AddChild<UIButton>(UIButton::StaticClass(), L"CheckNameButton");
    check_name_button_->SetRelativePosition({478.f, 250.f});
    check_name_button_->SetSize({100.f, 40.f});
    check_name_button_->SetSprite(UIButton::State::kNormal, button_sprite, L"ButtonSheet_0");
    check_name_button_->SetSprite(UIButton::State::kHover, button_sprite, L"ButtonSheet_1");
    check_name_button_->SetSprite(UIButton::State::kPressed, button_sprite, L"ButtonSheet_2");
    check_name_button_->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    check_name_button_->SetDrawMode(UIImage::DrawMode::kSliced);
    check_name_button_->SetTextColor(Math::Color::White);
    check_name_button_->SetText(L"중복확인");
    check_name_button_->OnClick(this, &UICharacterCreate::OnClick);

    confirm_button_ = AddChild<UIButton>(UIButton::StaticClass(), L"ConfirmButton");
    confirm_button_->SetRelativePosition({244.f, 300.f});
    confirm_button_->SetSize({162.f, 53.f});
    confirm_button_->SetSprite(UIButton::State::kNormal, button_sprite, L"ButtonSheet_0");
    confirm_button_->SetSprite(UIButton::State::kHover, button_sprite, L"ButtonSheet_1");
    confirm_button_->SetSprite(UIButton::State::kPressed, button_sprite, L"ButtonSheet_2");
    confirm_button_->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    confirm_button_->SetDrawMode(UIImage::DrawMode::kSliced);
    confirm_button_->SetTextColor(Math::Color::White);
    confirm_button_->SetText(L"확인");
    confirm_button_->OnClick(this, &UICharacterCreate::OnConfirm);
    confirm_button_->SetDisabled(true);

    cancel_button_ = AddChild<UIButton>(UIButton::StaticClass(), L"CancelButton");
    cancel_button_->SetRelativePosition({416.f, 300.f});
    cancel_button_->SetSize({162.f, 53.f});
    cancel_button_->SetSprite(UIButton::State::kNormal, button_sprite, L"ButtonSheet_0");
    cancel_button_->SetSprite(UIButton::State::kHover, button_sprite, L"ButtonSheet_1");
    cancel_button_->SetSprite(UIButton::State::kPressed, button_sprite, L"ButtonSheet_2");
    cancel_button_->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    cancel_button_->SetDrawMode(UIImage::DrawMode::kSliced);
    cancel_button_->SetTextColor(Math::Color::White);
    cancel_button_->OnClick(this, &UICharacterCreate::OnCancel);
    cancel_button_->SetText(L"취소");
}

void UICharacterCreate::OnCheckResult(bool is_available) const
{
    check_name_button_->SetDisabled(false);
    confirm_button_->SetDisabled(!is_available);
}

void UICharacterCreate::Init()
{
    // background_->SetSize(GetSize());
    
    UIContainer::Init();
}

void UICharacterCreate::Tick(float delta_time)
{
    UIContainer::Tick(delta_time);

    timer_ += delta_time;
    if (timer_ >= 1.f / 10.f)
    {
        UISprite* character_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\UIPlayerSheet.png");
        
        frame_index_ = (frame_index_ + 1) % 7;
        character_->SetSprite(character_sprite, L"UIPlayerSheet_" + std::to_wstring(frame_index_));

        timer_ = 0.f;
    }
}

void UICharacterCreate::Render()
{
    Renderer* renderer = Renderer::Get();
    UIContainer::Render();

    Math::Vector2 position = GetAbsolutePosition();
    renderer->DrawSolidBox(
        position + Math::Vector2(494, 40),
        {84.f, 84.f},
        color_picker_->GetColor()
    );
}

void UICharacterCreate::OnColorChanged(const Math::Color& color) const
{
    character_->SetColor(color);
    
    color_text_->SetText(L"#" + Math::Color::ColorToHex(color));
}

void UICharacterCreate::OnClick() const
{
    if (name_input_->GetText().empty())
    {
        UIPopup::ShowPopup(L"이 이름은 사용할 수 없습니다.", PopupOption::OK, [&](std::wstring input_text, PopupOption option)
        {
            return true;
        });
        return;
    }
    
    CheckNameRequest request;
    request.name = name_input_->GetText();
    SessionSubsystem::Get()->SendPacket(request);

    check_name_button_->SetDisabled(true);
}

void UICharacterCreate::OnConfirm() const
{
    CreateCharacterRequest request;
    request.name = name_input_->GetText();
    request.character_color = Math::Color::ColorToHex(color_picker_->GetColor());
    SessionSubsystem::Get()->SendPacket(request);

    confirm_button_->SetDisabled(true);
}

void UICharacterCreate::OnCancel()
{
    SetActive(false);
    
    if (auto* state = UI::Get()->GetState())
    {
        if (auto* element = state->FindElement<UICharacterSelect>(L"CharacterSelect"))
            element->SetActive(true);
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UICharacterCreate>("UICharacterCreate")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
