#pragma once
#include "Misc/EnumClassFlags.h"

enum class ActorLayer : uint16_t
{
    kDefault = (0x01<<0),
    kPlayer = (0x01<<1),
    kNPC = (0x01<<2),
    kMob = (0x01<<3),
    kItemDrop = (0x01<<4)
};

#define ActorLayer_FLAGS(ActorLayer) 
inline constexpr ActorLayer operator|(ActorLayer lhs, ActorLayer rhs) 
{ 
using T = std::underlying_type_t<ActorLayer>; 
return static_cast<ActorLayer>(static_cast<T>(lhs) | static_cast<T>(rhs)); 
} 

inline constexpr ActorLayer operator&(ActorLayer lhs, ActorLayer rhs) 
{ 
using T = std::underlying_type_t<ActorLayer>; 
return static_cast<ActorLayer>(static_cast<T>(lhs) & static_cast<T>(rhs)); 
} 

inline constexpr ActorLayer operator^(ActorLayer lhs, ActorLayer rhs) 
{ 
using T = std::underlying_type_t<ActorLayer>; 
return static_cast<ActorLayer>(static_cast<T>(lhs) ^ static_cast<T>(rhs)); 
} 

inline constexpr ActorLayer operator~(ActorLayer rhs) 
{ 
using T = std::underlying_type_t<ActorLayer>; 
return static_cast<ActorLayer>(~static_cast<T>(rhs)); 
} 

inline ActorLayer& operator|=(ActorLayer& lhs, ActorLayer rhs) 
{ 
using T = std::underlying_type_t<ActorLayer>; 
lhs = static_cast<ActorLayer>(static_cast<T>(lhs) | static_cast<T>(rhs)); 
return lhs; 
} 

inline ActorLayer& operator&=(ActorLayer& lhs, ActorLayer rhs) 
{ 
using T = std::underlying_type_t<ActorLayer>; 
lhs = static_cast<ActorLayer>(static_cast<T>(lhs) & static_cast<T>(rhs)); 
return lhs; 
} 

inline ActorLayer& operator^=(ActorLayer& lhs, ActorLayer rhs) 
{ 
using T = std::underlying_type_t<ActorLayer>; 
lhs = static_cast<ActorLayer>(static_cast<T>(lhs) ^ static_cast<T>(rhs)); 
return lhs; 
} 

inline bool operator!(ActorLayer rhs) 
{ 
using T = std::underlying_type_t<ActorLayer>; 
return !static_cast<T>(rhs); 
}