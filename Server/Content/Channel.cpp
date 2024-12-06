#include "pch.h"
#include "Channel.h"

#include "ClientPacketHandler.h"

map<int,shared_ptr<Channel>>* GChannelMap = new map<int,shared_ptr<Channel>>();

void Channel::Init()
{
    GChannelMap->insert(pair<int,shared_ptr<Channel>>(0,make_shared<Channel>(0)));
    GChannelMap->insert(pair<int,shared_ptr<Channel>>(1,make_shared<Channel>(1)));
}

void Channel::LeaveAndJoin(shared_ptr<User> userRef, const shared_ptr<Channel>& targetChannel)
{
    WRITE_LOCK;
    Leave(std::move(userRef));
    targetChannel->Enter(std::move(userRef));
}

Channel::Channel(uint32_t channelIdentifyKey):channelIdentifyKey(channelIdentifyKey)
{
    
}

bool Channel::Enter(shared_ptr<User> userRef)
{
    WRITE_LOCK;
    users[userRef->userIdentifyId] = userRef;
    return true;
}

void Channel::Leave(shared_ptr<User> userRef)
{
    WRITE_LOCK;
    users.erase(userRef->userIdentifyId);

    S_LeaveChannel pkt;
    pkt._userId = userRef->userIdentifyId;

    auto sendBuffer = ClientPacketHandler::MakeSendBuffer<S_LeaveChannel>(pkt,S_PKT_LEAVE_CHANNEL);
    Broadcast(sendBuffer);
}

void Channel::Broadcast(shared_ptr<SendBuffer> sendBufferRef)
{
    for(auto& p: users)
    {
        p.second->ownerSession.lock()->Send(sendBufferRef);
    }
}
