#include "pch.h"
#include "NetTCPSocket.h"
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

namespace Net::TCP {

NetTCPSocket::NetTCPSocket() :
	internal_socket_(INVALID_SOCKET)
{
}

NetTCPSocket::~NetTCPSocket()
{
}

bool NetTCPSocket::Create()
{
	internal_socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (internal_socket_ == INVALID_SOCKET)
	{
		return false;
	}

	return true;
}

void NetTCPSocket::Close()
{
	if (internal_socket_ != INVALID_SOCKET)
	{
		closesocket(internal_socket_);
		internal_socket_ = INVALID_SOCKET;
	}
}

bool NetTCPSocket::SetNonBlockingMode(bool is_nonblock)
{
	u_long mode = is_nonblock ? 1 : 0;
	return ioctlsocket(internal_socket_, FIONBIO, &mode) == 0;
}

/*
bool NetTCPSocket::RegisterIOCPHandle(HANDLE iocp_handle)
{
	HANDLE conn_handle = CreateIoCompletionPort((HANDLE)internal_socket_, iocp_handle, (ULONG_PTR)internal_socket_, 0);
	if (conn_handle == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	return true;
}
*/

bool NetTCPSocket::Bind(const NetAddress& net_address, bool reuse_addr)
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

bool NetTCPSocket::Listen(int backlog)
{
	if (listen(internal_socket_, backlog) == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool NetTCPSocket::Accept(NetTCPSocket& client_socket)
{
	SOCKET client_internal;
	sockaddr_in addr = { 0, };
	int addr_len = sizeof(addr);
	client_internal = accept(internal_socket_, reinterpret_cast<sockaddr*>(&addr), &addr_len);
	if (client_internal == INVALID_SOCKET)
	{
		return false;
	}

	client_socket.internal_socket_ = client_internal;

	char ip_buffer[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &addr.sin_addr, ip_buffer, INET_ADDRSTRLEN) == NULL)
	{
		return false;
	}

	client_socket.net_address_.ip_address = ip_buffer;
	client_socket.net_address_.port = ntohs(addr.sin_port);
	return true;
}

bool NetTCPSocket::Connect(const NetAddress& net_address)
{
	sockaddr_in addr = { 0, };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(net_address.port);
	if (inet_pton(AF_INET, net_address.ip_address.c_str(), &addr.sin_addr) <= 0)
	{
		return false;
	}
	if (connect(internal_socket_, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

bool NetTCPSocket::Send(const char* data, int data_length, int& sent_length)
{
	sent_length = send(internal_socket_, data, data_length, 0);
	if (sent_length <= 0)
	{
		return false;
	}
	return true;
}

bool NetTCPSocket::Recv(char* data, int data_length, int& received_length)
{
	received_length = recv(internal_socket_, data, data_length, 0);
	if (received_length <= 0)
	{
		return false;
	}

	return true;
}

bool NetTCPSocket::Shutdown(int how)
{
	if (internal_socket_ == INVALID_SOCKET)
		return false;

	int result = ::shutdown(internal_socket_, how);
	return result != SOCKET_ERROR;
}

/*
bool NetTCPSocket::RecvIOCP(int data_length)
{
	TCPIOContext* io_context = new TCPIOContext();
	io_context->tcp_socket = *this;
	io_context->buffer.buf = new char[data_length];
	io_context->buffer.len = data_length;
	ZeroMemory(&io_context->overlapped, sizeof(io_context->overlapped));

	DWORD flags = 0;
	int result = WSARecv(
		internal_socket_,
		&io_context->buffer,
		1,
		NULL,
		&flags,
		&io_context->overlapped,
		NULL
	);

	if (result == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error == WSA_IO_PENDING)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;
}
*/
} // namespace Net