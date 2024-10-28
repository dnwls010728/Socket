#include "pch.h"
#include "ClientPacketHandler.h"
#include "User.h"
#include "Room.h"
#include <iostream>

#include "User.h"
PacketHandlerFunc GPacketHandler[UINT16_MAX];

void HandleInvalid(const shared_ptr<PacketSession>& session, BYTE* buf, int32_t len)
{
}

void HandleEnter(const shared_ptr<PacketSession>& session, C_EnterPacket& pkt)
{
    shared_ptr<GameSession> gameSession = static_pointer_cast<GameSession>(session);
    cout << "Enter EnterPacket" << endl;
    cout << pkt.GetName()<<endl;
    cout<< pkt.GetId() << endl;

    static atomic<uint32_t> idGenerator = 0;

    shared_ptr<User> user = make_shared<User>();
    user->userIdentifyId = ++idGenerator;
    user->ownerSession = gameSession;
    user->name = pkt.GetName();
    user->id = pkt.GetId();
    user->locationX = 0;
    user->locationY = 0;
    gameSession->userRef = user;
    gameSession->roomRef = GRoom;

    GRoom->Enter(user);
    
    S_EnterPacket sendPkt;
    sendPkt.SetSuccess(1);
    sendPkt._userId = user->userIdentifyId;
    sendPkt._name = user->name;
    auto sendBuffer = ClientPacketHandler::MakeSendBuffer<S_EnterPacket>(sendPkt,S_PKT_ENTER);
    session->Send(sendBuffer);

    S_BroadcastingEnterPacket broadcastPkt;
    broadcastPkt._success=1;
    broadcastPkt._userId=user->userIdentifyId;
    broadcastPkt._name = user->name;
    //입장을 브로드캐스팅
    auto broadcastSendBuffer = ClientPacketHandler::MakeSendBuffer<S_BroadcastingEnterPacket>(broadcastPkt,S_PKT_BROADCASTING_ENTER);
    GRoom->DoAsync(&Room::Broadcast, broadcastSendBuffer);
}
void HandleMoving(const shared_ptr<PacketSession>& session, C_MovingPacket& pkt)
{
    shared_ptr<GameSession> gameSession = static_pointer_cast<GameSession>(session);

    shared_ptr<User> user = static_pointer_cast<User>(gameSession->userRef);

    user->locationX = pkt._locationX;
    user->locationY = pkt._locationY;

    S_MovingPacket sendPkt;
    sendPkt._success = 1;
    sendPkt._userId = user->userIdentifyId;
    sendPkt._locationX = user->locationX;
    sendPkt._locationY = user->locationY;

    cout << "User IdentifiyId : " << user->userIdentifyId << endl;
    cout << "Location X : " << user->locationX << endl;
    cout << "Location Y : " << user->locationY << endl;
    cout << "User name :" << user->name << endl;
    auto sendBuffer = ClientPacketHandler::MakeSendBuffer<S_MovingPacket>(sendPkt,S_PKT_MOVING);
    //룸 전체에 브로드 캐스팅
    GRoom->DoAsync(&Room::Broadcast,sendBuffer);
    
}
