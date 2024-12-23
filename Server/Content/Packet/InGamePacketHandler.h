#pragma once
#include "../../../CommonDLL/InGameContentPacket.h"

class InGamePacketHandler
{
public:
    void HandleActorAttack(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorAttack> pkt);
    void HandleActorMove(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorMove> pkt);
    void HandleActorAppear(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorAppear> pkt);
    void HandleActorDisappear(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorDisappear> pkt);
    void HandleGetItems(const shared_ptr<PacketSession>& session,shared_ptr<C_GetItems> pkt);
    void HandleUseItem(const shared_ptr<PacketSession>& session, shared_ptr<C_UseItem> pkt);
};
