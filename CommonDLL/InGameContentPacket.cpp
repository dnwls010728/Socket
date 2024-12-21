#include "pch.h"
#include "InGameContentPacket.h"
#include "Serializer.h"
#include "DeSerializer.h"

// C_ActorAttack
BYTE* C_ActorAttack::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _targetId, currentByte);
    GSerializer->Serialize(buffer, _damage, currentByte);
    return buffer;
}

void C_ActorAttack::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _targetId);
    GDeSerializer->Deserialize(buffer, currentByte, _damage);
}

uint16_t C_ActorAttack::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_targetId) +
           GSerializer->GetPacketSize(_damage);
}

// S_ActorAttack
BYTE* S_ActorAttack::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _targetId, currentByte);
    GSerializer->Serialize(buffer, _currentHp, currentByte);
    return buffer;
}

void S_ActorAttack::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _targetId);
    GDeSerializer->Deserialize(buffer, currentByte, _currentHp);
}

uint16_t S_ActorAttack::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_targetId) +
           GSerializer->GetPacketSize(_currentHp);
}

// C_ActorMove
BYTE* C_ActorMove::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _targetId, currentByte);
    GSerializer->Serialize(buffer, _locationX, currentByte);
    GSerializer->Serialize(buffer, _locationY, currentByte);
    return buffer;
}

void C_ActorMove::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _targetId);
    GDeSerializer->Deserialize(buffer, currentByte, _locationX);
    GDeSerializer->Deserialize(buffer, currentByte, _locationY);
}

uint16_t C_ActorMove::GetSize()
{
    return GSerializer->GetPacketSize(_targetId) +
           GSerializer->GetPacketSize(_locationX) +
           GSerializer->GetPacketSize(_locationY);
}

// S_ActorMove
BYTE* S_ActorMove::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _targetId, currentByte);
    GSerializer->Serialize(buffer, _locationX, currentByte);
    GSerializer->Serialize(buffer, _locationY, currentByte);
    return buffer;
}

void S_ActorMove::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _targetId);
    GDeSerializer->Deserialize(buffer, currentByte, _locationX);
    GDeSerializer->Deserialize(buffer, currentByte, _locationY);
}

uint16_t S_ActorMove::GetSize()
{
    return GSerializer->GetPacketSize(_targetId) +
           GSerializer->GetPacketSize(_locationX) +
           GSerializer->GetPacketSize(_locationY);
}

// C_ActorAppear
BYTE* C_ActorAppear::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _targetId, currentByte);
    GSerializer->Serialize(buffer, _locationX, currentByte);
    GSerializer->Serialize(buffer, _locationY, currentByte);
    return buffer;
}

void C_ActorAppear::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _targetId);
    GDeSerializer->Deserialize(buffer, currentByte, _locationX);
    GDeSerializer->Deserialize(buffer, currentByte, _locationY);
}

uint16_t C_ActorAppear::GetSize()
{
    return GSerializer->GetPacketSize(_targetId) +
           GSerializer->GetPacketSize(_locationX) +
           GSerializer->GetPacketSize(_locationY);
}

// S_ActorAppear
BYTE* S_ActorAppear::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _targetId, currentByte);
    GSerializer->Serialize(buffer, _locationX, currentByte);
    GSerializer->Serialize(buffer, _locationY, currentByte);
    return buffer;
}

void S_ActorAppear::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _targetId);
    GDeSerializer->Deserialize(buffer, currentByte, _locationX);
    GDeSerializer->Deserialize(buffer, currentByte, _locationY);
}

uint16_t S_ActorAppear::GetSize()
{
    return GSerializer->GetPacketSize(_targetId) +
           GSerializer->GetPacketSize(_locationX) +
           GSerializer->GetPacketSize(_locationY);
}

// C_ActorDisappear
BYTE* C_ActorDisappear::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _targetId, currentByte);
    return buffer;
}

void C_ActorDisappear::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _targetId);
}

uint16_t C_ActorDisappear::GetSize()
{
    return GSerializer->GetPacketSize(_targetId);
}

// S_ActorDisappear
BYTE* S_ActorDisappear::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _targetId, currentByte);
    return buffer;
}

void S_ActorDisappear::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _targetId);
}

uint16_t S_ActorDisappear::GetSize()
{
    return GSerializer->GetPacketSize(_targetId);
}

BYTE* C_GetItems::Serialize(BYTE* buffer)
{
    int currentByte=0;
    GSerializer->Serialize(buffer,_userId,currentByte);
    GSerializer->Serialize(buffer,_itemSize,currentByte);
    GSerializer->Serialize(buffer,_itemIdArr,_itemSize,currentByte);
    return buffer;
}

void C_GetItems::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _itemSize);

    _itemIdArr = new uint32_t[_itemSize];
    GDeSerializer->Deserialize(buffer, currentByte, _itemIdArr);
    
}

uint16_t C_GetItems::GetSize()
{
    return GSerializer->GetPacketSize(_userId)+
        GSerializer->GetPacketSize(_itemSize)+
            GSerializer->GetPacketSize(_itemIdArr,_itemSize);
}

BYTE* S_GetItems::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _itemSize, currentByte);
    GSerializer->Serialize(buffer, _itemIdArr, _itemSize, currentByte);
    return buffer;
}

void S_GetItems::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _itemSize);

    _itemIdArr = new uint32_t[_itemSize];
    GDeSerializer->Deserialize(buffer, currentByte, _itemIdArr);
}

uint16_t S_GetItems::GetSize()
{
    return GSerializer->GetPacketSize(_userId)+
        GSerializer->GetPacketSize(_itemSize)+
            GSerializer->GetPacketSize(_itemIdArr,_itemSize);
}

// C_UseItem
BYTE* C_UseItem::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _itemId, currentByte);
    return buffer;
}

void C_UseItem::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _itemId);
}

uint16_t C_UseItem::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_itemId);
}

// S_UseItem
BYTE* S_UseItem::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _itemId, currentByte);
    GSerializer->Serialize(buffer, isSuccess, currentByte);
    GSerializer->Serialize(buffer, errorMsg, currentByte);
    return buffer;
}

void S_UseItem::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _itemId);
    GDeSerializer->Deserialize(buffer, currentByte, isSuccess);
    GDeSerializer->Deserialize(buffer, currentByte, errorMsg);
}

uint16_t S_UseItem::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_itemId) +
           GSerializer->GetPacketSize(isSuccess) +
           GSerializer->GetPacketSize(errorMsg);
}

// S_IsClear
BYTE* S_IsClear::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _isClear, currentByte);
    return buffer;
}

void S_IsClear::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _isClear);
}

uint16_t S_IsClear::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_isClear);
}
