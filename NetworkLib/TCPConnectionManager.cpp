#include "pch.h"
#include "TCPConnectionManager.h"
#include <iostream>

namespace Net {

    TCPConnectionManager::TCPConnectionManager() : nextUniqueKey_(1) {}

    void TCPConnectionManager::AddClient(SOCKET socket, const NetAddress& address) {
        std::lock_guard<std::mutex> lock(mutex_);
        TCPConnectionState state;
        state.uniqueKey = nextUniqueKey_++;
        state.address = address;
        state.socket = socket;
        state.lastResponseTime = std::chrono::steady_clock::now();
        connections_[socket] = state;
    }

    void TCPConnectionManager::RemoveClient(SOCKET socket) {
        std::lock_guard<std::mutex> lock(mutex_);
        connections_.erase(socket);
    }

    TCPConnectionState TCPConnectionManager::GetClientState(SOCKET socket) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = connections_.find(socket);
        if (it != connections_.end()) {
            return it->second;
        }
        return TCPConnectionState{};
    }

    std::map<SOCKET, TCPConnectionState> TCPConnectionManager::GetAllConnections() {
        std::lock_guard<std::mutex> lock(mutex_);
        return connections_;
    }

	void TCPConnectionManager::UpdateClientResponseTime(SOCKET socket) {
		std::lock_guard<std::mutex> lock(mutex_);
		auto it = connections_.find(socket);
		if (it != connections_.end()) {
			it->second.lastResponseTime = std::chrono::steady_clock::now();
		}
	}       

    size_t TCPConnectionManager::GetClientCount() {
        std::lock_guard<std::mutex> lock(mutex_);
        return connections_.size();
    }

} // namespace Net
