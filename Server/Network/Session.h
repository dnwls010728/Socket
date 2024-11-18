#pragma once


#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetworkConnector.h"
#include "../../CommonDLL/RecvBuffer.h"
enum
{
	BUFFER_SIZE = 0x10000, //64KB
};
class Service;

class Session : public IocpObject
{

	friend class Listener;
	friend class IocpCore;
	friend class Service;
	
	

public:
	Session();
	virtual ~Session();

public:
	void Send(shared_ptr<SendBuffer> buffer);
	bool Connect();
	void Disconnect(const WCHAR* cause);

	shared_ptr<Service> GetService() { return _service.lock(); }
	void SetService(shared_ptr<Service> service) { _service = service; }

public:
	void SetNetAddress(NetworkConnector address) { _netAddress = address; }
	NetworkConnector GetAddress() { return _netAddress; }
	SOCKET GetSocket()  { return _socket; }
	bool IsConnected() { return _connected;}
	shared_ptr<Session> GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

private:
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int numOfBytes = 0) override;

private:
	bool RegisterConnect();
	bool RegisterDisconnect();
	void RegisterRecv();
	void RegisterSend();

	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessRecv(int numOfBytes);
	void ProcessSend(int numOfBytes);

	void HandlerError(int errCode);

protected:
	virtual void OnConnected() { }
	virtual int OnRecv(BYTE* buffer, int len) { return len; }
	virtual void OnSend(int len){}
	virtual void OnDisconnected(){}

private:
	weak_ptr<Service> _service;
	SOCKET _socket = INVALID_SOCKET;
	NetworkConnector _netAddress = {};
	atomic<bool> _connected = false;

private:
	USE_LOCK;

	RecvBuffer _recvBuffer;

	queue < shared_ptr<SendBuffer>> _sendQueue;
	atomic<bool> _sendRegistered = false;

private:
	/* IocpEvent ���� */
	ConnectEvent		_connectEvent;
	DisconnectEvent		_disconnectEvent;
	RecvEvent			_recvEvent;
	SendEvent			_sendEvent;
};

struct PacketHeader
{
	uint16_t size;
	uint16_t id;


};

class PacketSession : public Session
{
public :
	PacketSession();
	virtual ~PacketSession();

	shared_ptr<PacketSession> GetPacketSessionRef() { return static_pointer_cast<PacketSession>(shared_from_this()); }

protected:
	virtual int OnRecv(BYTE* buffer, int len) sealed;
	virtual void OnRecvPacket(BYTE* buffer, int len) abstract;
};