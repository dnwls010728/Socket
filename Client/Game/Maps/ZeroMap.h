#pragma once
#include "Level/Level.h"

namespace Net
{
    struct IPacket;
}

namespace UI
{
    class ListBox;
    class EditableTextBox;
}

class ZeroMap : public Level
{
    SHADER_CLASS_HELPER(ZeroMap)
    GENERATED_BODY(ZeroMap, Level)
    
public:
    ZeroMap(const std::wstring& kName);
    virtual ~ZeroMap() override = default;

protected:
    virtual void Load() override;
    virtual void Unload(EndPlayReason type) override;

#pragma region UI
    std::shared_ptr<UI::ListBox> chat_list_;
    std::shared_ptr<UI::EditableTextBox> chat_input_;
#pragma endregion

private:
    void ProcessPackets(const std::shared_ptr<Net::IPacket>& packet);
    void OnChatInput(const std::wstring& kText);
    
};
