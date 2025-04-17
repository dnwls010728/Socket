#include "ClientInfo.h"

ClientInfo::ClientInfo(int client_number)
	: client_number_(client_number)
	, room_number_(-1)
	, observed_room_list(false)
	, client_name_(L"")
	, db_key_(-1)
	, account_number_(0)
{
}


ClientInfo::ClientInfo(ClientInfo&& client_info) noexcept
{
	client_number_ = client_info.client_number_;
	room_number_ = client_info.room_number_;
	observed_room_list = client_info.observed_room_list;
	client_name_ = std::move(client_info.client_name_);
	db_key_ = client_info.db_key_;
}

ClientInfo& ClientInfo::operator=(ClientInfo&& client_info) noexcept
{
	if (this != &client_info) 
	{
		client_number_ = client_info.client_number_;
		room_number_ = client_info.room_number_;
		observed_room_list = client_info.observed_room_list;
		client_name_ = client_info.client_name_;
		db_key_ = client_info.db_key_;
	}
	return *this;
}

ClientInfo::~ClientInfo()
{
}
