#pragma once

#include "User.h"
#include "Room.h"
class GameSession final: public PacketSession
{
public:
	GameSession() = default;
	virtual ~GameSession() override
	{
		cout << "~GameSession \n";
	}
	GameSession(const GameSession& rhs) = delete;
	GameSession& operator=(const GameSession& rhs) = delete;
	GameSession(GameSession&& rhs) = delete;
	GameSession& operator=(GameSession&& rhs) = delete;
	

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int len) override;
	virtual void OnSend(int len) override;
public:
	shared_ptr<class User> userRef;
	
};