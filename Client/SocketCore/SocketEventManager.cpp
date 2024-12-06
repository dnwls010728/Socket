#include "pch.h"
#include "SocketEventManager.h"

SocketEventManager::SocketEventManager()
{
    
}

bool SocketEventManager::PollEvent(SocketEvent& socketEvent)
{
    if(eventQueue.empty())return false;

    socketEvent = eventQueue.front();
    eventQueue.pop();

    return true;
}

bool SocketEventManager::RegisterEvent(const std::shared_ptr<Packet>& pkt,uint16_t pktId)
{
    if(pktId == S_PKT_ENTER)
    {
        S_EnterPacket* enterPkt = (S_EnterPacket*)pkt.get();
        EnterEvent event{};
        event.userId = enterPkt->_userId;
        event.name = enterPkt->_name;
        SocketEvent socketEvent{};
        socketEvent.type = S_PKT_ENTER;
        socketEvent.enter = event;
        eventQueue.push(socketEvent);
        return true;
    }
    else if(pktId == S_PKT_MOVING)
    {
        S_MovingPacket* movingPkt = (S_MovingPacket*)pkt.get();
        MovingEvent event{};
        event.userId = movingPkt->_userId;
        event.locationX = movingPkt->_locationX;
        event.locationY = movingPkt->_locationY;

        SocketEvent socketEvent{};
        socketEvent.moving = event;
        socketEvent.type = S_PKT_MOVING;
        eventQueue.push(socketEvent);
        return true;
    }else if(pktId == S_PKT_BROADCASTING_ENTER)
    {
        S_BroadcastingEnterPacket* broadcastingEnterPkt = (S_BroadcastingEnterPacket*)pkt.get();
        BroadcastingEnterEvent event{};
        event.userId = broadcastingEnterPkt->_userId;
        event.name = broadcastingEnterPkt->_name;
        SocketEvent socketEvent{};
        socketEvent.broadcastingEnter=event;
        socketEvent.type = S_PKT_BROADCASTING_ENTER;
        eventQueue.push(socketEvent);
        return true;
    }else if(pktId == S_PKT_ENTER_OTHER_USER)
    {
        S_EnterOtherUserPacket* enterOtherUserPkt = (S_EnterOtherUserPacket*)pkt.get();
        EnterOtherUserEvent event{};
        event.currentUserCnt_ = enterOtherUserPkt->currentUserCnt_;
        event.nameArr_ =enterOtherUserPkt->nameArr_;
        event.userIdentifyidArr_ = enterOtherUserPkt->userIdentifyidArr_;
        event.locationXArr_ = enterOtherUserPkt->locationXArr_;
        event.locationYArr_ = enterOtherUserPkt->locationYArr_;
        SocketEvent socketEvent{};
        socketEvent.enterOtherUser = event;
        socketEvent.type = S_PKT_ENTER_OTHER_USER;
        eventQueue.push(socketEvent);
        return true;
    }else if(pktId == S_PKT_LEAVE_OTHER_USER)
    {
        S_LeaveOtherUserPacket* leaveOtherUserPkt = (S_LeaveOtherUserPacket*)pkt.get();
        LeaveOtherUserEvent event{};
        event.userId = leaveOtherUserPkt->_userId;
        SocketEvent socketEvent{};
        socketEvent.leaveOtherUser = event;
        socketEvent.type = S_PKT_LEAVE_OTHER_USER;
        eventQueue.push(socketEvent);
        return true;
    }else if(pktId == S_PKT_ENTER_ROOM)
    {
        S_EnterRoom* enterRoomPkt = (S_EnterRoom*)pkt.get();
        EnterRoom event{};
        event.roomNum=enterRoomPkt->_currentRoomNum;
        SocketEvent socketEvent{};
        socketEvent.enterRoom = event;
        socketEvent.type = S_PKT_ENTER_ROOM;
        eventQueue.push(socketEvent);
        return true;
    }else if(pktId == S_PKT_ENTER_CHANNEL)
    {
        S_EnterChannel* enterChannelPkt = (S_EnterChannel*)pkt.get();
        EnterChannel event{};
        event.currentChannelNum = enterChannelPkt->_currentChannelNum;
        SocketEvent socketEvent;
        socketEvent.enterChannel = event;
        socketEvent.type = S_PKT_ENTER_CHANNEL;
        eventQueue.push(socketEvent);
        return true;
    }else if(pktId == S_PKT_LEAVE_CHANNEL)
    {
        S_LeaveChannel* leaveChannelPkt = (S_LeaveChannel*)pkt.get();
        LeaveChannel event{};
        event.userId = leaveChannelPkt->_userId;
        SocketEvent socketEvent{};
        socketEvent.leaveChannel = event;
        socketEvent.type = S_PKT_LEAVE_CHANNEL;
        eventQueue.push(socketEvent);
        return true;
    }
    return false;
}

void SocketEventManager::Clear()
{
}
