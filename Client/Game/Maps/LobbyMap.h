#pragma once
#include <CommonObject.h>

#include "Level/Level.h"
#include "UI/Widget.h"

namespace Net
{
    struct IPacket;
}

namespace UI
{
    class Button;
    class EditableTextBox;
    class ListBox;
}

class LobbyMap : public Level
{
    SHADER_CLASS_HELPER(LobbyMap)
    GENERATED_BODY(LobbyMap, Level)
    
public:
    LobbyMap(const std::wstring& kName);
    virtual ~LobbyMap() override = default;

    virtual void Load() override;
    virtual void Unload(EndPlayReason type) override;
    virtual void Tick(float delta_time) override;

private:
    void ProcessPackets(std::shared_ptr<Net::IPacket> packet);
    void SubscribeRoomRefresh();
    void UnsubscribeRoomRefresh();
    void AddRoom(RoomInfo& room_info);
    void RemoveRoom(int room_number);
    void ModifyRoom(RoomInfo& room_info);
    
#pragma region UI Widget
    std::shared_ptr<UI::ListBox> room_list_box_;
    std::shared_ptr<UI::EditableTextBox> room_name_editable_text_box_;
    std::shared_ptr<UI::Button> back_button_;
#pragma endregion
    
};
