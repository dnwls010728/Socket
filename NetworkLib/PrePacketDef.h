#pragma once
#include "NetDef.h"

#define NET_PACKET_ID_PING 5
#define NET_PACKET_ID_PONG 6

namespace Net {

	// 서버 > 클라이언트 연결 상태 확인
	struct PingPacket : public IPacket 
	{
		SERIALIZABLE_FIELDS()
		REGISTER_PACKET(PingPacket, NET_PACKET_ID_PING)
	};

	// 클라이언트 > 서버 응답
	struct PongPacket : public IPacket
	{
		SERIALIZABLE_FIELDS()
		REGISTER_PACKET(PongPacket, NET_PACKET_ID_PONG)
	};

} // namespace Net
