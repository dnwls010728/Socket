#include "pch.h"
#include "GameSession.h"
#include "ClientPacketHandler.h"
#include "Room.h"

void GameSession::OnConnected()
{
	wcout << L"Connected : " << GetSessionRef()->GetAddress().GetIpAddress() << endl;
}

void GameSession::OnDisconnected()
{
	wcout << L"DisConnected" << GetSessionRef()->GetAddress().GetIpAddress() << endl;
	if(roomRef.lock().use_count() != 0)
	{
		roomRef.lock()->Leave(userRef);
	}
		
}

void GameSession::OnRecvPacket(BYTE* buffer, int len)
{
	shared_ptr<PacketSession> session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	ClientPacketHandler::HandlePacket(session, buffer, len);
	
}

void GameSession::OnSend(int len)
{
}
