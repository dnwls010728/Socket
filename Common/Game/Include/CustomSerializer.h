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
        *this << value.page_index;
        *this << value.room_count;
		*this << value.room_list;
        return *this;
    }

    CustomSerializer& operator>>(RoomList& value) {
        *this >> value.page_index;
        *this >> value.room_count;
        *this >> value.room_list;
        return *this;
    }
};