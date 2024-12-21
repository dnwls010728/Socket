#include "pch.h"
#include "LobbyChannelPacket.h"
#include "Serializer.h"
#include "DeSerializer.h"

// C_EnterToLobby
BYTE* C_EnterToLobby::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    return buffer;
}

void C_EnterToLobby::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
}

uint16_t C_EnterToLobby::GetSize()
{
    return GSerializer->GetPacketSize(_userId);
}

// S_EnterToLobby
BYTE* S_EnterToLobby::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    return buffer;
}

void S_EnterToLobby::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
}

uint16_t S_EnterToLobby::GetSize()
{
    return GSerializer->GetPacketSize(_userId);
}

BYTE* C_EnterToMatching::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    return buffer;
}

void C_EnterToMatching::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    
}

uint16_t C_EnterToMatching::GetSize()
{
    return GSerializer->GetPacketSize(_userId);
}

// S_EnterToMatching
BYTE* S_EnterToMatching::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _matchingQueueId, currentByte);
    GSerializer->Serialize(buffer, _matchingQueueTurn, currentByte);
    return buffer;
}

void S_EnterToMatching::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _matchingQueueId);
    GDeSerializer->Deserialize(buffer, currentByte, _matchingQueueTurn);
}

uint16_t S_EnterToMatching::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_matchingQueueId) +
           GSerializer->GetPacketSize(_matchingQueueTurn);
}

BYTE* S_MatchingFinished::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _matchingQueueId, currentByte);
    GSerializer->Serialize(buffer, _matchingQueueTurn, currentByte);
    return buffer;
}

void S_MatchingFinished::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte=0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _matchingQueueId);
    GDeSerializer->Deserialize(buffer, currentByte, _matchingQueueTurn);
}

uint16_t S_MatchingFinished::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
        GSerializer->GetPacketSize(_matchingQueueId) +
           GSerializer->GetPacketSize(_matchingQueueTurn);
}

// Example: S_SignIn
BYTE* S_SignIn::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _username, currentByte);
    GSerializer->Serialize(buffer, _password, currentByte);
    GSerializer->Serialize(buffer, _isSuccess, currentByte);
    GSerializer->Serialize(buffer, _errorReason, currentByte);
    return buffer;
}

void S_SignIn::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _username);
    GDeSerializer->Deserialize(buffer, currentByte, _password);
    GDeSerializer->Deserialize(buffer, currentByte, _isSuccess);
    GDeSerializer->Deserialize(buffer, currentByte, _errorReason);
}

uint16_t S_SignIn::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_username) +
           GSerializer->GetPacketSize(_password) +
           GSerializer->GetPacketSize(_isSuccess) +
           GSerializer->GetPacketSize(_errorReason);
}

BYTE* C_Login::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _username, currentByte);
    GSerializer->Serialize(buffer, _password, currentByte);
    return buffer;
    
}

void C_Login::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _username);
    GDeSerializer->Deserialize(buffer, currentByte, _password);
    
}

uint16_t C_Login::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
        GSerializer->GetPacketSize(_username)+
        GSerializer->GetPacketSize(_password);
    
}

BYTE* S_Login::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _username, currentByte);
    GSerializer->Serialize(buffer, _password, currentByte);
    GSerializer->Serialize(buffer, _isSuccess, currentByte);
    GSerializer->Serialize(buffer, _errorReason, currentByte);
    return buffer;
}

void S_Login::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte=0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _username);
    GDeSerializer->Deserialize(buffer, currentByte, _password);
    GDeSerializer->Deserialize(buffer, currentByte, _isSuccess);
    GDeSerializer->Deserialize(buffer, currentByte, _errorReason);
    
}

uint16_t S_Login::GetSize()
{
    return GSerializer->GetPacketSize(_userId)+
        GSerializer->GetPacketSize(_username)+
            GSerializer->GetPacketSize(_password)+
                GSerializer->GetPacketSize(_isSuccess)+
                    GSerializer->GetPacketSize(_errorReason);
}

