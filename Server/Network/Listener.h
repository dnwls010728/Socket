#pragma once


#include "IocpCore.h"
#include "NetworkConnector.h"

class AcceptEvent;
class ServerService;

class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener() override;
	Listener(const Listener&) = delete;
	Listener& operator=(const Listener&) = delete;
	Listener(Listener&&) = delete;
	Listener& operator=(Listener&&) = delete;

	
public:
	/* �ܺο��� ��� */
	bool StartAccept(shared_ptr<ServerService> service);
	void CloseSocket();

public:
	/* �������̽� ���� */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int numOfBytes = 0) override;

private:
	/* ���� ���� */
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

protected:
	SOCKET _socket = INVALID_SOCKET;
	vector<AcceptEvent*> _acceptEvents;
	shared_ptr<ServerService> _service;
};
