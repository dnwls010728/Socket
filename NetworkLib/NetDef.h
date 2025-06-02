#pragma once

#include "IPacket.h"
#include "string"

namespace Net {

	enum NetErrorCode : uint16_t
	{
		kSuccess = 0,
		kTimeout
	};

	struct NetAddress
	{
		NetAddress(const std::string& ip = "0.0.0.0", int p = 0) : ip_address(ip), port(p) {}

		std::string ip_address;
		unsigned short port;

		bool operator==(const NetAddress& rhs) const
		{
			return ip_address == rhs.ip_address && port == rhs.port;
		}

		bool operator!=(const NetAddress& rhs) const
		{
			return !(*this == rhs);
		}

		std::string ToString() const
		{
			return ip_address + ":" + std::to_string(port);
		}
	};


	// 임시
	namespace TCP {
		struct ReceivedPacketInfo
		{
			uint32_t client_key;
			std::unique_ptr<IPacket> packet;
		};

		// 패킷 전송 후 응답이 올 때 호출될 콜백
		struct PendingPacketCallback
		{
			uint32_t sequence;
			std::function<void(uint32_t client_key, std::unique_ptr<IPacket>)> callback;

			// 응답 만료 시간
			std::chrono::steady_clock::time_point expiration_time;
		};
	}
	
	inline float GetClientTime()
	{
		static const auto startTime = std::chrono::steady_clock::now();
		auto now = std::chrono::steady_clock::now();
		return std::chrono::duration<float>(now - startTime).count();
	}
}