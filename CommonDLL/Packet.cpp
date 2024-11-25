#include "pch.h"
#include "Packet.h"
#include "DeSerializer.h"
#include "Serializer.h"

C_EnterPacket::C_EnterPacket()
{
}

BYTE* C_EnterPacket::Serialize(BYTE* buffer)
{
	int currentByte = 0;
	Serializer::Serialize(buffer, _name, currentByte);
	Serializer::Serialize(buffer, _id, currentByte);
	return buffer;
}

void C_EnterPacket::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte = 0;
	
	DeSerializer::Deserialize(buffer, currentByte,_name);
	DeSerializer::Deserialize(buffer, currentByte,_id);
}

uint16_t C_EnterPacket::GetSize()
{
	uint16_t ret = 0;
	ret += Serializer::GetPacketSize(_name);
	ret += Serializer::GetPacketSize(_id);
	return ret;
}

S_EnterPacket::S_EnterPacket()
{
	
}

BYTE* S_EnterPacket::Serialize(BYTE* buffer)
{
	
	int currentByte = 0;
	Serializer::Serialize(buffer, _success, currentByte);
	Serializer::Serialize(buffer,_userId, currentByte);
	Serializer::Serialize(buffer,_name, currentByte);
	return buffer;
}

void S_EnterPacket::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte = 0;
	DeSerializer::Deserialize(buffer, currentByte,_success);
	DeSerializer::Deserialize(buffer, currentByte,_userId);
	DeSerializer::Deserialize(buffer, currentByte,_name);
}

uint16_t S_EnterPacket::GetSize()
{
	return Serializer::GetPacketSize(_success)+
		Serializer::GetPacketSize(_userId)+
			Serializer::GetPacketSize(_name);
}

C_MovingPacket::C_MovingPacket()
{
}

BYTE* C_MovingPacket::Serialize(BYTE* buffer)
{
	int currentByte = 0;
	Serializer::Serialize(buffer,_locationX,currentByte);
	Serializer::Serialize(buffer,_locationY,currentByte);
	return buffer;
}

void C_MovingPacket::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte = 0;
	DeSerializer::Deserialize(buffer, currentByte,_locationX);
	DeSerializer::Deserialize(buffer, currentByte,_locationY);
}

uint16_t C_MovingPacket::GetSize()
{
	return Serializer::GetPacketSize(_locationX)+Serializer::GetPacketSize(_locationY);
}

S_MovingPacket::S_MovingPacket()
{
}

BYTE* S_MovingPacket::Serialize(BYTE* buffer)
{
	int currentByte = 0;
	Serializer::Serialize(buffer,_success,currentByte);
	Serializer::Serialize(buffer,_userId,currentByte);
	Serializer::Serialize(buffer,_locationX,currentByte);
	Serializer::Serialize(buffer,_locationY,currentByte);
	return buffer;
}

void S_MovingPacket::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte = 0;
	DeSerializer::Deserialize(buffer, currentByte,_success);
	DeSerializer::Deserialize(buffer, currentByte,_userId);
	DeSerializer::Deserialize(buffer, currentByte,_locationX);
	DeSerializer::Deserialize(buffer, currentByte,_locationY);
}

uint16_t S_MovingPacket::GetSize()
{
	return Serializer::GetPacketSize(_success)+
		Serializer::GetPacketSize(_userId)+
			Serializer::GetPacketSize(_locationX)+
				Serializer::GetPacketSize(_locationY);
}

S_BroadcastingEnterPacket::S_BroadcastingEnterPacket()
{
}

BYTE* S_BroadcastingEnterPacket::Serialize(BYTE* buffer)
{
	int currentByte=0;
	Serializer::Serialize(buffer,_success,currentByte);
	Serializer::Serialize(buffer,_userId,currentByte);
	Serializer::Serialize(buffer,_name,currentByte);
	return buffer;
}

void S_BroadcastingEnterPacket::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte=0;
	DeSerializer::Deserialize(buffer, currentByte,_success);
	DeSerializer::Deserialize(buffer, currentByte,_userId);
	DeSerializer::Deserialize(buffer, currentByte,_name);
}

uint16_t S_BroadcastingEnterPacket::GetSize()
{
	return Serializer::GetPacketSize(_success)+
		Serializer::GetPacketSize(_userId)+
			Serializer::GetPacketSize(_name);
}

S_EnterOtherUserPacket::S_EnterOtherUserPacket()
{
}

BYTE* S_EnterOtherUserPacket::Serialize(BYTE* buffer)
{
	int currentByte = 0;
	Serializer::Serialize(buffer,currentUserCnt_,currentByte);
	Serializer::Serialize(buffer,userIdentifyidArr_,currentUserCnt_,currentByte);
	Serializer::Serialize(buffer,nameArr_,currentUserCnt_,currentByte);
	Serializer::Serialize(buffer,locationXArr_,currentUserCnt_,currentByte);
	Serializer::Serialize(buffer,locationYArr_,currentUserCnt_,currentByte);
	return buffer;
	
}

void S_EnterOtherUserPacket::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte = 0;
	DeSerializer::Deserialize(buffer, currentByte,currentUserCnt_);

	userIdentifyidArr_ = new uint32_t[currentUserCnt_];
	nameArr_ = new std::string[currentUserCnt_];
	locationXArr_ = new float[currentUserCnt_];
	locationYArr_ = new float[currentUserCnt_];
	
	DeSerializer::Deserialize(buffer,currentByte,userIdentifyidArr_);
	DeSerializer::Deserialize(buffer,currentByte,nameArr_);
	DeSerializer::Deserialize(buffer,currentByte,locationXArr_);
	DeSerializer::Deserialize(buffer,currentByte,locationYArr_);
	
}

uint16_t S_EnterOtherUserPacket::GetSize()
{
	return Serializer::GetPacketSize(userIdentifyidArr_,currentUserCnt_)
	+ Serializer::GetPacketSize(nameArr_,currentUserCnt_)
	+ Serializer::GetPacketSize(locationXArr_,currentUserCnt_)
	+ Serializer::GetPacketSize(locationYArr_,currentUserCnt_)
	+ Serializer::GetPacketSize(currentUserCnt_);
	
}

C_EnterOtherUserPacket::C_EnterOtherUserPacket()
{
}

BYTE* C_EnterOtherUserPacket::Serialize(BYTE* buffer)
{
	int currentByte=0;
	Serializer::Serialize(buffer,_userId,currentByte);
	return buffer;
}

void C_EnterOtherUserPacket::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte = 0;
	DeSerializer::Deserialize(buffer,currentByte,_userId);
}

uint16_t C_EnterOtherUserPacket::GetSize()
{
	return Serializer::GetPacketSize(_userId);
}


