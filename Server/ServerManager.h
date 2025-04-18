#pragma once

#include "Singleton.h"
#include "TCPServerSocket.h"
#include "MySQL/MySQLManager.h"
#include "Session/SessionManager.h"

class ServerManager : public Singleton<ServerManager>
{
public:
	ServerManager();
	virtual ~ServerManager() override = default;

	bool Execute();

	bool OnClientConnected(const Net::TCPConnectionState& state);
	void OnClientDisconnected(const Net::TCPConnectionState& state);
	void OnPacketReceived(const Net::TCPConnectionState& state, std::unique_ptr<Net::IPacket> packet);

private:
	Net::TCP::TCPServerSocket server_socket_;

	MySQLManager mysql_manager_;
	SessionManager session_manager_;
};
