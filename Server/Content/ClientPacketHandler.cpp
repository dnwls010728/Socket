#include "pch.h"
#include "ClientPacketHandler.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

void HandleInvalid(const shared_ptr<PacketSession>& session, BYTE* buf, int32_t len)
{
}

void HandleEnter(const shared_ptr<PacketSession>& session, C_EnterPacket& pkt)
{
}
