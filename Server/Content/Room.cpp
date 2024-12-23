#include "pch.h"
#include "Room.h"

#include "ClientPacketHandler.h"

map<int,shared_ptr<Room>>* GRoomMap = new map<int,shared_ptr<Room>>();

void Room::Init()
{
    GRoomMap->insert(pair<int,shared_ptr<Room>>(0, make_shared<Room>(0,"firstRoom")));
    GRoomMap->insert(pair<int,shared_ptr<Room>>(1, make_shared<Room>(1,"secondRoom")));
}


Room::Room(uint32_t roomIdentifyKey, string roomName):roomIdentifyKey(roomIdentifyKey),roomName(roomName)
{
}

bool Room::Enter(const shared_ptr<User>& userRef)
{
    WRITE_LOCK;
    users[userRef->userIdentifyId] = userRef;
    return true;
}

void Room::Leave(const shared_ptr<User>& userRef)
{
    WRITE_LOCK;
    users.erase(userRef->userIdentifyId);
}

void Room::Broadcast(const shared_ptr<SendBuffer>& sendBufferRef)
{
    for(auto& p : users)
    {
        
        p.second->ownerSession.lock()->Send(sendBufferRef);
    }
}
