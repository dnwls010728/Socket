#pragma once
#include "Serializer.h"
#include "CommonObject.h"


class CustomSerializer : public Net::Serializer
{
public:
	EXTEND_SERIALIZER()

	CustomSerializer& operator<<(const Movement& value)
	{
		*this << value.x;
		*this << value.y;
		return *this;
	}

	CustomSerializer& operator>>(Movement& value)
	{
		*this >> value.x;
		*this >> value.y;
		return *this;
	}

	CustomSerializer& operator<<(const CharacterInfo& value)
	{
		*this << value.unique_id;
		*this << value.account_unique_id;
		*this << value.name;
		*this << value.lv;
		*this << value.job;
		*this << value.map_unique_id;
		*this << value.last_position_x;
		*this << value.last_position_y;
		return *this;
	}

	CustomSerializer& operator>>(CharacterInfo& value)
	{
		*this >> value.unique_id;
		*this >> value.account_unique_id;
		*this >> value.name;
		*this >> value.lv;
		*this >> value.job;
		*this >> value.map_unique_id;
		*this >> value.last_position_x;
		*this >> value.last_position_y;
		return *this;
	}

	CustomSerializer& operator<<(const ObjectInfo& value)
	{
		*this << value.name;
		*this << value.class_name;
		*this << value.unique_id;
		*this << value.last_position_x;
		*this << value.last_position_y;
		*this << value.map_unique_id;
		return *this;
	}

	CustomSerializer& operator>>(ObjectInfo& value)
	{
		*this >> value.name;
		*this >> value.class_name;
		*this >> value.unique_id;
		*this >> value.last_position_x;
		*this >> value.last_position_y;
		*this >> value.map_unique_id;
		return *this;
	}

};