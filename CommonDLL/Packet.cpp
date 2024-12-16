#include "pch.h"
#include "Packet.h"
#include "DeSerializer.h"
#include "Serializer.h"


BYTE* C_Enter::Serialize(BYTE* buffer)
{
	int currentByte = 0;
	Serializer::Serialize(buffer, _name, currentByte);
	Serializer::Serialize(buffer, _id, currentByte);
	return buffer;
}

void C_Enter::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte = 0;
	
	DeSerializer::Deserialize(buffer, currentByte,_name);
	DeSerializer::Deserialize(buffer, currentByte,_id);
}

uint16_t C_Enter::GetSize()
{
	uint16_t ret = 0;
	ret += Serializer::GetPacketSize(_name);
	ret += Serializer::GetPacketSize(_id);
	return ret;
}



BYTE* S_Enter::Serialize(BYTE* buffer)
{
	
	int currentByte = 0;
	Serializer::Serialize(buffer, _success, currentByte);
	Serializer::Serialize(buffer,_userId, currentByte);
	Serializer::Serialize(buffer,_name, currentByte);
	return buffer;
}

void S_Enter::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte = 0;
	DeSerializer::Deserialize(buffer, currentByte,_success);
	DeSerializer::Deserialize(buffer, currentByte,_userId);
	DeSerializer::Deserialize(buffer, currentByte,_name);
}

uint16_t S_Enter::GetSize()
{
	return Serializer::GetPacketSize(_success)+
		Serializer::GetPacketSize(_userId)+
			Serializer::GetPacketSize(_name);
}



BYTE* C_Moving::Serialize(BYTE* buffer)
{
	int currentByte = 0;
	Serializer::Serialize(buffer,_locationX,currentByte);
	Serializer::Serialize(buffer,_locationY,currentByte);
	return buffer;
}

void C_Moving::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte = 0;
	DeSerializer::Deserialize(buffer, currentByte,_locationX);
	DeSerializer::Deserialize(buffer, currentByte,_locationY);
}

uint16_t C_Moving::GetSize()
{
	return Serializer::GetPacketSize(_locationX)+Serializer::GetPacketSize(_locationY);
}



BYTE* S_Moving::Serialize(BYTE* buffer)
{
	int currentByte = 0;
	Serializer::Serialize(buffer,_success,currentByte);
	Serializer::Serialize(buffer,_userId,currentByte);
	Serializer::Serialize(buffer,_locationX,currentByte);
	Serializer::Serialize(buffer,_locationY,currentByte);
	return buffer;
}

void S_Moving::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte = 0;
	DeSerializer::Deserialize(buffer, currentByte,_success);
	DeSerializer::Deserialize(buffer, currentByte,_userId);
	DeSerializer::Deserialize(buffer, currentByte,_locationX);
	DeSerializer::Deserialize(buffer, currentByte,_locationY);
}

uint16_t S_Moving::GetSize()
{
	return Serializer::GetPacketSize(_success)+
		Serializer::GetPacketSize(_userId)+
			Serializer::GetPacketSize(_locationX)+
				Serializer::GetPacketSize(_locationY);
}



BYTE* S_BroadcastingEnter::Serialize(BYTE* buffer)
{
	int currentByte=0;
	Serializer::Serialize(buffer,_success,currentByte);
	Serializer::Serialize(buffer,_userId,currentByte);
	Serializer::Serialize(buffer,_name,currentByte);
	return buffer;
}

void S_BroadcastingEnter::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte=0;
	DeSerializer::Deserialize(buffer, currentByte,_success);
	DeSerializer::Deserialize(buffer, currentByte,_userId);
	DeSerializer::Deserialize(buffer, currentByte,_name);
}

uint16_t S_BroadcastingEnter::GetSize()
{
	return Serializer::GetPacketSize(_success)+
		Serializer::GetPacketSize(_userId)+
			Serializer::GetPacketSize(_name);
}


