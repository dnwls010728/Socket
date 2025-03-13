#pragma once

#include "IPacket.h"
#include "string"

namespace Net {

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
}