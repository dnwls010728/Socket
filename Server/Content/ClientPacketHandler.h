#pragma once
#include "Room.h"
#include "../../CommonDLL/InGameContentPacket.h"
#include "../../CommonDLL/LobbyChannelPacket.h"
#include "../../CommonDLL/SendBuffer.h"
#include "../../CommonDLL/Packet.h"
#include "Packet/InGamePacketHandler.h"
#include "Packet/LobbyPacketHandler.h"
using PacketHandlerFunc = function<void(const shared_ptr<PacketSession>&, BYTE*, int32_t)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];
extern LobbyPacketHandler* GLobby;
extern InGamePacketHandler* GInGame;

enum InGameContentPacketNumber : uint16_t
{
	C_PKT_ACTOR_ATTACK=2000,
	S_PKT_ACTOR_ATTACK=2001,
	C_PKT_ACTOR_MOVE=2002,
	S_PKT_ACTOR_MOVE=2003,
	C_PKT_ACTOR_APPEAR=2004,
	S_PKT_ACTOR_APPEAR=2005,
	C_PKT_ACTOR_DISAPPEAR=2006,
	S_PKT_ACTOR_DISAPPEAR=2007,
	C_PKT_GET_ITEMS=2008,
	S_PKT_GET_ITEMS=2009,
	C_PKT_USE_ITEM=2010,
	S_PKT_USE_ITEM=2011,
	S_PKT_IS_CLEAR=2012
	
};

enum LobbyChannelPacketNumber: uint16_t
{
	C_PKT_ENTER_TO_LOBBY=3000,
	S_PKT_ENTER_TO_LOBBY=3001,
	C_PKT_ENTER_TO_MATCHING=3002,
	S_PKT_ENTER_TO_MATCHING=3003,
	S_PKT_MATCHING_FINISHED=3004,
	C_PKT_MATCHING_CANCEL=3005,
	S_PKT_MATCHING_CANCEL=3006,
	C_PKT_JOIN_GAME=3007,
	S_PKT_JOIN_GAME=3008,
	C_PKT_SIGN_IN=3009,
	S_PKT_SIGN_IN=3010,
	C_PKT_LOGIN=3011,
	S_PKT_LOGIN=3012,
	C_PKT_GET_MY_AWARD_INFO=3013,
	S_PKT_GET_MY_AWARD_INFO=3014,
	C_PKT_SELECT_CHARACTER=3015,
	S_PKT_SELECT_CHARACTER=3016,
	C_PKT_SELECT_WEAPON=3017,
	S_PKT_SELECT_WEAPON=3018,
	C_PKT_BUY_WEAPON=3019,
	S_PKT_BUY_WEAPON=3020,
	C_PKT_CACHE_FULLING=3021,
	S_PKT_CACHE_FULLING=3022
	
};
//Packet.h
void HandleInvalid(const shared_ptr<PacketSession>& session, BYTE* buf, int32_t len);



//InGameContentPacket.h
void HandleActorAttack(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorAttack> pkt);
void HandleActorMove(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorMove> pkt);
void HandleActorAppear(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorAppear> pkt);
void HandleActorDisappear(const shared_ptr<PacketSession>& session, shared_ptr<C_ActorDisappear> pkt);
void HandleGetItems(const shared_ptr<PacketSession>& session,shared_ptr<C_GetItems> pkt);
void HandleUseItem(const shared_ptr<PacketSession>& session, shared_ptr<C_UseItem> pkt);

//LobbyChannelPacket.h
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


class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32_t i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = HandleInvalid;

		// InGameContentPacketNumber의 C_ 접두사 패킷 처리기 등록
		GPacketHandler[C_PKT_ACTOR_ATTACK] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{
			return HandlePacket<C_ActorAttack>(HandleActorAttack, session, buffer, len);
		};

		GPacketHandler[C_PKT_ACTOR_MOVE] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{
			return HandlePacket<C_ActorMove>(HandleActorMove, session, buffer, len);
		};

		GPacketHandler[C_PKT_ACTOR_APPEAR] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{
			return HandlePacket<C_ActorAppear>(HandleActorAppear, session, buffer, len);
		};

		GPacketHandler[C_PKT_ACTOR_DISAPPEAR] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{
			return HandlePacket<C_ActorDisappear>(HandleActorDisappear, session, buffer, len);
		};

		GPacketHandler[C_PKT_USE_ITEM] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{
			return HandlePacket<C_UseItem>(HandleUseItem, session, buffer, len);
		};
		GPacketHandler[C_PKT_GET_ITEMS] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{
			return HandlePacket<C_GetItems>(HandleGetItems, session, buffer, len);
		};

		// LobbyChannelPacketNumber의 C_ 접두사 패킷 처리기 등록
		GPacketHandler[C_PKT_ENTER_TO_LOBBY] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{
			return HandlePacket<C_EnterToLobby>(HandleEnterToLobby, session, buffer, len);
		};

		GPacketHandler[C_PKT_ENTER_TO_MATCHING] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{
			return HandlePacket<C_EnterToMatching>(HandleEnterToMatching, session, buffer, len);
		};

		GPacketHandler[C_PKT_MATCHING_CANCEL] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{
			return HandlePacket<C_MatchingCancel>(HandleMatchingCancel, session, buffer, len);
		};
		// 새로 추가된 패킷 처리기 등록
		GPacketHandler[C_PKT_JOIN_GAME] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{
			return HandlePacket<C_JoinGame>(HandleJoinGame, session, buffer, len);
		};

		GPacketHandler[C_PKT_SIGN_IN] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{
			return HandlePacket<C_SignIn>(HandleSignIn, session, buffer, len);
		};

		GPacketHandler[C_PKT_LOGIN] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{
			return HandlePacket<C_Login>(HandleLogin, session, buffer, len);
		};

		GPacketHandler[C_PKT_GET_MY_AWARD_INFO] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{
			return HandlePacket<C_GetMyAwardInfo>(HandleGetMyAwardInfo, session, buffer, len);
		};

		GPacketHandler[C_PKT_SELECT_CHARACTER] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{
			return HandlePacket<C_SelectCharacter>(HandleSelectCharacter, session, buffer, len);
		};

		GPacketHandler[C_PKT_SELECT_WEAPON] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{
			return HandlePacket<C_SelectWeapon>(HandleSelectWeapon, session, buffer, len);
		};

		GPacketHandler[C_PKT_BUY_WEAPON] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{
			return HandlePacket<C_BuyWeapon>(HandleBuyWeapon, session, buffer, len);
		};

		GPacketHandler[C_PKT_CACHE_FULLING] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
		{
			return HandlePacket<C_CacheFulling>(HandleCacheFulling, session, buffer, len);
		};
		
		Room::Init();
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