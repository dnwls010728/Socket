#pragma once
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Level/Level.h"

namespace Net
{
    struct IPacket;
}

namespace UI_OLD
{
    class Image;
    class TextBox;
    class ListBox;
    class Button;
    class EditableTextBox;
}

class LoginMap : public Level
{
    SHADER_CLASS_HELPER(Level)
    GENERATED_BODY(LoginMap, Level)
    
public:
    LoginMap(const std::wstring& kName);
    virtual ~LoginMap() override = default;

protected:
    virtual void Load() override;
    
};
