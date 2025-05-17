#pragma once
#include "NetDef.h"

#define NET_PACKET_ID_PING_REQUEST 4
#define NET_PACKET_ID_PING 5
#define NET_PACKET_ID_PONG 6
#define NET_PACKET_ID_ERROR 7

namespace Net {

	// 클라이언트 > 서버 핑 요청
	struct PingRequestPacket : public IPacket 
	{
		float client_time;
		SERIALIZABLE_FIELDS(client_time)
		REGISTER_PACKET(PingRequestPacket, NET_PACKET_ID_PING_REQUEST)
	};
	
	// 서버 > 클라이언트 연결 상태 확인
	struct PingPacket : public IPacket 
	{
		float client_time;
		float server_time;
		SERIALIZABLE_FIELDS(client_time, server_time)
		REGISTER_PACKET(PingPacket, NET_PACKET_ID_PING)
	};

	// 클라이언트 > 서버 응답
	struct PongPacket : public IPacket
	{
		SERIALIZABLE_FIELDS()
		REGISTER_PACKET(PongPacket, NET_PACKET_ID_PONG)
	};

	// 서버 > 클라 에러메시지
	struct ErrorPacket : public IPacket
	{
		uint16_t error_code;
		std::wstring error_message;
		SERIALIZABLE_FIELDS(error_code, error_message)
		REGISTER_PACKET(ErrorPacket, NET_PACKET_ID_ERROR)
	};

} // namespace Net
