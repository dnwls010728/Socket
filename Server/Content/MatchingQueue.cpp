#include "pch.h"
#include "MatchingQueue.h"

#include "ClientPacketHandler.h"
#include "../../CommonDLL/LobbyChannelPacket.h"

MatchingQueue* GMatchingQueue = new MatchingQueue();

int MatchingQueue::Push(const shared_ptr<User>& user)
{
    WRITE_LOCK;
    _userQueue.push(std::move(user));
    if (_userQueue.size()==4)
    {
        NoticeStartGame();
    }

    return _userQueue.size()-1;
}

void MatchingQueue::NoticeStartGame()
{
    vector<shared_ptr<User>> users;
    while (_userQueue.size()>0)
    {
        auto user = _userQueue.front();
        _userQueue.pop();
        users.emplace_back(user);
    }

    
    int cnt=0;
    for (auto user : users)
    {
        S_MatchingFinished pkt;
        pkt._userId = user->userIdentifyId;
        pkt._matchingQueueId=0;
        pkt._matchingQueueTurn = cnt++;

        auto sendBuffer = ClientPacketHandler::MakeSendBuffer<S_MatchingFinished>(pkt,S_PKT_MATCHING_FINISHED);
        user->ownerSession.lock()->Send(sendBuffer);
    }
}
