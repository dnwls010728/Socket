#pragma once
class NetworkConnector
{
public:
	NetworkConnector() = default;
	NetworkConnector(SOCKADDR_IN sockAddr);
	NetworkConnector(wstring ip, UINT16 port);
	
	SOCKADDR_IN& GetSockAddr() { return _sockAddr; }
	wstring GetIpAddress();
	UINT16 GetPort() const { return ::ntohs(_sockAddr.sin_port); }

	static IN_ADDR Ip2Address(const WCHAR* ip);

private:
	SOCKADDR_IN _sockAddr = {};

};