BYTE* C_GetMyAwardInfo::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    return buffer;
}

void C_GetMyAwardInfo::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
}

uint16_t C_GetMyAwardInfo::GetSize()
{
    return GSerializer->GetPacketSize(_userId);
}

BYTE* S_GetMyAwardInfo::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer,_awardCnt,currentByte);
    GSerializer->Serialize(buffer,_awardIdArr,static_cast<uint8_t>(_awardCnt),currentByte);
    GSerializer->Serialize(buffer,_awardNameArr,static_cast<uint8_t>(_awardCnt),currentByte);
    GSerializer->Serialize(buffer,_awardInfoArr,static_cast<uint8_t>(_awardCnt),currentByte);
    return buffer;
}

void S_GetMyAwardInfo::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer,currentByte,_userId);
    GDeSerializer->Deserialize(buffer,currentByte,_awardCnt);

    _awardNameArr = new std::string[_awardCnt];
    _awardInfoArr = new std::string[_awardCnt];
    _awardIdArr = new uint32_t[_awardCnt];

    GDeSerializer->Deserialize(buffer,currentByte,_awardIdArr);
    GDeSerializer->Deserialize(buffer,currentByte,_awardNameArr);
    GDeSerializer->Deserialize(buffer,currentByte,_awardInfoArr);
    
}

uint16_t S_GetMyAwardInfo::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
        GSerializer->GetPacketSize(_awardCnt) +
            GSerializer->GetPacketSize(_awardIdArr,_awardCnt)+
                GSerializer->GetPacketSize(_awardNameArr,_awardCnt)+
                    GSerializer->GetPacketSize(_awardInfoArr,_awardCnt);
}

// C_MatchingCancel
BYTE* C_MatchingCancel::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _matchingQueueId, currentByte);
    return buffer;
}

void C_MatchingCancel::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _matchingQueueId);
}

uint16_t C_MatchingCancel::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_matchingQueueId);
}

// S_MatchingCancel
BYTE* S_MatchingCancel::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _matchingQueueId, currentByte);
    GSerializer->Serialize(buffer, _isSuccess, currentByte);
    return buffer;
}

void S_MatchingCancel::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _matchingQueueId);
    GDeSerializer->Deserialize(buffer, currentByte, _isSuccess);
}

uint16_t S_MatchingCancel::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_matchingQueueId) +
           GSerializer->GetPacketSize(_isSuccess);
}

// C_JoinGame
BYTE* C_JoinGame::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _matchingQueueId, currentByte);
    GSerializer->Serialize(buffer,_mapId, currentByte);
    return buffer;
}

void C_JoinGame::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _matchingQueueId);
    GDeSerializer->Deserialize(buffer, currentByte, _mapId);
}

uint16_t C_JoinGame::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_matchingQueueId)+
               GSerializer->GetPacketSize(_mapId);
}

// S_JoinGame
BYTE* S_JoinGame::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _matchingQueueId, currentByte);
    GSerializer->Serialize(buffer, _inGameId, currentByte);
    return buffer;
}

void S_JoinGame::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _matchingQueueId);
    GDeSerializer->Deserialize(buffer, currentByte, _inGameId);
}

uint16_t S_JoinGame::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_matchingQueueId) +
           GSerializer->GetPacketSize(_inGameId);
}

BYTE* C_SignIn::Serialize(BYTE* buffer)
{
    int currentByte=0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _username, currentByte);
    GSerializer->Serialize(buffer, _password, currentByte);
    return buffer;
}

void C_SignIn::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte=0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _username);
    GDeSerializer->Deserialize(buffer, currentByte, _password);
}

uint16_t C_SignIn::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
    +GSerializer->GetPacketSize(_username) +
        GSerializer->GetPacketSize(_password);
}

