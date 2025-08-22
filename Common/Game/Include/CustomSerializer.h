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

	SERIALIZER_EXTEND(CharacterProfile,
		v.character_id,
		v.map_id,
		v.name,
		v.position.x,
		v.position.y,
		v.body_color,
		v.stats
		)

	SERIALIZER_EXTEND(ItemInfo,
		v.inventory_type,
		v.item_id,
		v.slot_index,
		v.count
		)

    SERIALIZER_EXTEND(PartyMemberInfo,
        v.player_id,
        v.name,
        v.lv,
        v.hp,
        v.max_hp
        )

} // namespace Net