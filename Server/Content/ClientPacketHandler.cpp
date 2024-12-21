#include "pch.h"
#include "ClientPacketHandler.h"
#include "User.h"
#include "Room.h"
#include <iostream>

PacketHandlerFunc GPacketHandler[UINT16_MAX];
LobbyPacketHandler* GLobby= new LobbyPacketHandler();
InGamePacketHandler* GInGame= new InGamePacketHandler();
void HandleInvalid(const shared_ptr<PacketSession>& session, BYTE* buf, int32_t len)
{
}

void HandleActorAttack(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorAttack> pkt)
{
}

void HandleActorMove(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorMove> pkt)
{
}

void HandleActorAppear(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorAppear> pkt)
{
}

void HandleActorDisappear(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorDisappear> pkt)
{
}

void HandleGetItems(const shared_ptr<PacketSession>& session, shared_ptr<C_GetItems> pkt)
{
}

void HandleUseItem(const shared_ptr<PacketSession>& session, shared_ptr<C_UseItem> pkt)
{
}

void HandleEnterToLobby(const shared_ptr<PacketSession>& session, shared_ptr<C_EnterToLobby> pkt)
{
    GLobby->HandleEnterToLobby(session, pkt);
}

void HandleEnterToMatching(const shared_ptr<PacketSession>& session, shared_ptr<C_EnterToMatching> pkt)
{
    GLobby->HandleEnterToMatching(session, pkt);
}

void HandleMatchingCancel(const shared_ptr<PacketSession>& session, shared_ptr<C_MatchingCancel> pkt)
{
    GLobby->HandleMatchingCancel(session, pkt);
}

void HandleJoinGame(const shared_ptr<PacketSession>& session, shared_ptr<C_JoinGame> pkt)
{
    GLobby->HandleJoinGame(session, pkt);
}

void HandleSignIn(const shared_ptr<PacketSession>& session, shared_ptr<C_SignIn> pkt)
{
    GLobby->HandleSignIn(session, pkt);
}

void HandleLogin(const shared_ptr<PacketSession>& session, shared_ptr<C_Login> pkt)
{
    GLobby->HandleLogin(session, pkt);
}

void HandleGetMyAwardInfo(const shared_ptr<PacketSession>& session, shared_ptr<C_GetMyAwardInfo> pkt)
{
    GLobby->HandleGetMyAwardInfo(session, pkt);
}

void HandleSelectCharacter(const shared_ptr<PacketSession>& session, shared_ptr<C_SelectCharacter> pkt)
{
    GLobby->HandleSelectCharacter(session, pkt);
}

void HandleSelectWeapon(const shared_ptr<PacketSession>& session, shared_ptr<C_SelectWeapon> pkt)
{
    GLobby->HandleSelectWeapon(session,pkt);
}

void HandleBuyWeapon(const shared_ptr<PacketSession>& session, shared_ptr<C_BuyWeapon> pkt)
{
    GLobby->HandleBuyWeapon(session, pkt);
}

void HandleCacheFulling(const shared_ptr<PacketSession>& session, shared_ptr<C_CacheFulling> pkt)
{
    GLobby->HandleCacheFulling(session, pkt);
}
