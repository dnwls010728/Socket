#pragma once;

using PacketHandlerFunc = function<void(const shared_ptr<PacketSession>&, BYTE*, int32_t)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum PacketNumber : uint16_t
{
	C_PKT_ENTER = 1000,
	S_PKT_ENTER = 1001
};

void HandleInvalid(const shared_ptr<PacketSession>& session, BYTE* buf, int32_t len);
void HandleEnter(const shared_ptr<PacketSession>& session, C_EnterPacket& pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32_t i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = HandleInvalid;
		GPacketHandler[C_PKT_ENTER] = [](const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len) {return HandlePacket<C_EnterPacket>(HandleEnter,session, buffer, len); };
	}

	static void HandlePacket(const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}

	static shared_ptr<SendBuffer> MakeSendBuffer(S_EnterPacket& pkt) { return MakeSendBuffer(pkt, S_PKT_ENTER); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, const shared_ptr<PacketSession>& session, BYTE* buffer, int32_t len) 
	{
		PacketType pkt;
		pkt.Deserialize(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader));
		return func(session, pkt);
	}
};