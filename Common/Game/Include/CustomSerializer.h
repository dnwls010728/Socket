#pragma once
#include "Serializer.h"
#include "CommonObject.h"

namespace Net
{
	/*
	 SERIALIZER_EXTEND( Example, v.a, v.b )
	 or
	 inline Serializer& operator<<(Serializer& s, const Example& v)
	 { 
		s << v.a;
		s << v.b;
		return s; 
	}
	inline Serializer& operator>>(Serializer& s, Example& v)
	{
		s >> v.a;
		s >> v.b;
		return s;
	}
	*/

	// CharacterInfo
	SERIALIZER_EXTEND(CharacterInfo,
		v.unique_id,
		v.account_id,
		v.name,
		v.lv,
		v.job,
		v.map_id,
		v.last_position_x,
		v.last_position_y
		)

	SERIALIZER_EXTEND(ItemInfo,
		v.inventory_type,
		v.item_id,
		v.slot_index,
		v.count
		)

	SERIALIZER_EXTEND(InventoryChange,
		v.dest,
		v.arg
		)

} // namespace Net