BYTE* S_EnterOtherUser::Serialize(BYTE* buffer)
{
	int currentByte = 0;
	Serializer::Serialize(buffer,currentUserCnt_,currentByte);
	Serializer::Serialize(buffer,userIdentifyidArr_,currentUserCnt_,currentByte);
	Serializer::Serialize(buffer,nameArr_,currentUserCnt_,currentByte);
	Serializer::Serialize(buffer,locationXArr_,currentUserCnt_,currentByte);
	Serializer::Serialize(buffer,locationYArr_,currentUserCnt_,currentByte);
	return buffer;
	
}

void S_EnterOtherUser::Deserialize(BYTE* buffer, int32_t len)
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

uint16_t S_EnterOtherUser::GetSize()
{
	return Serializer::GetPacketSize(userIdentifyidArr_,currentUserCnt_)
	+ Serializer::GetPacketSize(nameArr_,currentUserCnt_)
	+ Serializer::GetPacketSize(locationXArr_,currentUserCnt_)
	+ Serializer::GetPacketSize(locationYArr_,currentUserCnt_)
	+ Serializer::GetPacketSize(currentUserCnt_);
	
}



BYTE* C_EnterOtherUser::Serialize(BYTE* buffer)
{
	int currentByte=0;
	Serializer::Serialize(buffer,_userId,currentByte);
	return buffer;
}

void C_EnterOtherUser::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte = 0;
	DeSerializer::Deserialize(buffer,currentByte,_userId);
}

uint16_t C_EnterOtherUser::GetSize()
{
	return Serializer::GetPacketSize(_userId);
}



BYTE* S_LeaveOtherUser::Serialize(BYTE* buffer)
{
	int currentByte = 0;
	Serializer::Serialize(buffer,_userId,currentByte);
	return buffer;
}

void S_LeaveOtherUser::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte = 0;
	DeSerializer::Deserialize(buffer,currentByte,_userId);
}

uint16_t S_LeaveOtherUser::GetSize()
{
	return Serializer::GetPacketSize(_userId);
}



BYTE* C_EnterRoom::Serialize(BYTE* buffer)
{
	int currentByte = 0;
	Serializer::Serialize(buffer,_roomNum,currentByte);
	return buffer;
}

void C_EnterRoom::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte = 0;
	DeSerializer::Deserialize(buffer,currentByte,_roomNum);
}

uint16_t C_EnterRoom::GetSize()
{
	return Serializer::GetPacketSize(_roomNum);
}



BYTE* S_EnterRoom::Serialize(BYTE* buffer)
{
	int currentByte = 0;
	Serializer::Serialize(buffer,_currentRoomNum,currentByte);
	return buffer;
}

void S_EnterRoom::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte = 0;
	DeSerializer::Deserialize(buffer,currentByte,_currentRoomNum);
}

uint16_t S_EnterRoom::GetSize()
{
	return Serializer::GetPacketSize(_currentRoomNum);
}



BYTE* C_EnterChannel::Serialize(BYTE* buffer)
{
	int currentByte=0;
	Serializer::Serialize(buffer,_channelNum,currentByte);
	return buffer;
}

void C_EnterChannel::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte=0;
	DeSerializer::Deserialize(buffer,currentByte,_channelNum);
}

uint16_t C_EnterChannel::GetSize()
{
	return Serializer::GetPacketSize(_channelNum);
}



BYTE* S_EnterChannel::Serialize(BYTE* buffer)
{
	int currentByte=0;
	Serializer::Serialize(buffer,_currentChannelNum,currentByte);
	return buffer;
}

void S_EnterChannel::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte=0;
	DeSerializer::Deserialize(buffer,currentByte,_currentChannelNum);
	
}

uint16_t S_EnterChannel::GetSize()
{
	return Serializer::GetPacketSize(_currentChannelNum);
}



BYTE* S_LeaveChannel::Serialize(BYTE* buffer)
{
	int currentByte=0;
	Serializer::Serialize(buffer,_userId,currentByte);
	return buffer;
}

void S_LeaveChannel::Deserialize(BYTE* buffer, int32_t len)
{
	int currentByte=0;
	DeSerializer::Deserialize(buffer,currentByte,_userId);
}

uint16_t S_LeaveChannel::GetSize()
{
	return Serializer::GetPacketSize(_userId);
}
