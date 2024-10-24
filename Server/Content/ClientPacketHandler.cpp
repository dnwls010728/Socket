#include "pch.h"
#include "ClientPacketHandler.h"
#include <iostream>
PacketHandlerFunc GPacketHandler[UINT16_MAX];

void HandleInvalid(const shared_ptr<PacketSession>& session, BYTE* buf, int32_t len)
{
}

void HandleEnter(const shared_ptr<PacketSession>& session, C_EnterPacket& pkt)
{
    cout << "Enter EnterPacket" << endl;
    cout << pkt.GetName()<<endl;
    cout<< pkt.GetId() << endl;
    S_EnterPacket sendPkt;
    sendPkt.SetSuccess(1);
    auto sendBuffer = ClientPacketHandler::MakeSendBuffer<S_EnterPacket>(sendPkt,S_PKT_ENTER);
    session->Send(sendBuffer);
}
