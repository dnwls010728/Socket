#pragma once

#include <memory>
#include "../../CommonDLL/Packet.h"
#include "SocketSession.h"

using PacketHandlerFunc = std::function<void(BYTE*, int32_t)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];
enum PacketNumber : uint16_t
{
    C_PKT_ENTER = 1000,
    S_PKT_ENTER = 1001,
    C_PKT_MOVING=1002,
    S_PKT_MOVING=1003,
    S_PKT_BROADCASTING_ENTER=1004,
    S_PKT_ENTER_OTHER_USER=1005,
    C_PKT_ENTER_OTHER_USER=1006
    
};

void HandleInvalid(BYTE* buf, int32_t len);
void HandleEnter(std::shared_ptr<S_EnterPacket> pkt);
void HandleMoving(std::shared_ptr<S_MovingPacket> pkt);
void HandleBroadcastEnter(std::shared_ptr<S_BroadcastingEnterPacket> pkt);
void HandleEnterOtherUser(std::shared_ptr<S_EnterOtherUserPacket> pkt);

class ServerPacketHandler
{
public:
    static void Init()
    {
        for(int32_t i=0; i<UINT16_MAX; i++)
            GPacketHandler[i] = HandleInvalid;
        GPacketHandler[S_PKT_ENTER] = [](BYTE* buffer,int32_t len)
        {return HandlePacket<S_EnterPacket>(HandleEnter,buffer,len);};
        GPacketHandler[S_PKT_MOVING] = [](BYTE* buffer,int32_t len)
        {return HandlePacket<S_MovingPacket>(HandleMoving,buffer,len);};
        GPacketHandler[S_PKT_BROADCASTING_ENTER] = [](BYTE* buffer,int32_t len)
        {return HandlePacket<S_BroadcastingEnterPacket>(HandleBroadcastEnter,buffer,len);};
        GPacketHandler[S_PKT_ENTER_OTHER_USER] = [](BYTE* buffer,int32_t len)
        {return HandlePacket<S_EnterOtherUserPacket>(HandleEnterOtherUser,buffer,len);};
        
    }
    static void HandlePacket(BYTE* buffer, int32_t len)
    {
        PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
        return GPacketHandler[header->id](buffer, len);
    }

public:
    template<typename PacketType, typename ProcessFunc>
    static void HandlePacket(ProcessFunc func,BYTE* buffer,int32_t len)
    {
        std::shared_ptr<PacketType> pkt = std::make_shared<PacketType>();
        
        pkt->Deserialize(buffer + sizeof(PacketHeader),len-sizeof(PacketHeader));
        return func(std::move(pkt));
    }

    template<typename T>
    static std::shared_ptr<SendBuffer> MakeSendBuffer(T& pkt, uint16_t pktId)
    {
        const uint16_t dataSize = pkt.GetSize();
        const uint16_t packetSize = dataSize + sizeof(PacketHeader);
        std::shared_ptr<SendBuffer> sendBuffer = GSendBufferManager->Open(packetSize);
        PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
        header->size = packetSize;
        pkt.Serialize(reinterpret_cast<BYTE*>(&header[1]));
        header->id = pktId;
        sendBuffer->Close(packetSize);

        return sendBuffer;
    }
};