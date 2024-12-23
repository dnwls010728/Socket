#pragma once
#include <map>

#include "User.h"

class User;



class Room : public JobQueue
{
public:
    static void Init();
    Room(uint32_t roomIdentifyKey,string roomName);
    bool Enter(const shared_ptr<User>& userRef);
    void Leave(const shared_ptr<User>& userRef);
    void Broadcast(const shared_ptr<SendBuffer>& sendBufferRef);
    
    uint32_t roomIdentifyKey;
    string roomName;
    
    map<int, shared_ptr<User>> users;
    
private:
    USE_LOCK;
};

extern map<int, shared_ptr<Room>>* GRoomMap;