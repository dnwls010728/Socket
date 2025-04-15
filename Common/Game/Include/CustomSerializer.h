#pragma once
#include "Serializer.h"
#include "CommonObject.h"


class CustomSerializer : public Net::Serializer
{
public:
	EXTEND_SERIALIZER()
    
    CustomSerializer& operator<<(const RoomInfo& value) {
		*this << value.current_user_count;
		*this << value.max_user_count;
		*this << value.room_number;
        *this << value.room_title;
        return *this;
    }

    CustomSerializer& operator>>(RoomInfo& value) {
        *this >> value.current_user_count;
        *this >> value.max_user_count;
        *this >> value.room_number;
        *this >> value.room_title;
        return *this;
    }

    CustomSerializer& operator<<(const RoomList& value) {
		*this << value.room_list;
        return *this;
    }

    CustomSerializer& operator>>(RoomList& value) {
        *this >> value.room_list;
        return *this;
    }

    CustomSerializer& operator<<(const ClientData& value) {
        *this << value.client_name;
        *this << value.client_number;
        return *this;
    }

    CustomSerializer& operator>>(ClientData& value) {
        *this >> value.client_name;
        *this >> value.client_number;
        return *this;
    }

	CustomSerializer& operator<<(const RoomInfoEx& value) {
		*this << value.room_number;
		*this << value.current_user_count;
		*this << value.max_user_count;
		*this << value.room_title;
		*this << value.client_list;
		return *this;
	}

	CustomSerializer& operator>>(RoomInfoEx& value) {
		*this >> value.room_number;
		*this >> value.current_user_count;
		*this >> value.max_user_count;
		*this >> value.room_title;
		*this >> value.client_list;
		return *this;
	}

};