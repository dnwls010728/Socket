#pragma once
#include <map>

#include "User.h"
class User;

class Channel : public JobQueue
{
public:
    static void Init();
    void LeaveAndJoin(shared_ptr<User> userRef,const shared_ptr<Channel>& targetChannel);
    Channel(uint32_t channelIdentifyKey);
    bool Enter(shared_ptr<User> userRef);
    void Leave(shared_ptr<User> userRef);
    void Broadcast(shared_ptr<SendBuffer> sendBufferRef);
    
    uint32_t channelIdentifyKey;
    
    map<int, shared_ptr<User>> users;
    
private:
    USE_LOCK;
    
};
extern map<int,shared_ptr<Channel>>* GChannelMap;