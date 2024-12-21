#pragma once
#include "Packet.h"

class EXPORT_API C_EnterToLobby : public Packet
{
public:
    C_EnterToLobby() = default;
    

    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
};

class EXPORT_API S_EnterToLobby : public Packet
{
public:
    S_EnterToLobby() = default;

    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
};

class EXPORT_API C_EnterToMatching : public Packet
{
public:
    C_EnterToMatching() = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
};

class EXPORT_API S_EnterToMatching : public Packet
{
public:
    S_EnterToMatching() = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _matchingQueueId;
    uint32_t _matchingQueueTurn;
};

class EXPORT_API S_MatchingFinished : public Packet
{
public:
    S_MatchingFinished() = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _matchingQueueId;
    uint32_t _matchingQueueTurn;
};

class EXPORT_API C_MatchingCancel : public Packet
{
public:
    C_MatchingCancel() = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _matchingQueueId;
};

class EXPORT_API S_MatchingCancel : public Packet
{
public:
    S_MatchingCancel() = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _matchingQueueId;
    uint8_t _isSuccess;
};

class EXPORT_API C_JoinGame : public Packet
{
public:
    C_JoinGame() = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _matchingQueueId;
};

class EXPORT_API S_JoinGame : public Packet
{
public:
    S_JoinGame() = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _matchingQueueId;
    uint32_t _inGameId;
};

class  C_SignIn : public Packet
{
public:
    EXPORT_API C_SignIn() = default;
    EXPORT_API virtual BYTE* Serialize(BYTE* buffer) override;
    EXPORT_API virtual void Deserialize(BYTE* buffer, int32_t len) override;
    EXPORT_API virtual uint16_t GetSize() override;

    uint32_t _userId;
    std::string _username;
    std::string _password;
};

class S_SignIn : public Packet
{
public:
    EXPORT_API S_SignIn() = default;
    EXPORT_API virtual BYTE* Serialize(BYTE* buffer) override;
    EXPORT_API virtual void Deserialize(BYTE* buffer, int32_t len) override;
    EXPORT_API virtual uint16_t GetSize() override;

    uint32_t _userId;
    std::string _username;
    std::string _password;
    uint8_t _isSuccess;
    std::string _errorReason;
};

class C_Login : public Packet
{
public:
    EXPORT_API C_Login() = default;
    EXPORT_API virtual BYTE* Serialize(BYTE* buffer) override;
    EXPORT_API virtual void Deserialize(BYTE* buffer, int32_t len) override;
    EXPORT_API virtual uint16_t GetSize() override;

    uint32_t _userId;
    std::string _username;
    std::string _password;
};

class S_Login : public Packet
{
public:
    
    EXPORT_API S_Login() =default;
    EXPORT_API virtual BYTE* Serialize(BYTE* buffer) override;
    EXPORT_API virtual void Deserialize(BYTE* buffer, int32_t len) override;
    EXPORT_API virtual uint16_t GetSize() override;

    uint32_t _userId;
    std::string _username;
    std::string _password;
    uint8_t _isSuccess;
    std::string _errorReason;
};

class EXPORT_API C_GetMyAwardInfo : public Packet
{
public:
    
    C_GetMyAwardInfo() =default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
};

class EXPORT_API S_GetMyAwardInfo : public Packet
{
public:
    
    S_GetMyAwardInfo() =default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _awardCnt;
    uint32_t* _awardIdArr;
    std::string* _awardNameArr;
    std::string* _awardInfoArr;
};

class EXPORT_API C_SelectCharacter :  public Packet
{
public:
    
    C_SelectCharacter() =default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _characterId;
};

class  S_SelectCharacter : public Packet
{
public:
    
    EXPORT_API S_SelectCharacter() =default;
    EXPORT_API virtual BYTE* Serialize(BYTE* buffer) override;
    EXPORT_API virtual void Deserialize(BYTE* buffer, int32_t len) override;
    EXPORT_API virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _characterId;
    uint8_t _isSuccess;
    std::string _errorReason;
};

class EXPORT_API C_SelectWeapon : public Packet
{
public:
    
    C_SelectWeapon() = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _weaponId;
};

class  S_SelectWeapon : public Packet
{
public:
    
    EXPORT_API S_SelectWeapon() = default;
    EXPORT_API virtual BYTE* Serialize(BYTE* buffer) override;
    EXPORT_API virtual void Deserialize(BYTE* buffer, int32_t len) override;
    EXPORT_API virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _weaponId;
    uint8_t _isSuccess;
    std::string _errorReason;
};

class EXPORT_API C_BuyWeapon : public Packet
{
public:
    
    C_BuyWeapon() = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _weaponId;
    
};

class S_BuyWeapon : public Packet
{
public:
    
    EXPORT_API S_BuyWeapon() = default;
    EXPORT_API virtual BYTE* Serialize(BYTE* buffer) override;
    EXPORT_API virtual void Deserialize(BYTE* buffer, int32_t len) override;
    EXPORT_API virtual uint16_t GetSize() override;


    uint32_t _userId;
    uint32_t _weaponId;
    uint8_t _isSuccess;
    std::string _errorReason;
};

class EXPORT_API C_CacheFulling : public Packet
{
public:
    
    C_CacheFulling() = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _cacheAmount;
    
};

class  S_CacheFulling : public Packet
{
public:
    
    EXPORT_API S_CacheFulling() = default;
    EXPORT_API virtual BYTE* Serialize(BYTE* buffer) override;
    EXPORT_API virtual void Deserialize(BYTE* buffer, int32_t len) override;
    EXPORT_API virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _cacheAmount;
    uint8_t _isSuccess;
    std::string _errorReason;
};
