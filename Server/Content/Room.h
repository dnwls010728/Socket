#pragma once
#include <map>

#include "User.h"

class User;

class Room : public JobQueue
{
public:
    bool Enter(shared_ptr<User> userRef);
    void Leave(shared_ptr<User> userRef);
    void Broadcast(shared_ptr<SendBuffer> sendBufferRef);

public:
    map<int, shared_ptr<User>> users;
    
private:
    USE_LOCK;
};

extern shared_ptr<Room> GRoom;