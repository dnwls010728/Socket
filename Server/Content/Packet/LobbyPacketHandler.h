#pragma once
#include "../../../CommonDLL/LobbyChannelPacket.h"
class LobbyPacketHandler
{
public:
    void HandleEnterToLobby(const shared_ptr<PacketSession>& session, shared_ptr<C_EnterToLobby> pkt);
    void HandleEnterToMatching(const shared_ptr<PacketSession>& session, shared_ptr<C_EnterToMatching> pkt);
    void HandleMatchingCancel(const shared_ptr<PacketSession>& session, shared_ptr<C_MatchingCancel> pkt);
    void HandleJoinGame(const shared_ptr<PacketSession>& session, shared_ptr<C_JoinGame> pkt);
    void HandleSignIn(const shared_ptr<PacketSession>& session, shared_ptr<C_SignIn> pkt);
    void HandleLogin(const shared_ptr<PacketSession>& session, shared_ptr<C_Login> pkt);
    void HandleGetMyAwardInfo(const shared_ptr<PacketSession>& session, shared_ptr<C_GetMyAwardInfo> pkt);
    void HandleSelectCharacter(const shared_ptr<PacketSession>& session, shared_ptr<C_SelectCharacter> pkt);
    void HandleSelectWeapon(const shared_ptr<PacketSession>& session, shared_ptr<C_SelectWeapon> pkt);
    void HandleBuyWeapon(const shared_ptr<PacketSession>& session, shared_ptr<C_BuyWeapon> pkt);
    void HandleCacheFulling(const shared_ptr<PacketSession>& session, shared_ptr<C_CacheFulling> pkt);
};
