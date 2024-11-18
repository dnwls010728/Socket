#include "pch.h"
#include "Network.h"

#include "../SocketCore/SocketEventManager.h"
#include "Actor/Character/Player/PlayerCharacter.h"
#include "Actor/Component/TransformComponent.h"
#include "Level/World.h"

Network::Network(const std::wstring& kName) :
    Actor(kName)
{
}

void Network::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
    
    //Socket 이벤트 처리
    SocketEventManager* socket_event_manager = SocketEventManager::Get();
    SocketEvent socketEvent{};

    while(socket_event_manager->PollEvent(socketEvent))
    {
        if(socketEvent.type == S_PKT_ENTER)
        {
            
                    
            PlayerCharacter* player = World::Get()->SpawnActor<PlayerCharacter>(PlayerCharacter::StaticClass());
                    
            player->SetPacketId(socketEvent.enter.userId);
            std::wstring playerName;
            playerName.assign(socketEvent.enter.name.begin(),socketEvent.enter.name.end());
            player->SetNickname(playerName);
            current_player_id = socketEvent.enter.userId;
            players_.insert({player->GetPacketId(), player});
                    
        }else if(socketEvent.type == S_PKT_MOVING)
        {
            MovingEvent evt = socketEvent.moving;
            if(current_player_id != evt.userId)
            {
                auto it = players_.find(evt.userId);
                if (it != players_.end()) {
                    PlayerCharacter* targetPlayer = it->second;
                    targetPlayer->SetIsPostionUpdated(true);
                    targetPlayer->SetLastRecentPosition(Math::Vector2(evt.locationX,evt.locationY));
                }    
            }
             
        }else if(socketEvent.type == S_PKT_BROADCASTING_ENTER)
        {
            //S_EnterPacket보다 BroadCastingPacket이 먼저 도착할 경우 실행하지 않음
            if(current_player_id != 0)
            {
                BroadcastingEnterEvent evt = socketEvent.broadcastingEnter;
                PlayerCharacter* player = World::Get()->SpawnActor<PlayerCharacter>(PlayerCharacter::StaticClass());
                player->SetPacketId(evt.userId);
                std::wstring playerName;
                playerName.assign(evt.name.begin(),evt.name.end());
                player->SetNickname(playerName);
                player->SetIsPostionUpdated(true);
                player->SetLastRecentPosition(Math::Vector2(0.0f, 0.0f));
                players_.insert({player->GetPacketId(), player});
            }
            
        }else if(socketEvent.type == S_PKT_ENTER_OTHER_USER)
        {
            EnterOtherUserEvent evt = socketEvent.enterOtherUser;
            for(int idx=0;idx<socketEvent.enterOtherUser.currentUserCnt_;idx++)
            {
                if(evt.userIdentifyidArr_[idx] != current_player_id)
                {
                    PlayerCharacter* player = World::Get()->SpawnActor<PlayerCharacter>(PlayerCharacter::StaticClass());
                    
                    player->SetPacketId(evt.userIdentifyidArr_[idx]);
                    std::wstring playerName;
                    playerName.assign(evt.nameArr_[idx].begin(),evt.nameArr_[idx].end());
                    player->SetNickname(playerName);
                    player->SetIsPostionUpdated(true);
                    player->SetLastRecentPosition(Math::Vector2(evt.locationXArr_[idx], evt.locationYArr_[idx]));
                    //targetPlayer->GetTransform()->SetPosition(Math::Vector2(evt.locationX, evt.locationY));
                    players_.insert({player->GetPacketId(), player});    
                }
                
            }
        }
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Network>("Network")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
