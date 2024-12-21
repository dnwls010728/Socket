#pragma once
#include <map>

#include "User.h"

class Lobby : public JobQueue
{
public:
    Lobby() = default;
    ~Lobby()
    {
        cout <<"Lobby::~Lobby()"<<endl;
    }
    Lobby(const Lobby&) = default;
    Lobby& operator=(const Lobby&) = default;
    Lobby(Lobby&&) = default;
    Lobby& operator=(Lobby&&) = default;

    void Enter(const shared_ptr<User>& userRef);
    void Leave(const shared_ptr<User>& userRef);
    void Broadcast(const shared_ptr<SendBuffer>& sendBufferRef);
    
    std::map<int,shared_ptr<User>> users;
private:
    USE_LOCK;
};

extern Lobby* globalLobby;