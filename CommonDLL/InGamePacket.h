#pragma once
#include "Packet.h"

class EXPORT_API C_EnterToLobby: public Packet
{
public:
    C_EnterToLobby() =default;
    ~C_EnterToLobby() override =default;

    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
};

class EXPORT_API S_EnterToLobby: public Packet
{
    S_EnterToLobby() =default;
    ~S_EnterToLobby() override =default;

    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
};

class EXPORT_API C_EnterToMatching: public Packet
{
    C_EnterToMatching() =default;
    ~C_EnterToMatching() override =default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
};

class  EXPORT_API S_EnterToMatching: public Packet
{
    S_EnterToMatching() =default;
    ~S_EnterToMatching() override =default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _matchingQueueId;
    uint32_t _matchingQueueTurn;
};

class EXPORT_API S_MatchingFinished: public Packet
{
    S_MatchingFinished() =default;
    ~S_MatchingFinished() override =default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _matchingQueueId;
    uint32_t _matchingQueueTurn;
};

class EXPORT_API C_MatchingCancle : public Packet
{
    C_MatchingCancle() =default;
    ~C_MatchingCancle() override =default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _matchingQueueId;
};

class EXPORT_API S_MatchingCancle : public Packet
{
    S_MatchingCancle() =default;
    ~S_MatchingCancle() override =default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _matchingQueueId;
    uint8_t _isSuccess;
};

class EXPORT_API C_JoinGame : public Packet
{
    C_JoinGame() =default;
    ~C_JoinGame() override =default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _matchingQueueId;
};

class EXPORT_API S_JoinGame : public Packet
{
    S_JoinGame() =default;
    ~S_JoinGame() override =default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _matchingQueueId;
    uint32_t _inGameId;
};

class EXPORT_API C_SignIn : public Packet
{
    C_SignIn() =default;
    ~C_SignIn() override =default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    std::string _username;
    std::string _password;
};

class EXPORT_API S_SignIn : public Packet
{
    S_SignIn() =default;
    ~S_SignIn() override =default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    std::string _username;
    std::string _password;
    uint8_t _isSuccess;
    std::string _errorReason;
};

class EXPORT_API C_Login:public Packet
{
    C_Login() =default;
    ~C_Login() override =default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    std::string _username;
    std::string _password;
};

class EXPORT_API S_Login : public Packet
{
    S_Login() =default;
    ~S_Login() override =default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    std::string _username;
    std::string _password;
    uint8_t _isSuccess;
    std::string _errorReason;
};

class EXPORT_API C_GetMyAwardInfo : public Packet
{
    C_GetMyAwardInfo() =default;
    ~C_GetMyAwardInfo() override =default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
};

class EXPORT_API S_GetMyAwardInfo : public Packet
{
    S_GetMyAwardInfo() =default;
    ~S_GetMyAwardInfo() override =default;
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
    C_SelectCharacter() =default;
    ~C_SelectCharacter() override =default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _characterId;
};

class EXPORT_API S_SelectCharacter : public Packet
{
    S_SelectCharacter() =default;
    ~S_SelectCharacter() override =default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _characterId;
    uint8_t _isSuccess;
    std::string _errorReason;
};

class EXPORT_API C_SelectWeapon : public Packet
{
    C_SelectWeapon() = default;
    ~C_SelectWeapon() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _weaponId;
};

class EXPORT_API S_SelectWeapon : public Packet
{
    S_SelectWeapon() = default;
    ~S_SelectWeapon() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _weaponId;
    uint8_t _isSuccess;
    std::string _errorReason;
};

class EXPORT_API C_BuyWeapon : public Packet
{
    C_BuyWeapon() = default;
    ~C_BuyWeapon() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _weaponId;
    
};

class EXPORT_API S_BuyWeapon : public Packet
{
    S_BuyWeapon() = default;
    ~S_BuyWeapon() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;


    uint32_t _userId;
    uint32_t _weaponId;
    uint8_t _isSuccess;
    std::string _errorReason;
};

class EXPORT_API C_CacheFulling : public Packet
{
    C_CacheFulling() = default;
    ~C_CacheFulling() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _cacheAmount;
    
};

class EXPORT_API S_CacheFulling : public Packet
{
    S_CacheFulling() = default;
    ~S_CacheFulling() override = default;
    virtual BYTE* Serialize(BYTE* buffer) override;
    virtual void Deserialize(BYTE* buffer, int32_t len) override;
    virtual uint16_t GetSize() override;

    uint32_t _userId;
    uint32_t _cacheAmount;
    uint8_t _isSuccess;
    std::string _errorReason;
};
