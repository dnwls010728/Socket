#pragma once
#include "UI/UIContainer.h"
#include <functional>

#include "Element/UIEditableText.h"

class UIImage;
class UIText;
class UIButton;
class UIEditableText;

class UIPopup : public UIContainer
{
    GENERATED_BODY(UIPopup, UIContainer)
    enum class PopupOption : uint32_t 
    {
        None    = 0,
        OK      = 1 << 0,
        Cancel  = 1 << 1,
        Yes     = 1 << 2,
        No      = 1 << 3,
        Edit    = 1 << 10,
    };
    
    struct PopupParam
    {
        PopupParam() :
        callback([](const std::wstring&, PopupOption){return true;}),
        content_type(UIEditableText::ContentType::kStandard)
        {}
        PopupOption option;
        std::wstring caption;
        std::wstring default_input_text;
        UIEditableText::ContentType content_type;
        std::wstring placeholder;
        Function<bool(const std::wstring& input_text, PopupOption)> callback;
    };
public:
    UIPopup(const std::wstring& name);
    virtual ~UIPopup() override = default;
    
    static void ShowPopup(const PopupParam &param);

    virtual void PopupSetting(const PopupParam& param);
protected:
    virtual void Init() override;
    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;
    virtual bool OnKey(uint16_t key_code, bool is_pressed) override;
    void SubmitOnEnter();

    virtual void UpdateControl(PopupOption option);
    virtual UIButton* CreateButton(std::wstring_view label, PopupOption opt);
    virtual void UpdateLayout();
    
    inline virtual UIEditableText* GetInputControl() const { return input_control_; }
    inline virtual UIEditableText* GetCaptionControl() const { return input_control_; }
    inline virtual UIImage* GetBackgroundImage() const { return background_; }
    inline virtual UIImage* GetInputBackgroundImage() const { return  input_background_; }
    inline virtual std::map<PopupOption, UIButton*>& GetButtons() { return buttons_; }

private:
    void SetFoucsInputText();

    UIEditableText* input_control_;
    UIText* caption_control_;
    std::map<PopupOption, UIButton*> buttons_;
    UIImage* background_;
    UIImage* input_background_;
    
    Function<void(const std::wstring& input_text, PopupOption)> result_callback_;
    PopupOption current_option_;
    
    static constexpr float kElementMargin       = 15.0f;
    static constexpr float kPadding             = 20.f;
    static constexpr float kInputHeight         = 30.f;
    static constexpr float kButtonWidth         = 50.f;
    static constexpr float kButtonHeight        = 27.f;
};

ENUM_CLASS_FLAGS(UIPopup::PopupOption)