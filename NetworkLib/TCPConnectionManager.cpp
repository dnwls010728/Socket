#include "pch.h"
#include "TCPConnectionManager.h"
#include <iostream>

namespace Net {

    TCPConnectionManager::TCPConnectionManager() : nextUniqueKey_(1) {}

    uint32_t TCPConnectionManager::AddClient(SOCKET socket, const NetAddress& address) {
        std::lock_guard<std::mutex> lock(mutex_);
        TCPConnectionState state;
        state.uniqueKey = nextUniqueKey_++;
        state.address = address;
        state.socket = socket;
        state.lastResponseTime = std::chrono::steady_clock::now();
        connections_[state.uniqueKey] = state;
        return state.uniqueKey;
    }

    void TCPConnectionManager::RemoveClient(uint32_t client_key) {
        std::lock_guard<std::mutex> lock(mutex_);
        connections_.erase(client_key);
    }

    TCPConnectionState TCPConnectionManager::GetClientState(uint32_t client_key) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = connections_.find(client_key);
        if (it != connections_.end()) {
            return it->second;
        }
        return TCPConnectionState{0,};
    }

    TCPConnectionState TCPConnectionManager::GetClientStateBySocket(SOCKET socket)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& pair : connections_)
        {
            if (pair.second.socket == socket) {
                return pair.second;
            }
        }
		return TCPConnectionState{ 0, };
    }

    std::unordered_map<uint32_t, TCPConnectionState> TCPConnectionManager::GetAllConnections() {
        std::lock_guard<std::mutex> lock(mutex_);
        return connections_;
    }

	void TCPConnectionManager::UpdateClientResponseTime(uint32_t client_key) {
		std::lock_guard<std::mutex> lock(mutex_);
		auto it = connections_.find(client_key);
		if (it != connections_.end()) {
			it->second.lastResponseTime = std::chrono::steady_clock::now();
		}
	}       

    size_t TCPConnectionManager::GetClientCount() {
        std::lock_guard<std::mutex> lock(mutex_);
        return connections_.size();
    }

} // namespace Net
