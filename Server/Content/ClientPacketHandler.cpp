#include "pch.h"
#include "ClientPacketHandler.h"
#include "User.h"
#include "Room.h"
#include <iostream>

PacketHandlerFunc GPacketHandler[UINT16_MAX];

void HandleInvalid(const shared_ptr<PacketSession>& session, BYTE* buf, int32_t len)
{
}

void HandleEnter(const shared_ptr<PacketSession>& session, shared_ptr<C_Enter> pkt)
{
    shared_ptr<GameSession> gameSession = static_pointer_cast<GameSession>(session);
    cout << "Enter EnterPacket" << endl;
    cout << pkt->_name<<endl;
    cout<< pkt->_id << endl;

    static atomic<uint32_t> idGenerator = 0;

    shared_ptr<User> user = make_shared<User>();
    user->userIdentifyId = ++idGenerator;
    user->ownerSession = gameSession;
    user->name = pkt->_name;
    user->id = pkt->_id;
    user->locationX = 0;
    user->locationY = 0;
    gameSession->userRef = user;
    
    
    S_Enter sendPkt;
    sendPkt._success = 1;
    sendPkt._userId = user->userIdentifyId;
    sendPkt._name = user->name;
    auto sendBuffer = ClientPacketHandler::MakeSendBuffer<S_Enter>(sendPkt,S_PKT_ENTER);
    session->Send(sendBuffer);

    
    


    
}
void HandleMoving(const shared_ptr<PacketSession>& session, shared_ptr<C_Moving> pkt)
{
    shared_ptr<GameSession> gameSession = static_pointer_cast<GameSession>(session);
    
    shared_ptr<User> user = static_pointer_cast<User>(gameSession->userRef);

    user->locationX = pkt->_locationX;
    user->locationY = pkt->_locationY;

    S_Moving sendPkt;
    sendPkt._success = 1;
    sendPkt._userId = user->userIdentifyId;
    sendPkt._locationX = user->locationX;
    sendPkt._locationY = user->locationY;
    
    auto sendBuffer = ClientPacketHandler::MakeSendBuffer<S_Moving>(sendPkt,S_PKT_MOVING);
    //룸 전체에 브로드 캐스팅
    gameSession->roomRef.lock()->DoAsync(&Room::Broadcast,sendBuffer);
    
}

void HandleEnterOtherUser(const shared_ptr<PacketSession>& session, shared_ptr<C_EnterOtherUser> pkt)
{
    // Handle Enter는 처음으로 들어온 유저만이 호출할 수 있는 패킷
    // 따라서 room의 현재 패킷에 들어있는 데이터들을 동기화 할 필요가 있음.
    shared_ptr<GameSession> gameSession = static_pointer_cast<GameSession>(session);

    if(gameSession->roomRef.lock().use_count() != 0 && gameSession->roomRef.lock()->users.size() != 0)
    {
        auto userMap = gameSession->roomRef.lock()->users;
        S_EnterOtherUser sendOtherPkt;
        size_t arrSize = userMap.size();
        uint32_t* userIdxArr = new uint32_t[arrSize];
        string* userNameArr = new string[arrSize];
        float* locationXArr = new float[arrSize];
        float* locationYArr = new float[arrSize];
        int userCnt = 0;
        //배열에 담기
        for(auto it = userMap.begin(); it != userMap.end(); ++it)
        {
            userIdxArr[userCnt] = it->second->userIdentifyId;
            userNameArr[userCnt] = it->second->name;
            locationXArr[userCnt] = it->second->locationX;
            locationYArr[userCnt] = it->second->locationY;
            ++userCnt;
        }

        //패킷에 지정
        sendOtherPkt.userIdentifyidArr_=userIdxArr;
        sendOtherPkt.nameArr_ = userNameArr;
        sendOtherPkt.locationXArr_ = locationXArr;
        sendOtherPkt.locationYArr_ = locationYArr;
        sendOtherPkt.currentUserCnt_ = userMap.size();
        auto sendOtherPktBuffer = ClientPacketHandler::MakeSendBuffer<S_EnterOtherUser>(sendOtherPkt,S_PKT_ENTER_OTHER_USER);


        delete[] userIdxArr;
        delete[] locationXArr;
        delete[] locationYArr;
        delete[] userNameArr;
        
        session->Send(sendOtherPktBuffer);
        
    }
}

