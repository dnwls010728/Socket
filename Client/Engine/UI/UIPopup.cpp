// UIPopup.cpp
#include "pch.h"
#include "UIPopup.h"

#include "Scancode.h"
#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIText.h"
#include "UI/Element/UIButton.h"
#include "UI/Element/UIEditableText.h"
#include "UI/UIState.h"
#include "Windows/DX/UISprite.h"

UIPopup::UIPopup(const std::wstring& name)
    : UIContainer(name),
      background_(nullptr),
      caption_control_(nullptr),
      input_control_(nullptr),
      input_background_(nullptr),
      current_option_(PopupOption::None),
      result_callback_([](const std::wstring&, PopupOption){})
{
    auto* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");

    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);

    caption_control_ = AddChild<UIText>(UIText::StaticClass(), L"Message");
    caption_control_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    caption_control_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    caption_control_->SetColor(Math::Color::White);
    caption_control_->SetIgnoreRayCast(true);

    SetSize({150.f,200.f});
}

void UIPopup::Init()
{
    UIContainer::Init();
    background_->SetSize(GetSize());
}

void UIPopup::ShowPopup(const PopupParam &param)
{
    UIState* state = UI::Get()->GetState();
    if (state == nullptr)
        return;
    
    UIPopup *popup = state->AddElement<UIPopup>(UIPopup::StaticClass(), L"Popup");
    popup->PopupSetting(param);
    
    Math::Vector2 pos{
        std::round((EngineSettings::Get()->GetScreenWidth() - popup->GetSize().x) / 2),
        std::round((EngineSettings::Get()->GetScreenHeight() - popup->GetSize().y) / 2)
    };
    
    popup->SetAbsolutePosition(pos);
    popup->SetActive(true);
    
    if ((param.option & PopupOption::Edit)  == PopupOption::Edit)
        popup->SetFoucsInputText();
    else
       UI::Get()->SetFocus(popup);
}


void UIPopup::PopupSetting(const PopupParam &param)
{
    current_option_ = param.option;
    caption_control_->SetText(param.caption);
    
    UpdateControl(param.option);
    if ((param.option & PopupOption::Edit) == PopupOption::Edit && input_control_ )
    {
        input_control_->SetPlaceholderText(param.placeholder);
        input_control_->SetText(param.default_input_text);
        input_control_->SetContentType(param.content_type);
        input_control_->SetCharacterLimit(param.input_limit);
    }
    UpdateLayout();

    result_callback_ = [this, callback = param.callback](const std::wstring& input_text, PopupOption option)
    {
        if (callback(input_text, option))
        {
            UIState* state=UI::Get()->GetState();
            state->PostTask([state, this]() {
                SetActive(false);
                state->RemoveElement(this);
            });
        }
    };
}

void UIPopup::UpdateControl(PopupOption option)
{
    // 에딧 박스
    if ((option & PopupOption::Edit) != PopupOption::None)
    {
        if (!input_control_)
        {
            UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");

            input_background_ = AddChild<UIImage>(UIImage::StaticClass(), L"IDBackground");
            input_background_->SetSprite(panel_sprite, L"Panel_0");
            input_background_->SetDrawMode(UIImage::DrawMode::kSliced);
            input_background_->SetIgnoreRayCast(true);
            input_control_ = AddChild<UIEditableText>(UIEditableText::StaticClass(), L"Input");
        }
        input_control_->SetActive(true);
    }
    else
    {
        if (input_control_)
        {
            RemoveChild(input_control_);
            input_control_ = nullptr;
        }
        if (input_background_)
        {
            RemoveChild(input_background_);
            input_background_ = nullptr;
        }
    }

    std::vector<std::pair<PopupOption, std::wstring>> all_button_defs = {
        {PopupOption::OK, L"확인"},
        {PopupOption::Cancel, L"취소"},
        {PopupOption::Yes, L"예"},
        {PopupOption::No, L"아니요"}
    };

    for (const auto& [opt, label] : all_button_defs)
    {
        bool is_exist = (option & opt) != PopupOption::None;

        if (is_exist)
        {
            if (!buttons_.count(opt))
                buttons_[opt] = CreateButton(label, opt);
        }
        else
        {
            if (buttons_.count(opt))
            {
                RemoveChild(buttons_[opt]);
                buttons_.erase(opt);
            }
        }
    }
}

