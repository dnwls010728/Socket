#include "pch.h"
#include "Packet.h"
#include "..\Util\DeSerializer.h";
C_EnterPacket::C_EnterPacket()
{
}

BYTE* C_EnterPacket::Serialize()
{
	return nullptr;
}

void C_EnterPacket::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte = 0;
	auto packet = DeSerializer::Deserialize<C_EnterPacket>(buffer, currentByte);
	_name = packet._name;
	_id = packet._id;
}

S_EnterPacket::S_EnterPacket()
{
}

BYTE* S_EnterPacket::Serialize()
{
	return nullptr;
}

void S_EnterPacket::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte = 0;
	auto packet = DeSerializer::Deserialize<S_EnterPacket>(buffer, currentByte);
	_success = packet._success;
}
