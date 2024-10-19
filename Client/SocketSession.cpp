#include "pch.h"
#include "SocketSession.h"
#include "RecvBuffer.h"
#include <mutex>
#include <vector>
#include <thread>
#include "SendBuffer.h"
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <atomic>
#pragma comment(lib,"ws2_32.lib")

SocketSession::SocketSession():_recvBuffer(BUFFER_SIZE)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

SocketSession::~SocketSession()
{
	for (auto& worker : _workers) {
		worker.join();
	}

	closesocket(_socket);
	WSACleanup();
}

bool SocketSession::Connect()
{
	_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	_sockAddr.sin_family = AF_INET;
	_sockAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	_sockAddr.sin_port = htons(SERVER_PORT);

	connect(_socket, (sockaddr*)&_sockAddr, sizeof(_sockAddr));

	HANDLE hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	CreateIoCompletionPort((HANDLE)_socket, hIocp, 0, 0);

	IOContext* ioContext = new IOContext;
	ZeroMemory(&ioContext->overlapped, sizeof(OVERLAPPED));
	ioContext->wsabuf.buf = reinterpret_cast<char*>(_recvBuffer.WritePos());
	ioContext->wsabuf.len = _recvBuffer.FreeSize();
	ioContext->operation = 0;

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	_connected.exchange(true);
	WSARecv(_socket, &ioContext->wsabuf, 1, &numOfBytes, &flags, &ioContext->overlapped, NULL);

	_workers.reserve(2);
	for (int i = 0; i < 2; i++)
	{
		_workers.emplace_back([this, hIocp]() { this->WorkerThread(hIocp); });
	}
	return true;
}

void SocketSession::Send(std::shared_ptr<SendBuffer> buffer)
{
	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(buffer->Buffer());
	wsaBuf.len = static_cast<long>(sizeof(buffer->WriteSize()));
	DWORD numOfBytes = 0;
	::WSASend(_socket, &wsaBuf, 1, &numOfBytes, 0, nullptr, nullptr);
}

void SocketSession::Disconnect(const char* cause)
{
	_connected.exchange(false);
	std::cout << "Disconnected: " << cause << std::endl;
	OnDisconnected();
}

void SocketSession::ProcessConnect()
{
}

void SocketSession::ProcessDisconnect()
{
}

void SocketSession::ProcessRecv(int numOfBytes)
{
	IOContext* ioContext = new IOContext;
	ZeroMemory(&ioContext->overlapped, sizeof(OVERLAPPED));
	ioContext->wsabuf.buf = reinterpret_cast<char*>(_recvBuffer.WritePos());
	ioContext->wsabuf.len = _recvBuffer.FreeSize();
	ioContext->operation = 0;

	DWORD nob = 0;
	DWORD flags = 0;

	WSARecv(_socket, &ioContext->wsabuf, 1, &nob, &flags, &ioContext->overlapped, NULL);
}

void SocketSession::ProcessSend(int numOfBytes)
{
}

void SocketSession::OnRecvPacket(BYTE* buffer, int len)
{
	std::shared_ptr<SocketSession> session = GetSocketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	//ServerPacketSession::HandlePacket(session, buffer, len);
}

void SocketSession::OnConnected()
{
}

int SocketSession::OnRecv(BYTE* buffer, int len)
{
	int processLen = 0;

	while (true)
	{
		int dataSize = len - processLen;

		//헤더보다 크기가 작은 요청은 무시
		if (dataSize < sizeof(PacketHeader))
			break;

		PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processLen]));

		if (dataSize < header.size)
			break;

		OnRecvPacket(&buffer[processLen], header.size);

		processLen += header.size;
	}
	return processLen;
}

void SocketSession::OnSend(int len)
{
}

void SocketSession::OnDisconnected()
{
}
