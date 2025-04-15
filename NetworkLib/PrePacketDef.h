#pragma once
#include "NetDef.h"

#define NET_PACKET_ID_PING 5
#define NET_PACKET_ID_PONG 6
#define NET_PACKET_ID_ERROR 7

namespace Net {

	// 서버 > 클라이언트 연결 상태 확인
	struct PingPacket : public IPacket 
	{
		SERIALIZABLE_PACKET_FIELDS(Serializer)
		REGISTER_PACKET(PingPacket, NET_PACKET_ID_PING)
	};

	// 클라이언트 > 서버 응답
	struct PongPacket : public IPacket
	{
		SERIALIZABLE_PACKET_FIELDS(Serializer)
		REGISTER_PACKET(PongPacket, NET_PACKET_ID_PONG)
	};

	// 서버 > 클라 에러메시지
	struct ErrorPacket : public IPacket
	{
		uint16_t error_code;
		std::wstring error_message;
		SERIALIZABLE_PACKET_FIELDS(Serializer, error_code, error_message)
		REGISTER_PACKET(ErrorPacket, NET_PACKET_ID_ERROR)
	};

} // namespace Net