// C_SelectCharacter
BYTE* C_SelectCharacter::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _characterId, currentByte);
    return buffer;
}

void C_SelectCharacter::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _characterId);
}

uint16_t C_SelectCharacter::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_characterId);
}

// S_SelectCharacter
BYTE* S_SelectCharacter::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _characterId, currentByte);
    GSerializer->Serialize(buffer, _isSuccess, currentByte);
    GSerializer->Serialize(buffer, _errorReason, currentByte);
    return buffer;
}

void S_SelectCharacter::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _characterId);
    GDeSerializer->Deserialize(buffer, currentByte, _isSuccess);
    GDeSerializer->Deserialize(buffer, currentByte, _errorReason);
}

uint16_t S_SelectCharacter::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_characterId) +
           GSerializer->GetPacketSize(_isSuccess) +
           GSerializer->GetPacketSize(_errorReason);
}

// C_SelectWeapon
BYTE* C_SelectWeapon::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _weaponId, currentByte);
    return buffer;
}

void C_SelectWeapon::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _weaponId);
}

uint16_t C_SelectWeapon::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_weaponId);
}

// S_SelectWeapon
BYTE* S_SelectWeapon::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _weaponId, currentByte);
    GSerializer->Serialize(buffer, _isSuccess, currentByte);
    GSerializer->Serialize(buffer, _errorReason, currentByte);
    return buffer;
}

void S_SelectWeapon::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _weaponId);
    GDeSerializer->Deserialize(buffer, currentByte, _isSuccess);
    GDeSerializer->Deserialize(buffer, currentByte, _errorReason);
}

uint16_t S_SelectWeapon::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_weaponId) +
           GSerializer->GetPacketSize(_isSuccess) +
           GSerializer->GetPacketSize(_errorReason);
}

// C_BuyWeapon
BYTE* C_BuyWeapon::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _weaponId, currentByte);
    return buffer;
}

void C_BuyWeapon::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _weaponId);
}

uint16_t C_BuyWeapon::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_weaponId);
}

// S_BuyWeapon
BYTE* S_BuyWeapon::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _weaponId, currentByte);
    GSerializer->Serialize(buffer, _isSuccess, currentByte);
    GSerializer->Serialize(buffer, _errorReason, currentByte);
    return buffer;
}

void S_BuyWeapon::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _weaponId);
    GDeSerializer->Deserialize(buffer, currentByte, _isSuccess);
    GDeSerializer->Deserialize(buffer, currentByte, _errorReason);
}

uint16_t S_BuyWeapon::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_weaponId) +
           GSerializer->GetPacketSize(_isSuccess) +
           GSerializer->GetPacketSize(_errorReason);
}

// C_CacheFulling
BYTE* C_CacheFulling::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _cacheAmount, currentByte);
    return buffer;
}

void C_CacheFulling::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _cacheAmount);
}

uint16_t C_CacheFulling::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_cacheAmount);
}

// S_CacheFulling
BYTE* S_CacheFulling::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    GSerializer->Serialize(buffer, _userId, currentByte);
    GSerializer->Serialize(buffer, _cacheAmount, currentByte);
    GSerializer->Serialize(buffer, _isSuccess, currentByte);
    GSerializer->Serialize(buffer, _errorReason, currentByte);
    return buffer;
}

void S_CacheFulling::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    GDeSerializer->Deserialize(buffer, currentByte, _userId);
    GDeSerializer->Deserialize(buffer, currentByte, _cacheAmount);
    GDeSerializer->Deserialize(buffer, currentByte, _isSuccess);
    GDeSerializer->Deserialize(buffer, currentByte, _errorReason);
}

uint16_t S_CacheFulling::GetSize()
{
    return GSerializer->GetPacketSize(_userId) +
           GSerializer->GetPacketSize(_cacheAmount) +
           GSerializer->GetPacketSize(_isSuccess) +
           GSerializer->GetPacketSize(_errorReason);
}