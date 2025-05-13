#pragma once
#include "Serializer.h"
#include "CommonObject.h"

namespace Net {

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
	
// Movement
SERIALIZER_EXTEND(Movement,v.x,v.y)

// CharacterInfo
SERIALIZER_EXTEND(CharacterInfo,
	v.unique_id,
	v.account_unique_id,
	v.name,
	v.lv,
	v.job,
	v.map_unique_id,
	v.last_position_x,
	v.last_position_y
	)

// ObjectInfo 직렬화/역직렬화 등록
SERIALIZER_EXTEND(ObjectInfo,
	v.name,
	v.type_name,
	v.unique_id,
	v.last_position_x,
	v.last_position_y,
	v.map_unique_id
	)


} // namespace Net