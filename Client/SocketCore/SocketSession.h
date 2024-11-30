#pragma once
#include <winsock2.h>
#include <iostream>
#include "../../CommonDLL/SendBuffer.h"
#include "../../CommonDLL/RecvBuffer.h"
#include <vector>
#include <thread>
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 7777
#define BUFFER_SIZE 1024

struct PacketHeader
{
	uint16_t size;
	uint16_t id;
};
struct IOContext {
	OVERLAPPED overlapped;
	WSABUF wsabuf;
	char buffer[BUFFER_SIZE];
	int operation; // 0: recv, 1: send
};
class SocketSession : public std::enable_shared_from_this<SocketSession>
{
public:

	void WorkerThread(HANDLE hIOCP,IOContext* ioContext) {
		DWORD bytesTransferred;
		ULONG_PTR completionKey;
		
		while (true) {
			BOOL result = GetQueuedCompletionStatus(hIOCP, &bytesTransferred, &completionKey, (LPOVERLAPPED*)&ioContext, INFINITE);
			if (!result) {
				std::cerr << "GetQueuedCompletionStatus failed: " << GetLastError() << std::endl;
				continue;
			}

			if (bytesTransferred == 0) {
				std::cout << "Connection closed by server." << std::endl;
				continue;
			}

			if (ioContext->operation == 0) { // recv
				if (bytesTransferred == 0)
				{
					Disconnect("Recv 0.");
					continue;	
				}
				if (_recvBuffer.OnWrite(bytesTransferred) == false)
				{
					Disconnect("Onwrite Overflow");
					continue;
				}
				int32_t dataSize = _recvBuffer.DataSize();
				int32_t processLen = OnRecv(_recvBuffer.ReadPos(), dataSize);
				if (processLen < 0 || dataSize < processLen || _recvBuffer.OnRead(processLen) == false)
				{
					Disconnect("OnRead Overflow");
					continue;
				}
				_recvBuffer.Clean();

				ProcessRecv(0,ioContext);

				//WSASend((SOCKET)completionKey, &ioContext->wsabuf, 1, NULL, flags, &ioContext->overlapped, NULL);
			}
			else { // send
				OnSend(bytesTransferred);

				if (bytesTransferred == 0)
				{
					Disconnect("Send 0.");
					continue;
				}

				//�� �̻� ó���� ������ ����
			}
		}
	}


	SocketSession();
	~SocketSession();
	bool Connect();
	void Send(std::shared_ptr<SendBuffer> buffer);
	void Disconnect(const char* cause);

	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessRecv(int numOfBytes,IOContext* ioContext);
	void ProcessSend(int numOfBytes);
	void OnRecvPacket(BYTE* buffer, int len);

	std::shared_ptr<SocketSession> GetSocketSessionRef() { return static_pointer_cast<SocketSession>(shared_from_this()); }

	void OnConnected();	
	int OnRecv(BYTE* buffer, int len);
	void OnSend(int len);
	void OnDisconnected();
	SOCKET GetSocket() const { return _socket; }
	bool IsConnected() const{ return _connected; }
	std::shared_ptr<SocketSession> GetSessionRef() { return static_pointer_cast<SocketSession>(shared_from_this()); }
private:
	SOCKET _socket = INVALID_SOCKET;
	std::mutex _mutex;
	RecvBuffer _recvBuffer;
	std::atomic<bool> _connected = false;
	SOCKADDR_IN _sockAddr = {};
	std::vector<std::thread> _workers;
	IOContext *  _context;
};

extern SocketSession* GSocketSession;