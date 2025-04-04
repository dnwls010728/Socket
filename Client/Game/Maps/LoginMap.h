#pragma once
#include "Level/Level.h"

namespace UI
{
    class EditableTextBox;
    class Button;
    class TextBox;
}

class LoginMap : public Level
{
    SHADER_CLASS_HELPER(LoginMap)
    GENERATED_BODY(LoginMap, Level);
    
public:
    LoginMap(const std::wstring& kName);
    virtual ~LoginMap() override = default;

    virtual void Load() override;

private:
    void OnShowRegister();
    void OnShowLogin();
    void OnPasswordConfirm(const std::wstring& kValue);
    
#pragma region 로그인
    std::shared_ptr<UI::EditableTextBox> login_id_;
    std::shared_ptr<UI::EditableTextBox> login_password_;

    std::shared_ptr<UI::Button> login_;
    std::shared_ptr<UI::Button> register_;
#pragma endregion

#pragma region 회원가입
    std::shared_ptr<UI::EditableTextBox> register_id_;
    std::shared_ptr<UI::EditableTextBox> register_password_;
    std::shared_ptr<UI::EditableTextBox> register_password_confirm_;

    std::shared_ptr<UI::TextBox> register_message_;

    std::shared_ptr<UI::Button> register_ok_;
    std::shared_ptr<UI::Button> register_cancel_;
#pragma endregion
    
};
