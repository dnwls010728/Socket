#pragma once
#include "Level/Level.h"

namespace Net
{
    struct IPacket;
}

namespace UI
{
    class Button;
    class EditableTextBox;
}

class LoginMap : public Level
{
    SHADER_CLASS_HELPER(Level)
    GENERATED_BODY(LoginMap, Level);
    
public:
    LoginMap(const std::wstring& kName);
    virtual ~LoginMap() override = default;

protected:
    virtual void Load() override;
    virtual void Unload(EndPlayReason type) override;

private:
    void ProcessPackets(std::shared_ptr<Net::IPacket> packet);
    void OnRegister();
    void OnLogin();
    void OnRegisterSwitch();
    void OnLoginSwitch();

#pragma region UI
    std::shared_ptr<UI::EditableTextBox> register_id_;
    std::shared_ptr<UI::EditableTextBox> register_password_;

    std::shared_ptr<UI::Button> register_;
    std::shared_ptr<UI::Button> login_switch_;
    
    std::shared_ptr<UI::EditableTextBox> login_id_;
    std::shared_ptr<UI::EditableTextBox> login_password_;

    std::shared_ptr<UI::Button> login_;
    std::shared_ptr<UI::Button> register_switch_;
#pragma endregion
    
};
