#pragma once
#include "Channel.h"
#include "Room.h"
#include "../../CommonDLL/SendBuffer.h"
#include "../../CommonDLL/Packet.h"
using PacketHandlerFunc = function<void(const shared_ptr<PacketSession>&, BYTE*, int32_t)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum PacketNumber : uint16_t
{
	C_PKT_ENTER = 1000,
	S_PKT_ENTER = 1001,
	C_PKT_MOVING=1002,
	S_PKT_MOVING=1003,
	S_PKT_BROADCASTING_ENTER=1004,
	S_PKT_ENTER_OTHER_USER=1005,
	C_PKT_ENTER_OTHER_USER=1006,
	S_PKT_LEAVE_OTHER_USER=1007,
	C_PKT_ENTER_ROOM=1008,
	S_PKT_ENTER_ROOM=1009,
	C_PKT_ENTER_CHANNEL=1010,
	S_PKT_ENTER_CHANNEL=1011,
	S_PKT_LEAVE_CHANNEL=1012,
};

void HandleInvalid(const shared_ptr<PacketSession>& session, BYTE* buf, int32_t len);
void HandleEnter(const shared_ptr<PacketSession>& session, shared_ptr<C_Enter> pkt);
void HandleMoving(const shared_ptr<PacketSession>& session, shared_ptr<C_Moving> pkt);
void HandleEnterOtherUser(const shared_ptr<PacketSession>& session, shared_ptr<C_EnterOtherUser> pkt);
void HandleEnterRoom(const shared_ptr<PacketSession>& session,shared_ptr<C_EnterRoom> pkt);
void HandleEnterChannel(const shared_ptr<PacketSession>& session, shared_ptr<C_EnterChannel> pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32_t i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = HandleInvalid;
		GPacketHandler[C_PKT_ENTER] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{return HandlePacket<C_Enter>(HandleEnter,session, buffer, len); };
		GPacketHandler[C_PKT_MOVING] = [](const shared_ptr<PacketSession>& session,BYTE* buffer, int32_t len)
		{return HandlePacket<C_Moving>(HandleMoving,session, buffer, len); };
		GPacketHandler[C_PKT_ENTER_OTHER_USER] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{ return HandlePacket<C_EnterOtherUser>(HandleEnterOtherUser,session, buffer, len); };
		GPacketHandler[C_PKT_ENTER_ROOM] = [](const shared_ptr<PacketSession>& session,BYTE* buffer,int32_t len)
		{return HandlePacket<C_EnterRoom>(HandleEnterRoom,session, buffer, len); };
		GPacketHandler[C_PKT_ENTER_CHANNEL] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{return HandlePacket<C_EnterChannel>(HandleEnterChannel,session, buffer, len); };
		
		Room::Init();
		Channel::Init();
	}

	static void HandlePacket(const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}

	

public:
	template<typename PacketType, typename ProcessFunc>
	static void HandlePacket(ProcessFunc func, const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len) 
	{
		std::shared_ptr<PacketType> pkt = std::make_shared<PacketType>();
		pkt->Deserialize(buffer + sizeof(PacketHeader),len-sizeof(PacketHeader));
		return func(session,std::move(pkt));
	}

	template<typename T>
	static shared_ptr<SendBuffer> MakeSendBuffer(T& pkt, uint16_t pktId)
	{
		const uint16_t dataSize = pkt.GetSize();
		const uint16_t packetSize = dataSize + sizeof(PacketHeader);
		shared_ptr<SendBuffer> sendBuffer = GSendBufferManager->Open(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		pkt.Serialize(reinterpret_cast<BYTE*>(&header[1]));
		header->id = pktId;
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};