#pragma once
#include "User.h"

class MatchingQueue
{
public:
    int Push(const shared_ptr<User>& user);
private:
    void NoticeStartGame();
    
private:
    queue<shared_ptr<User>> _userQueue;
    USE_LOCK;
};

extern MatchingQueue* GMatchingQueue;