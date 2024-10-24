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
#include "ServerPacketHandler.h"
SocketSession* GSocketSession = new SocketSession();
SocketSession::SocketSession():_recvBuffer(BUFFER_SIZE),_context(IOContext{})
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

	if(connect(_socket, (sockaddr*)&_sockAddr, sizeof(_sockAddr)) == SOCKET_ERROR)
	{
		return false;
	};

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
	wsaBuf.len = static_cast<long>(buffer->WriteSize());
	DWORD numOfBytes = 0;
	if(SOCKET_ERROR == ::WSASend(_socket, &wsaBuf, 1, &numOfBytes, 0, nullptr, nullptr))
	{
		int errCode = WSAGetLastError();
		if(errCode != WSA_IO_PENDING)
		{
			std::cout << "Error in WSASend: " << errCode << std::endl;
		}
	}
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

	WSARecv(_socket, &ioContext->wsabuf, 1, &nob, &flags, &ioContext->overlapped, nullptr);
}

void SocketSession::ProcessSend(int numOfBytes)
{
}

void SocketSession::OnRecvPacket(BYTE* buffer, int len)
{
	//std::shared_ptr<SocketSession> session = GetSocketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	ServerPacketHandler::HandlePacket(buffer, len);
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

		//������� ũ�Ⱑ ���� ��û�� ����
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
	std::cout<< "Message Send : " << len << std::endl;
}

void SocketSession::OnDisconnected()
{
}