UIButton* UIPopup::CreateButton(std::wstring_view label, PopupOption opt)
{
    UISprite* button_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\ButtonSheet.png");
    
    UIButton* btn = AddChild<UIButton>(UIButton::StaticClass(), label.data());
    btn->SetSize({kButtonWidth, kButtonHeight});
    btn->SetText(label.data());
    btn->SetTextColor(Math::Color::White);
    btn->SetSprite(UIButton::State::kNormal, button_sprite, L"ButtonSheet_0");
    btn->SetSprite(UIButton::State::kHover, button_sprite, L"ButtonSheet_1");
    btn->SetSprite(UIButton::State::kPressed, button_sprite, L"ButtonSheet_2");
    btn->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    btn->SetDrawMode(UIImage::DrawMode::kSliced);
    btn->OnClick([this, opt]()
    {
        std::wstring input_text = input_control_ ? input_control_->GetText() : L"";
        result_callback_(input_text, opt);
    });
    btn->SetActive(true);
    return btn;
};

void UIPopup::SetFoucsInputText()
{
    if (input_control_)
        UI::Get()->SetFocus(input_control_);
}

void UIPopup::UpdateLayout()
{
    float now_y = kPadding;
    float now_x = kPadding;
    int button_count = buttons_.size();

    // 버튼들이 차지하는 너비
    const float button_line_width = button_count * kButtonWidth + (button_count - 1) * kElementMargin;
    
    // 캡션 컨트롤 배치
    float text_width = 0;
    float text_height = 0;
    
    Renderer::Get()->GetTextSize(caption_control_->GetText(), caption_control_->GetFontName(), caption_control_->GetFontSize(),
        text_width, text_height);
    text_width += 20;
    text_width = std::max(text_width, button_line_width);

    caption_control_->SetSize({ text_width, text_height });
    caption_control_->SetAbsolutePosition({ now_x, now_y });
    caption_control_->SetActive(true);

    now_y += text_height;
    const float ui_width = text_width + kPadding * 2;

    // 인풋 컨트롤 배치
    if (input_control_)
    {
        now_y += kElementMargin;
        if (input_background_)
        {
            input_background_->SetRelativePosition({ now_x, now_y });
            input_background_->SetSize({ text_width, kInputHeight });
        }
        
        input_control_->SetRelativePosition({ now_x + 10, now_y });
        input_control_->SetSize({ text_width - 20, kInputHeight });
       
        now_y += kInputHeight ;
    }


    // 버튼 배치
    now_y += kElementMargin;
    const float button_start_x = std::round((ui_width - button_line_width) / 2.f);
    int offset = 0;
    for (auto &[opt, btn] :  buttons_)
    {
        float x = button_start_x + offset * (kButtonWidth + kElementMargin);
        btn->SetRelativePosition({ x, now_y });
        btn->SetSize({ kButtonWidth, kButtonHeight });
        offset++;
    }
    now_y += kButtonHeight;
    
    now_y += kPadding;
    SetSize({ui_width, now_y});
    background_->SetSize({ui_width, now_y});
}

bool UIPopup::OnDragBegin(const Math::Vector2&) { return true; }
bool UIPopup::OnDrag(const Math::Vector2&, const Math::Vector2& delta)
{
    Math::Vector2 new_position = GetRelativePosition() + delta;
    SetRelativePosition(new_position);
    return true;
}
bool UIPopup::OnDragEnd(const Math::Vector2&) { return true; }

bool UIPopup::OnKey(uint32_t scancode, bool is_pressed)
{
    if (is_pressed)
    {
        switch (scancode)
        {
        case static_cast<uint32_t>(Scancode::kKeyEnter):
            {
                SubmitOnEnter();
                return true;
            }
        }
    }
    
    return false;
}

void UIPopup::SubmitOnEnter()
{
    std::vector<PopupOption> active_buttons = {PopupOption::OK, PopupOption::Yes, PopupOption::Cancel, PopupOption::No};
    for (const auto& button : active_buttons)
    {
        if ((current_option_ & button) != PopupOption::None)
        {
            std::wstring input_text = input_control_ ?  input_control_->GetText() : caption_control_->GetText();
            result_callback_(input_text, PopupOption::OK);
            break;
        }
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<UIPopup>("UIPopup")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
