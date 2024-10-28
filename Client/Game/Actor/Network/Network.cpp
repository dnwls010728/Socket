#include "pch.h"
#include "Network.h"

#include "../SocketCore/SocketEventManager.h"
#include "Actor/Character/Player/PlayerCharacter.h"
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
            // TODO: 현재 캐릭터에 이름 장착
            // 현재 움직이고있는 내 액터를 어캐 찾아야 함?]
            // 트리오브세이비어마냥 룸이 한개임 ㅋㅋ
            // 그래서 하나의 룸에 들어가면
            // 모두가 그 사실을 알 수 있음
            // 얘가 '내가'들어왔을 때에 실행되는 코드
                    
            // PlayerCharacter* player = World::Get()->SpawnActor<PlayerCharacter>(PlayerCharacter::StaticClass());
                    
            // player->SetPacketId(socketEvent.enter.userId);
                    
        }else if(socketEvent.type == S_PKT_MOVING)
        {
            //이 방의 인원이 '움직였을 때'에 실행되는 코드
            //TODO: 해당하는 캐릭터의 좌표를 이동
        }else if(socketEvent.type == S_PKT_BROADCASTING_ENTER)
        {
            //누군가가 '들어왔을 때' 실행되는 코드
            // TODO: 해당하는 캐릭터를 생성
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
