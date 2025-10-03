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
                v.slot_id,
                v.count
                )

        SERIALIZER_EXTEND(SkillInfo,
                v.skill_id,
                v.level,
                v.cooldown
                )

    SERIALIZER_EXTEND(PartyMemberInfo,
        v.character_id,
        v.name,
        v.body_color,
        v.lv,
        v.hp,
        v.max_hp
        )

	SERIALIZER_EXTEND(CardSelectInfo,
		v.card_id,
		v.level,
		v.offer_id,
		v.offered_at,
		v.slot
		)

} // namespace Net