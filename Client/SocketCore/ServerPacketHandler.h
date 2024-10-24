#pragma once

#include <memory>
#include "Packet.h"
#include "SocketSession.h"
using PacketHandlerFunc = std::function<void(BYTE*, int32_t)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum PacketNumber : uint16_t
{
    C_PKT_ENTER = 1000,
    S_PKT_ENTER = 1001
};

void HandleInvalid(BYTE* buf, int32_t len);
void HandleEnter(S_EnterPacket& pkt);

class ServerPacketHandler
{
public:
    static void Init()
    {
        for(int32_t i=0; i<UINT16_MAX; i++)
            GPacketHandler[i] = HandleInvalid;
        GPacketHandler[S_PKT_ENTER] = [](BYTE* buffer,int32_t len){return HandlePacket<S_EnterPacket>(HandleEnter,buffer,len);};
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
        PacketType pkt;
        pkt.Deserialize(buffer + sizeof(PacketHeader),len-sizeof(PacketHeader));
        return func(pkt);
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