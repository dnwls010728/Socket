#include "pch.h"
#include "Room.h"

#include "ClientPacketHandler.h"

map<int,shared_ptr<Room>>* GRoomMap = new map<int,shared_ptr<Room>>();


void Room::Init()
{
    GRoomMap->insert(pair<int,shared_ptr<Room>>(0, make_shared<Room>(0,"firstRoom")));
    GRoomMap->insert(pair<int,shared_ptr<Room>>(1, make_shared<Room>(1,"secondRoom")));
}

void Room::LeaveAndJoin(shared_ptr<User> userRef,const shared_ptr<Room>& targetRoom)
{
    WRITE_LOCK;
    Leave(std::move(userRef));
    targetRoom->Enter(std::move(userRef));
}

Room::Room(uint32_t roomIdentifyKey, string roomName):roomIdentifyKey(roomIdentifyKey),roomName(roomName)
{
}

bool Room::Enter(shared_ptr<User> userRef)
{
    WRITE_LOCK;
    users[userRef->userIdentifyId] = userRef;
    return true;
}

void Room::Leave(shared_ptr<User> userRef)
{
    WRITE_LOCK;
    users.erase(userRef->userIdentifyId);
}

void Room::Broadcast(shared_ptr<SendBuffer> sendBufferRef)
{
    for(auto& p : users)
    {
        
        p.second->ownerSession.lock()->Send(sendBufferRef);
    }
}
