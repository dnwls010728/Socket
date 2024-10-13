#include "pch.h"
#include "GameSession.h"


void GameSession::OnConnected()
{
	wcout << L"Connected : " << GetSessionRef()->GetAddress().GetIpAddress() << endl;
}

void GameSession::OnDisconnected()
{
	wcout << L"DisConnected" << GetSessionRef()->GetAddress().GetIpAddress() << endl;
}

void GameSession::OnRecvPacket(BYTE* buffer, int len)
{
	shared_ptr<PacketSession> session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	ClientPacketHandler::HandlePacket(session, buffer, len);

	//TODO: PacketId 대역 체크
	
}

void GameSession::OnSend(int len)
{
}
