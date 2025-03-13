#include "pch.h"
#include "UDPConnectionManager.h"
#include <string>
#include <chrono>
#include <optional>

namespace Net {

    UDPConnectionState* UDPConnectionManager::GetConnectionState(const NetAddress& net_address)
    {
        std::string key = net_address.ToString();
        std::lock_guard<std::mutex> lock(connections_mutex_);

        auto it = connections_.find(key);
        if (it == connections_.end())
        {
            return nullptr;
        }

        return &(it->second);
    }

    UDPConnectionState& UDPConnectionManager::AddClient(const NetAddress& net_address)
	{
        std::string key = net_address.ToString();
        bool new_connection = false;
        UDPConnectionState* conn_ptr;
        {
            std::lock_guard<std::mutex> lock(connections_mutex_);
           
            auto [it, inserted] = connections_.try_emplace(key);
            UDPConnectionState& conn = it->second;
            conn_ptr = &conn;

            if (inserted)
            {
                new_connection = true;
                conn.expected_sequence_number = 1;
                conn.next_sequence_number = 1;
                conn.last_activity_time = std::chrono::steady_clock::now();
                conn.remote_address = net_address;
            }
            else
            {
                // 기존 연결의 마지막 활동 시간 업데이트
                conn.last_activity_time = std::chrono::steady_clock::now();
            }
        }
        if (new_connection)
        {
            if(on_connect_callback_) on_connect_callback_(net_address);
        }

        return *conn_ptr;
	}   

    void UDPConnectionManager::RemoveClient(const NetAddress& net_address)
    {
        size_t remove_count = 0;
        std::string key = net_address.ToString();
        {
            std::lock_guard<std::mutex> lock(connections_mutex_);
            remove_count = connections_.erase(key);
        }

        if (remove_count != 0 && on_disconnect_callback_)
        {
            on_disconnect_callback_(net_address);
        }
    }
    /*
    std::map<std::string, UDPConnectionState> UDPConnectionManager::GetAllConnections()
    {
        std::lock_guard<std::mutex> lock(connections_mutex_);
        return connections_;
    }*/

    void UDPConnectionManager::UpdateClientResponseTime(const NetAddress& net_address)
    {
        std::string key = net_address.ToString();
        std::lock_guard<std::mutex> lock(connections_mutex_);
        auto it = connections_.find(key);
        if (it != connections_.end())
        {
            it->second.last_activity_time = std::chrono::steady_clock::now();
        }
    }

    size_t UDPConnectionManager::GetClientCount()
    {
        std::lock_guard<std::mutex> lock(connections_mutex_);
        return connections_.size();
    }

    /*
    void UDPConnectionManager::CleanUpConnections()
    {
        auto now = std::chrono::steady_clock::now();
        std::lock_guard<std::mutex> lock(connections_mutex_);
        for (auto it = connections_.begin(); it != connections_.end(); )
        {
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - it->second.last_activity_time);
            if (elapsed.count() > 60)
            {
                it = connections_.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    */

} // namespace Net
