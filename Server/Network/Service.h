#pragma once

#include "NetworkConnector.h"
#include "IocpCore.h"
#include "Listener.h"
#include <functional>
enum class ServiceType : uint8_t
{
	Server,
	Client
};


using SessionFactory = function<shared_ptr<Session>(void)>;

class Service : public enable_shared_from_this<Service>
{
public:
	Service(ServiceType type, NetworkConnector address, const shared_ptr<IocpCore>& core, SessionFactory factory, int maxSessionCount = 1);
	virtual ~Service()=default;
	Service(const Service&) = delete;
	Service& operator=(const Service&) = delete;
	Service(Service&&) = delete;
	Service& operator=(Service&&) = delete;

	virtual bool		Start() abstract;
	bool				CanStart() const { return _sessionFactory != nullptr; }

	virtual void		CloseService();
	void				SetSessionFactory(SessionFactory func) { _sessionFactory = func; }

	void				Broadcast(const shared_ptr<SendBuffer>& buffer);
	shared_ptr<Session>			CreateSession();
	void				AddSession(const shared_ptr<Session>& session);
	void				ReleaseSession(const shared_ptr<Session>& session);
	int				GetCurrentSessionCount() const { return _sessionCount; }
	int				GetMaxSessionCount() const { return _maxSessionCount; }

public:
	ServiceType			GetServiceType() const { return _type; }
	NetworkConnector			GetNetAddress() const { return _netAddress; }
	shared_ptr<IocpCore>& GetIocpCore() { return _iocpCore; }

protected:
	USE_LOCK;
	ServiceType			_type;
	NetworkConnector			_netAddress = {};
	shared_ptr<IocpCore>			_iocpCore;

	set<shared_ptr<Session>>		_sessions;
	int				_sessionCount = 0;
	int				_maxSessionCount = 0;
	SessionFactory		_sessionFactory;

};

class ClientService final : public Service
{
public:
	ClientService(NetworkConnector targetAddress, shared_ptr<IocpCore> core, SessionFactory factory, int maxSessionCount = 1);
	virtual ~ClientService() override = default;
	ClientService(const ClientService&) = delete;
	ClientService& operator=(const ClientService&) = delete;
	ClientService(ClientService&&) = delete;
	ClientService& operator=(ClientService&&) = delete;
	
	virtual bool Start() override;
};

class ServerService final : public Service
{
public:
	ServerService(NetworkConnector targetAddress, shared_ptr<IocpCore> core, SessionFactory factory, int maxSessionCount = 1);
	virtual ~ServerService() override;
	ServerService(const ServerService&) = delete;
	ServerService& operator=(const ServerService&) = delete;
	ServerService(ServerService&&) = delete;
	ServerService& operator=(ServerService&&) = delete;
	
	
	virtual bool Start() override;
	virtual void CloseService() override;

private:
	shared_ptr<Listener> _listener = nullptr;

};
