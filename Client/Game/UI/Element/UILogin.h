#pragma once
#include "UI/UIContainer.h"
#include "UI/Element/UIButton.h"

class UIEditableText;
class UIImage;

class UILogin : public UIContainer
{
    GENERATED_BODY(UILogin, UIContainer)
    
public:
    UILogin(const std::wstring& name);
    virtual ~UILogin() override = default;

    void SetLoginDisabled(bool is_disabled) const;

protected:
    virtual void Init() override;

    virtual bool OnKey(uint32_t scancode, bool is_pressed) override;

private:
    void OnLogin();
    void OnRegister();
    
    UIImage* background_;
    UIImage* id_background_;
    UIImage* password_background_;
    
    UIEditableText* input_id_;
    UIEditableText* input_password_;

    UIButton* login_button_;
    UIButton* register_button_;
    
};
