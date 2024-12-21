#include "pch.h"
#include "Lobby.h"

Lobby* globalLobby = new Lobby();

void Lobby::Enter(const shared_ptr<User>& userRef)
{
    WRITE_LOCK;
    users[userRef->userIdentifyId] = userRef;
}

void Lobby::Leave(const shared_ptr<User>& userRef)
{
    WRITE_LOCK;
    users.erase(userRef->userIdentifyId);
}

void Lobby::Broadcast(const shared_ptr<SendBuffer>& sendBufferRef)
{
    for(auto& p : users)
    {
        
        p.second->ownerSession.lock()->Send(sendBufferRef);
    }
}
