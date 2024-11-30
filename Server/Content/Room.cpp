#include "pch.h"
#include "Room.h"

#include "ClientPacketHandler.h"

shared_ptr<Room> GRoom = make_shared<Room>();

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

    S_LeaveOtherUserPacket pkt;
    pkt._userId = userRef->userIdentifyId;

    auto sendBuffer = ClientPacketHandler::MakeSendBuffer<S_LeaveOtherUserPacket>(pkt,S_PKT_LEAVE_OTHER_USER);
    Broadcast(sendBuffer);
}

void Room::Broadcast(shared_ptr<SendBuffer> sendBufferRef)
{
    for(auto& p : users)
    {
        p.second->ownerSession.lock()->Send(sendBufferRef);
    }
}
