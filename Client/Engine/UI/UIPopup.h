#pragma once
#include "UI/UIContainer.h"
#include <functional>

class UIImage;
class UIText;
class UIButton;
class UIEditableText;

enum class PopupOption : uint32_t 
{
    None    = 0,
    OK      = 1 << 0,
    Cancel  = 1 << 1,
    Yes     = 1 << 2,
    No      = 1 << 3,
    Edit    = 1 << 10,
};
ENUM_CLASS_FLAGS(PopupOption)

class UIPopup : public UIContainer
{
    GENERATED_BODY(UIPopup, UIContainer)

public:
    UIPopup(const std::wstring& name);
    virtual ~UIPopup() override = default;

    static void ShowPopup(std::wstring caption, PopupOption option, std::function<bool(std::wstring, PopupOption)> callback);

    void SetPopup(const std::wstring& caption, PopupOption option);
    void SetCallback(std::function<void(std::wstring input_text, PopupOption)> callback);

    void SetFoucsInputText();
    std::wstring GetInputText() const;

protected:
    virtual void Init() override;
    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;
    virtual bool OnKey(uint16_t key_code, bool is_pressed) override;

    virtual void SetMessage(const std::wstring& text);
    virtual void SetOptions(PopupOption option);
    UIButton* CreateButton(const std::wstring& label, PopupOption opt);

private:
    void UpdateLayout();

private:
    UIImage* background_;
    UIText* message_text_;
    std::map<PopupOption, UIButton*> buttons_;
    UIEditableText* input_text_;
    UIImage* input_background_;

    std::function<void(std::wstring input_text, PopupOption)> result_callback_;
    PopupOption current_option_;


    static constexpr float kElementMargin       = 15.0f;
    static constexpr float kPadding             = 20.f;
    static constexpr float kInputHeight         = 30.f;
    static constexpr float kButtonWidth         = 50.f;
    static constexpr float kButtonHeight        = 27.f;
};
