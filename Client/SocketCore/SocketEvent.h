#pragma once
#include "ServerPacketHandler.h"
struct EnterEvent
{
    
    uint32_t userId;
    std::string name;
};

struct MovingEvent
{
    
    uint32_t userId;
    float locationX;
    float locationY;
};

struct BroadcastingEnterEvent
{
    
    uint32_t userId;
    std::string name;
};
struct EnterOtherUserEvent
{
    uint32_t* userIdentifyidArr_;
    std::string* nameArr_;
    float* locationXArr_;
    float* locationYArr_;
    uint32_t currentUserCnt_;
};

struct LeaveOtherUserEvent
{
    uint32_t userId;
};

struct EnterRoom
{
    uint32_t roomNum;
};

struct EnterChannel
{
    uint32_t currentChannelNum;
};

struct LeaveChannel
{
    uint32_t userId;
};

struct SocketEvent
{
    Type::uint32 type;
    EnterEvent enter;
    MovingEvent moving;
    BroadcastingEnterEvent broadcastingEnter;
    EnterOtherUserEvent enterOtherUser;
    LeaveOtherUserEvent leaveOtherUser;
    EnterRoom enterRoom;
    EnterChannel enterChannel;
    LeaveChannel leaveChannel;
};


