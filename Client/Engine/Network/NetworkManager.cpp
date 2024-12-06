#include "pch.h"
#include "NetworkManager.h"

#include "../../SocketCore/SocketEventManager.h"
#include "Actor/NetworkActor.h"
#include "Actor/Character/Player/PlayerCharacter.h"
#include "Level/World.h"

NetworkManager::NetworkManager() :
    player_class_(PlayerCharacter::StaticClass()),
    players_()
{
}

void NetworkManager::Tick(float delta_time)
{
    SocketEventManager* event_manager = SocketEventManager::Get();
    SocketEvent event;

    while (event_manager->PollEvent(event))
    {
        switch (event.type)
        {
        case S_PKT_ENTER:
            {
                EnterEvent& cache_event = event.enter;
                packet_id_ = cache_event.userId;

                NetworkActor* actor = World::Get()->SpawnActor<NetworkActor>(player_class_);
                actor->packet_id_ = packet_id_;
                actor->is_mine_ = true;

                players_[actor->packet_id_] = actor;

                C_EnterRoom enterRoomPkt;
                enterRoomPkt._roomNum = 0;
                std::shared_ptr<SendBuffer> enterRoomSendBuffer = ServerPacketHandler::MakeSendBuffer<C_EnterRoom>(enterRoomPkt,C_PKT_ENTER_ROOM);
                GSocketSession->Send(enterRoomSendBuffer);

                C_EnterChannel enterChannelPkt;
                enterChannelPkt._channelNum = 0;
                std::shared_ptr<SendBuffer> enterChannelSendBuffer = ServerPacketHandler::MakeSendBuffer<C_EnterChannel>(enterChannelPkt,C_PKT_ENTER_CHANNEL);
                GSocketSession->Send(enterChannelSendBuffer);
                
                C_EnterOtherUserPacket pkt;
                pkt._userId = actor->packet_id_;
                std::shared_ptr<SendBuffer> send_buffer = ServerPacketHandler::MakeSendBuffer<C_EnterOtherUserPacket>(pkt, C_PKT_ENTER_OTHER_USER);
                GSocketSession->Send(send_buffer);
            }
            break;

        case S_PKT_ENTER_OTHER_USER:
            {
                EnterOtherUserEvent& cache_event = event.enterOtherUser;
                for (int i = 0; i < cache_event.currentUserCnt_; ++i)
                {
                    if (cache_event.userIdentifyidArr_[i] == packet_id_) continue;

                    NetworkActor* actor = World::Get()->SpawnActor<NetworkActor>(player_class_);
                    actor->packet_id_ = cache_event.userIdentifyidArr_[i];
                    actor->received_position_ = {cache_event.locationXArr_[i], cache_event.locationYArr_[i]};

                    players_[actor->packet_id_] = actor;
                }
            }
            break;

        case S_PKT_BROADCASTING_ENTER:
            {
                BroadcastingEnterEvent& cache_event = event.broadcastingEnter;
                if (cache_event.userId == packet_id_) break;

                NetworkActor* actor = World::Get()->SpawnActor<NetworkActor>(player_class_);
                actor->packet_id_ = cache_event.userId;
                actor->received_position_ = Math::Vector2::Zero();

                players_[actor->packet_id_] = actor;
            }
            break;

        case S_PKT_LEAVE_OTHER_USER:
            {
                LeaveOtherUserEvent& cache_event = event.leaveOtherUser;

                auto it = players_.find(cache_event.userId);
                if (it != players_.end())
                {
                    NetworkActor* actor = it->second;
                    actor->Destroy();
                    players_.erase(it);
                }
            }
            break;

        case S_PKT_MOVING:
            {
                MovingEvent& cache_event = event.moving;
                if (cache_event.userId == packet_id_) break;

                auto it = players_.find(cache_event.userId);
                if (it != players_.end())
                {
                    NetworkActor* actor = it->second;
                    actor->received_position_ = {cache_event.locationX, cache_event.locationY};
                }
            }
            break;

        case S_PKT_ENTER_ROOM:
            {
                EnterRoom& cache_event = event.enterRoom;

                std::cout << "enter room" << cache_event.roomNum << std::endl;
            }
            break;

        case S_PKT_ENTER_CHANNEL:
            {
                EnterChannel& cache_event = event.enterChannel;

                std::cout << "enter channel" << cache_event.currentChannelNum << std::endl;
            }
            break;

        case S_PKT_LEAVE_CHANNEL:
            {
                LeaveChannel& cache_event = event.leaveChannel;

                std::cout << "Leave Channel" << cache_event.userId << std::endl;
            }
        }
    }
}
