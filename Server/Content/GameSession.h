#pragma once

#include "User.h"
#include "Room.h"
class GameSession : public PacketSession
{
public:
	~GameSession()
	{
		cout << "~GameSession" << endl;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int len) override;
	virtual void OnSend(int len) override;
public:
	shared_ptr<class User> userRef;
	weak_ptr<class Room> roomRef;
	weak_ptr<class Channel> channelRef;
	
};