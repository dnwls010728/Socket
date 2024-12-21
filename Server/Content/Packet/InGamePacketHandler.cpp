#include "pch.h"
#include "InGamePacketHandler.h"

void InGamePacketHandler::HandleActorAttack(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorAttack> pkt)
{
}

void InGamePacketHandler::HandleActorMove(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorMove> pkt)
{
}

void InGamePacketHandler::HandleActorAppear(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorAppear> pkt)
{
}

void InGamePacketHandler::HandleActorDisappear(const shared_ptr<PacketSession>& session,
    shared_ptr<C_ActorDisappear> pkt)
{
}

void InGamePacketHandler::HandleGetItems(const shared_ptr<PacketSession>& session, shared_ptr<C_GetItems> pkt)
{
}

void InGamePacketHandler::HandleUseItem(const shared_ptr<PacketSession>& session, shared_ptr<C_UseItem> pkt)
{
}
