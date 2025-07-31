// UIPopup.cpp
#include "pch.h"
#include "UIPopup.h"

#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIText.h"
#include "UI/Element/UIButton.h"
#include "UI/Element/UIEditableText.h"
#include "Windows/DX/UISprite.h"

UIPopup::UIPopup(const std::wstring& name)
    : UIContainer(name),
      background_(nullptr),
      message_text_(nullptr),
      input_text_(nullptr),
      input_background_(nullptr),
      current_option_(PopupOption::None)
{
    auto* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");

    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);

    message_text_ = AddChild<UIText>(UIText::StaticClass(), L"Message");
    message_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    message_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    message_text_->SetColor(Math::Color::White);
    message_text_->SetIgnoreRayCast(true);

    SetSize({150.f,200.f});
    //SetPopup(L"",PopupOption::OK);
}

void UIPopup::SetPopup(const std::wstring& caption, PopupOption option)
{
    SetMessage(caption);
    SetOptions(option);
    UpdateLayout();
}

void UIPopup::SetMessage(const std::wstring& text)
{
    message_text_->SetText(text);
}

void UIPopup::SetOptions(PopupOption option)
{
    current_option_ = option;

    // 에딧 박스
    if ((option & PopupOption::Edit) != PopupOption::None)
    {
        if (!input_text_)
        {
            UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");

            input_background_ = AddChild<UIImage>(UIImage::StaticClass(), L"IDBackground");
            input_background_->SetSprite(panel_sprite, L"Panel_0");
            input_background_->SetDrawMode(UIImage::DrawMode::kSliced);
            input_background_->SetIgnoreRayCast(true);
            input_text_ = AddChild<UIEditableText>(UIEditableText::StaticClass(), L"Input");
        }
        input_text_->SetActive(true);
    }
    else
    {
        if (input_text_)
        {
            RemoveChild(input_text_);
            input_text_ = nullptr;
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

UIButton* UIPopup::CreateButton(const std::wstring& label, PopupOption opt)
{
    UISprite* button_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\ButtonSheet.png");
    
    UIButton* btn = AddChild<UIButton>(UIButton::StaticClass(), label);
    btn->SetSize({kButtonWidth, kButtonHeight});
    btn->SetText(label);
    btn->SetTextColor(Math::Color::White);
    btn->SetSprite(UIButton::State::kNormal, button_sprite, L"ButtonSheet_0");
    btn->SetSprite(UIButton::State::kHover, button_sprite, L"ButtonSheet_1");
    btn->SetSprite(UIButton::State::kPressed, button_sprite, L"ButtonSheet_2");
    btn->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    btn->SetDrawMode(UIImage::DrawMode::kSliced);
    btn->OnClick([this, opt]()
    {
        if (result_callback_) result_callback_(GetInputText(), opt);
    });
    btn->SetActive(true);
    return btn;
};

void UIPopup::SetCallback(std::function<void(std::wstring input_text, PopupOption)> callback)
{
    result_callback_ = std::move(callback);
}

void UIPopup::SetFoucsInputText()
{
    if (input_text_)
        UI::Get()->SetFocus(input_text_);
}

std::wstring UIPopup::GetInputText() const
{
    return input_text_ ? input_text_->GetText() : L"";
}

void UIPopup::UpdateLayout()
{
    float now_y = kPadding;
    float now_x = kPadding;
    float ui_width = kPadding*2;

    // 버튼들이 차지하는 너비 계싼
    std::vector<UIButton*> active_buttons;
    for (auto& [opt, btn] : buttons_)
        active_buttons.push_back(btn);
    
    const float button_line_width = active_buttons.size() * kButtonWidth +
        (active_buttons.size() - 1) * kElementMargin;
    
    // 메시지 박스 배치
    float text_width = 0;
    float text_height = 0;
    
    bool get_size_result = Renderer::Get()->GetTextSize(message_text_->GetText(), message_text_->GetFontName(), message_text_->GetFontSize(),
        text_width, text_height);

    if (!get_size_result)
    {
        // 임시로 크기를 크게 잡아 줄바꿈 방지
        message_text_->SetActive(false);
        constexpr float kTempWidth = 10000.f;
        constexpr float kTempHeight = 10000.f;
        message_text_->SetSize({ kTempWidth, kTempHeight });
        
        text_width = message_text_->GetTotalAdvance() ;
        text_height = message_text_->GetTotalLineHeight();
    }
    text_width += 20;
    text_width = std::max(text_width, button_line_width);
    
    message_text_->SetSize({ text_width, text_height });
    message_text_->SetAbsolutePosition({ now_x, now_y });
    message_text_->SetActive(true);

    now_y += text_height;
    ui_width = text_width + kPadding * 2;

    // 입력창 배치
    if (input_text_)
    {
        now_y += kElementMargin;
        if (input_background_)
        {
            input_background_->SetRelativePosition({ now_x, now_y });
            input_background_->SetSize({ text_width, kInputHeight });
        }
        
        input_text_->SetRelativePosition({ now_x + 10, now_y });
        input_text_->SetSize({ text_width - 20, kInputHeight });
       
        now_y += kInputHeight ;
    }

    if (!active_buttons.empty())
    {
        now_y += kElementMargin;

        const float total_button_width = active_buttons.size() * kButtonWidth +
                                         (active_buttons.size() - 1) * kElementMargin;
        const float start_x = std::round((ui_width - total_button_width) / 2.f);

        for (size_t i = 0; i < active_buttons.size(); ++i)
        {
            float x = start_x + i * (kButtonWidth + kElementMargin);
            active_buttons[i]->SetRelativePosition({ x, now_y });
            active_buttons[i]->SetSize({ kButtonWidth, kButtonHeight });
        }

        now_y += kButtonHeight;
    }

    now_y += kPadding;
    SetSize({ui_width, now_y});
    background_->SetSize({ui_width, now_y});
}

void UIPopup::Init()
{
    UIContainer::Init();
    background_->SetSize(GetSize());
}

bool UIPopup::OnDragBegin(const Math::Vector2&) { return true; }
bool UIPopup::OnDrag(const Math::Vector2&, const Math::Vector2& delta) { position_ += delta; return true; }
bool UIPopup::OnDragEnd(const Math::Vector2&) { return true; }

bool UIPopup::OnKey(uint16_t key_code, bool is_pressed)
{
    if (is_pressed)
    {
        switch (key_code)
        {
        case VK_RETURN:
            {
                std::vector<PopupOption> active_buttons = {PopupOption::OK, PopupOption::Yes, PopupOption::Cancel, PopupOption::No};
                for (const auto& button : active_buttons)
                {
                    if ((current_option_ & button) != PopupOption::None)
                    {
                        if (result_callback_)
                            result_callback_(GetInputText(), PopupOption::OK);
                        break;
                    }
                }
                return true;
            }
        }
    }
    
    return false;
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
