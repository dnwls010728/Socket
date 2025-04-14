#include "pch.h"
#include "NetworkManager.h"
#include <ws2tcpip.h>

namespace Net
{

	bool WSAInit()
	{
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			return false;

		return true;
	}

	void WSAUninit()
	{
		WSACleanup();
	}

	/*
	HANDLE NetworkManager::CreateIOCPHandle()
	{
		return CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	}

	void NetworkManager::CloseIOCPHandle(HANDLE iocp_handle)
	{
		if (iocp_handle)
			CloseHandle(iocp_handle);
	}


	bool NetworkManager::GetTCPIOStatus(HANDLE iocp_handle, TCPIOResult& result, int timeout_ms)
	{
		DWORD length;
		TCPIOContext* io_context = NULL;
		bool status_result = false;
		SOCKET receive_socket;

		status_result = GetQueuedCompletionStatus(iocp_handle, &length, &receive_socket, (LPOVERLAPPED*)&io_context, timeout_ms);
		if (status_result == false || length == 0)
		{
			return false;
		}

		if (io_context == NULL)
		{
			return false;
		}
		result.buffer.assign(io_context->buffer.buf, io_context->buffer.buf + length);
		result.net_socket = io_context->tcp_socket;
		delete[] io_context->buffer.buf;
		delete io_context;

		return true;
	}
	*/
} // namespace Net		