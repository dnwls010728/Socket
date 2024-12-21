#pragma once
#include "Packet.h"

class EXPORT_API C_ActorAttack : public Packet
{
public:
    C_ActorAttack() = default;
    ~C_ActorAttack() override = default;

    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _targetId;
    uint32_t _damage;
};

class EXPORT_API S_ActorAttack : public Packet
{
public:
    S_ActorAttack() = default;
    ~S_ActorAttack() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _targetId;
    uint32_t _currentHp;
};

class EXPORT_API C_ActorMove : public Packet
{
public:
    C_ActorMove() = default;
    ~C_ActorMove() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _targetId;
    float _locationX;
    float _locationY;
    
};

class EXPORT_API S_ActorMove : public Packet
{
public:
    S_ActorMove() = default;
    ~S_ActorMove() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _targetId;
    float _locationX;
    float _locationY;
};

class EXPORT_API C_ActorAppear : public Packet
{
public:
    
    C_ActorAppear() = default;
    ~C_ActorAppear() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _targetId;
    float _locationX;
    float _locationY;
    
};

class EXPORT_API S_ActorAppear : public Packet
{
public:
    S_ActorAppear() = default;
    ~S_ActorAppear() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _targetId;
    float _locationX;
    float _locationY;
};

class EXPORT_API C_ActorDisappear : public Packet
{
public:
    C_ActorDisappear() = default;
    ~C_ActorDisappear() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _targetId;
};

class EXPORT_API S_ActorDisappear : public Packet
{
public:
    S_ActorDisappear() = default;
    ~S_ActorDisappear() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _targetId;
};

class EXPORT_API C_GetItems: public Packet
{
public:
    C_GetItems() = default;
    ~C_GetItems() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _itemSize;

    uint32_t* _itemIdArr;
};

class EXPORT_API S_GetItems: public Packet
{
public:
    S_GetItems() = default;
    ~S_GetItems() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _itemSize;
    uint32_t* _itemIdArr;
};

class EXPORT_API C_UseItem: public Packet
{
public:
    
    C_UseItem() = default;
    ~C_UseItem() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _itemId;
};

class EXPORT_API S_UseItem: public Packet
{
public:
    S_UseItem() = default;
    ~S_UseItem() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _itemId;
    uint8_t isSuccess;
    std::string errorMsg;
};

class EXPORT_API S_IsClear : public Packet
{
public:
    S_IsClear() = default;
    ~S_IsClear() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint8_t _isClear;
    
};