void HandleEnterRoom(const shared_ptr<PacketSession>& session, shared_ptr<C_EnterRoom> pkt)
{
    cout << "Room Entered" << endl;
    shared_ptr<GameSession> gameSession = static_pointer_cast<GameSession>(session);

    if(GRoomMap->find(pkt->_roomNum) != GRoomMap->end())
    {

        auto newRoom = GRoomMap->find(pkt->_roomNum)->second;
        auto user = gameSession->userRef;
        if(gameSession->roomRef.use_count() == 0)
        {
            
            newRoom->Enter(user);
            gameSession->roomRef = newRoom;
            
        }
        else
        {
            
            auto currentRoom = gameSession->roomRef;

            currentRoom.lock()->LeaveAndJoin(user, newRoom);
            gameSession->roomRef = newRoom;

            
        }

        S_EnterRoom enterRoomPkt;
        enterRoomPkt._currentRoomNum = newRoom->roomIdentifyKey;
        auto enterRoomSendBuffer = ClientPacketHandler::MakeSendBuffer<S_EnterRoom>(enterRoomPkt,S_PKT_ENTER_ROOM);
        session->Send(enterRoomSendBuffer);
        
        S_BroadcastingEnter broadcastPkt;
        broadcastPkt._success=1;
        broadcastPkt._userId=user->userIdentifyId;
        broadcastPkt._name = user->name;
        auto broadcastSendBuffer = ClientPacketHandler::MakeSendBuffer<S_BroadcastingEnter>(broadcastPkt,S_PKT_BROADCASTING_ENTER);
        newRoom->DoAsync(&Room::Broadcast,broadcastSendBuffer);
        
        
        
    
    }
}

void HandleEnterChannel(const shared_ptr<PacketSession>& session, shared_ptr<C_EnterChannel> pkt)
{
    cout << "Channel Entered" << endl;
    shared_ptr<GameSession> gameSession = static_pointer_cast<GameSession>(session);

    if(GChannelMap->find(pkt->_channelNum) != GChannelMap->end())
    {

        auto newChannel = GChannelMap->find(pkt->_channelNum)->second;
        auto user = gameSession->userRef;
        if(gameSession->channelRef.use_count() == 0)
        {
            
            newChannel->Enter(user);
            gameSession->channelRef = newChannel;
            
        }
        else
        {
            
            auto currentChannel = gameSession->channelRef;

            currentChannel.lock()->LeaveAndJoin(user, newChannel);
            gameSession->channelRef = newChannel;

            
        }

        S_EnterChannel enterChannelPkt;
        enterChannelPkt._currentChannelNum = newChannel->channelIdentifyKey;
        auto enterChannelSendBuffer = ClientPacketHandler::MakeSendBuffer<S_EnterChannel>(enterChannelPkt,S_PKT_ENTER_CHANNEL);
        session->Send(enterChannelSendBuffer);
        
        
        
    
    }
}

void HandleActorAttack(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorAttack> pkt)
{
}

void HandleActorMove(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorMove> pkt)
{
}

void HandleActorAppear(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorAppear> pkt)
{
}

void HandleActorDisappear(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorDisappear> pkt)
{
}

void HandleGetItems(const shared_ptr<PacketSession>& session, shared_ptr<C_GetItems> pkt)
{
}

void HandleUseItem(const shared_ptr<PacketSession>& session, shared_ptr<C_UseItem> pkt)
{
}

void HandleEnterToLobby(const shared_ptr<PacketSession>& session, shared_ptr<C_EnterToLobby> pkt)
{
}

void HandleEnterToMatching(const shared_ptr<PacketSession>& session, shared_ptr<C_EnterToMatching> pkt)
{
}

void HandleMatchingCancel(const shared_ptr<PacketSession>& session, shared_ptr<C_MatchingCancel> pkt)
{
}

void HandleJoinGame(const shared_ptr<PacketSession>& session, shared_ptr<C_JoinGame> pkt)
{
}

void HandleSignIn(const shared_ptr<PacketSession>& session, shared_ptr<C_SignIn> pkt)
{
}

void HandleLogin(const shared_ptr<PacketSession>& session, shared_ptr<C_Login> pkt)
{
}

void HandleGetMyAwardInfo(const shared_ptr<PacketSession>& session, shared_ptr<C_GetMyAwardInfo> pkt)
{
}

void HandleSelectCharacter(const shared_ptr<PacketSession>& session, shared_ptr<C_SelectCharacter> pkt)
{
}

void HandleSelectWeapon(const shared_ptr<PacketSession>& session, shared_ptr<C_SelectWeapon> pkt)
{
}

void HandleBuyWeapon(const shared_ptr<PacketSession>& session, shared_ptr<C_BuyWeapon> pkt)
{
}

void HandleCacheFulling(const shared_ptr<PacketSession>& session, shared_ptr<C_CacheFulling> pkt)
{
}
