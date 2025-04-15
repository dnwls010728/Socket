#include "pch.h"
#include "LobbyMap.h"

#include <CustomPacket.h>

#include "GameInstance.h"
#include "Input/Keyboard.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/UIManager.h"
#include "UI/Widget/EditableTextBox.h"
#include "UI/Widget/ListBox.h"

LobbyMap::LobbyMap(const std::wstring& kName) :
    Level(kName),
    room_list_box_(nullptr),
    room_name_editable_text_box_(nullptr)
{
}

void LobbyMap::Load()
{
    Level::Load();

    SessionSubsystem* session_subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (session_subsystem) session_subsystem->packet_handler.Add(this, &LobbyMap::ProcessPackets);

    SubscribeRoomRefresh();

    UI::Manager* ui_manager = UI::Manager::Get();
    if (ui_manager)
    {
        room_list_box_ = UI::ListBox::Create(L"RoomListBox");
        room_list_box_->SetPosition({400.f, 300.f});
        room_list_box_->SetSize({300.f, 400.f});

        ui_manager->AddToViewport(room_list_box_);

        room_name_editable_text_box_ = UI::EditableTextBox::Create(L"RoomNameEditableTextBox");
        room_name_editable_text_box_->SetPosition({675.f, 200.f});
        room_name_editable_text_box_->SetSize({100.f, 30.f});
        room_name_editable_text_box_->OnReturn([&](const std::wstring& text)
        {
            SessionSubsystem* session_subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
            if (!session_subsystem) return;

            CreateRoomPacketReq create_room_request;
            create_room_request.room_title = text;
            create_room_request.max_user_count = 4;
            session_subsystem->SendPacket(create_room_request);
        });
        room_list_box_->OnDoubleClick([&](Type::uint64 user_data)
        {
            RoomInfo* room_info = reinterpret_cast<RoomInfo*>(user_data);
        });

        ui_manager->AddToViewport(room_name_editable_text_box_);
    }
}

void LobbyMap::Unload(EndPlayReason type)
{
    Level::Unload(type);

    UnsubscribeRoomRefresh();
    
    SessionSubsystem* session_subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (session_subsystem) session_subsystem->packet_handler.Remove(this, &LobbyMap::ProcessPackets);
}

void LobbyMap::Tick(float delta_time)
{
    Level::Tick(delta_time);

    Keyboard* keyboard = Keyboard::Get();
    if (keyboard)
    {
        if (keyboard->GetKeyDown('R'))
        {
            World::Get()->OpenLevel(L"LobbyMap");
        }
    }
}

void LobbyMap::ProcessPackets(std::shared_ptr<Net::IPacket> packet)
{
    if (!packet) return;

    UI::Manager* ui_manager = UI::Manager::Get();
    if (!ui_manager) return;
    
    switch (packet->GetPacketID())
    {
    case CreateRoomPacketAck::StaticPacketID:
        {
            std::shared_ptr<CreateRoomPacketAck> create_room_packet = std::dynamic_pointer_cast<CreateRoomPacketAck>(packet);
            if (create_room_packet && !create_room_packet->result)
            {
                Logger::Print(L"방 생성에 실패했습니다.");
                break;
            }

            ui_manager->RemoveFromViewport(room_list_box_);
            ui_manager->RemoveFromViewport(room_name_editable_text_box_);
            
            UnsubscribeRoomRefresh();
        }
        break;

    case RoomListPacketAck::StaticPacketID:
        {
            std::shared_ptr<RoomListPacketAck> room_list_packet = std::dynamic_pointer_cast<RoomListPacketAck>(packet);
            
            RoomList& room_list_data = room_list_packet->room_list;
            for (auto& room_info : room_list_data.room_list)
            {
                AddRoom(room_info);
            }
        }
        break;

    case OnUpdateRoomListPacket::StaticPacketID:
        {
            std::shared_ptr<OnUpdateRoomListPacket> room_update_packet = std::dynamic_pointer_cast<OnUpdateRoomListPacket>(packet);
            RoomInfo& room_info = room_update_packet->room_info;
            
            std::wstring room_title = room_info.room_title;
            room_title += L" (" + std::to_wstring(room_info.current_user_count) + L"/" + std::to_wstring(room_info.max_user_count) + L")";

            switch (room_update_packet->update_type)
            {
            case kRoomAdd: AddRoom(room_info); break;
            case kRoomModify: ModifyRoom(room_info); break;
            case kRoomRemove: RemoveRoom(room_info.room_number); break;
            default: break;
            }
        }
        break;

    default:
        break;
    }
}

void LobbyMap::SubscribeRoomRefresh()
{
    SessionSubsystem* session_subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (!session_subsystem) return;

    RoomListPacketReq room_list_request;
    session_subsystem->SendPacket(room_list_request);

    ObserveRoomListPacket observe_room_list_packet;
    observe_room_list_packet.is_observe = true;
    session_subsystem->SendPacket(observe_room_list_packet);
    
}

void LobbyMap::UnsubscribeRoomRefresh()
{
    SessionSubsystem* session_subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (!session_subsystem) return;

    ObserveRoomListPacket observe_room_list_packet;
    observe_room_list_packet.is_observe = false;
    session_subsystem->SendPacket(observe_room_list_packet);
}

void LobbyMap::AddRoom(RoomInfo& room_info)
{
    std::wstring room_title = room_info.room_title;
    room_title += L" (" + std::to_wstring(room_info.current_user_count) + L"/" + std::to_wstring(room_info.max_user_count) + L")";
    
    room_list_box_->AddItem(room_title, reinterpret_cast<uintptr_t>(&room_info));
}

void LobbyMap::RemoveRoom(int room_number)
{
    const std::vector<UI::ListBox::Item>& items = room_list_box_->GetItems();
    for (int i = 0; i < items.size(); ++i)
    {
        RoomInfo* room_info = reinterpret_cast<RoomInfo*>(items[i].user_data);
        if (room_info->room_number == room_number)
        {
            room_list_box_->RemoveItem(i);
            break;
        }
    }
}

void LobbyMap::ModifyRoom(RoomInfo& room_info)
{
    const std::vector<UI::ListBox::Item>& items = room_list_box_->GetItems();
    for (int i = 0; i < items.size(); ++i)
    {
        RoomInfo* room_info_ptr = reinterpret_cast<RoomInfo*>(items[i].user_data);
        if (room_info_ptr->room_number == room_info.room_number)
        {
            std::wstring room_title = room_info_ptr->room_title;
            room_title += L" (" + std::to_wstring(room_info_ptr->current_user_count) + L"/" + std::to_wstring(room_info_ptr->max_user_count) + L")";
            
            room_list_box_->SetItem(i, room_title, reinterpret_cast<uintptr_t>(room_info_ptr));
            break;
        }
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;
    
    registration::class_<LobbyMap>("LobbyMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
