#include "pch.h"
#include "NetUDPSocket.h"
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

namespace Net {

	NetUDPSocket::NetUDPSocket() :
		internal_socket_(INVALID_SOCKET),
		net_address_({ "",0 })
	{

	}

	NetUDPSocket::~NetUDPSocket()
	{
	}

	bool NetUDPSocket::Create()
	{
		internal_socket_ = socket(AF_INET, SOCK_DGRAM, 0);
		if (internal_socket_ == INVALID_SOCKET)
		{
			return false;
		}
		SetNonBlockingMode(true);

		return true;
	}

	void NetUDPSocket::Close()
	{
		if (internal_socket_ != INVALID_SOCKET)
		{
			closesocket(internal_socket_);
			internal_socket_ = INVALID_SOCKET;
		}
	}

	bool NetUDPSocket::SetNonBlockingMode(bool is_nonblock)
	{
		u_long mode = is_nonblock ? 1 : 0;
		return ioctlsocket(internal_socket_, FIONBIO, &mode) == 0;
	}

	bool NetUDPSocket::Bind(const NetAddress& net_address, bool reuse_addr)
	{
		if (reuse_addr)
		{
			int opt = 1;
			if (setsockopt(internal_socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0)
			{
				return false;
			}
		}

		// Set up socket address
		sockaddr_in addr = { 0, };
		addr.sin_family = AF_INET;
		addr.sin_port = htons(net_address.port);
		addr.sin_addr.s_addr = INADDR_ANY;
		if (inet_pton(AF_INET, net_address.ip_address.c_str(), &addr.sin_addr) <= 0)
		{
			return false;
		}

		// Bind the socket
		if (bind(internal_socket_, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			return false;
		}

		return true;
	}

	bool NetUDPSocket::SendTo(const NetAddress& net_address, const char* data, int data_length, int& sent_length)
	{
		sockaddr_in addr = { 0, };
		addr.sin_family = AF_INET;
		addr.sin_port = htons(net_address.port);
		if (inet_pton(AF_INET, net_address.ip_address.c_str(), &addr.sin_addr) <= 0)
		{
			return false;
		}

		sent_length = sendto(internal_socket_, data, data_length, 0, (sockaddr*)&addr, sizeof(addr));
		if (sent_length == SOCKET_ERROR)
		{
			return false;
		}
		return true;
	}

	bool NetUDPSocket::RecvFrom(char* data, int data_length, int& received_length, NetAddress& net_address)
	{
		sockaddr_in addr = { 0, };
		int addr_len = sizeof(addr);
		received_length = recvfrom(internal_socket_, data, data_length, 0, (sockaddr*)&addr, &addr_len);
		if (received_length <= 0)
		{
			return false;
		}

		char ip_buffer[INET_ADDRSTRLEN];
		if (inet_ntop(AF_INET, &addr.sin_addr, ip_buffer, INET_ADDRSTRLEN) == NULL)
		{
			return false;
		}

		net_address.ip_address = ip_buffer;
		net_address.port = ntohs(addr.sin_port);
		return true;
	}

} // namespace Net