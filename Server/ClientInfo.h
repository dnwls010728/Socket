#pragma once
#include <string>

class ClientInfo
{
public:
	ClientInfo(int client_number);
	ClientInfo(ClientInfo&& client_info) noexcept;
	ClientInfo& operator=(ClientInfo&& client_info) noexcept;
	~ClientInfo();

	int GetClientNumber() const { return client_number_; }

	// 없으면 -1
	int GetRoomNumber() const { return room_number_; }
	void SetRoomNumber(int room_number) { room_number_ = room_number; }

	void SetObserveRoomList(bool is_observe) { observed_room_list = is_observe; }
	bool IsObserveRoomList() const { return observed_room_list; }

	// 없으면 -1
	int GetDBKey() { return db_key_; }
	void SetDBKey(int db_key) { db_key_ = db_key; }

	std::wstring GetClientName() { return client_name_; }
	void SetClientName(std::wstring name) { client_name_ = name; }

	inline uint32_t GetAccountNumber() { return account_number_; }
	inline void SetAccountNumber(uint32_t account_number) { account_number_ = account_number; }

private:
	int client_number_;	
	int room_number_;
	bool observed_room_list;
	std::wstring client_name_;
	int db_key_;
	uint32_t account_number_;

};

