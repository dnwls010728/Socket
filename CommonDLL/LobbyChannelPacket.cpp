#include "pch.h"
#include "LobbyChannelPacket.h"
#include "Serializer.h"
#include "DeSerializer.h"

// C_EnterToLobby
BYTE* C_EnterToLobby::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    return buffer;
}

void C_EnterToLobby::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
}

uint16_t C_EnterToLobby::GetSize()
{
    return Serializer::GetPacketSize(_userId);
}

// S_EnterToLobby
BYTE* S_EnterToLobby::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    return buffer;
}

void S_EnterToLobby::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
}

uint16_t S_EnterToLobby::GetSize()
{
    return Serializer::GetPacketSize(_userId);
}

BYTE* C_EnterToMatching::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    return buffer;
}

void C_EnterToMatching::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    
}

uint16_t C_EnterToMatching::GetSize()
{
    return Serializer::GetPacketSize(_userId);
}

// S_EnterToMatching
BYTE* S_EnterToMatching::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _matchingQueueId, currentByte);
    Serializer::Serialize(buffer, _matchingQueueTurn, currentByte);
    return buffer;
}

void S_EnterToMatching::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _matchingQueueId);
    DeSerializer::Deserialize(buffer, currentByte, _matchingQueueTurn);
}

uint16_t S_EnterToMatching::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
           Serializer::GetPacketSize(_matchingQueueId) +
           Serializer::GetPacketSize(_matchingQueueTurn);
}

BYTE* S_MatchingFinished::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _matchingQueueId, currentByte);
    Serializer::Serialize(buffer, _matchingQueueTurn, currentByte);
    return buffer;
}

void S_MatchingFinished::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte=0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _matchingQueueId);
    DeSerializer::Deserialize(buffer, currentByte, _matchingQueueTurn);
}

uint16_t S_MatchingFinished::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
        Serializer::GetPacketSize(_matchingQueueId) +
           Serializer::GetPacketSize(_matchingQueueTurn);
}

// Example: S_SignIn
BYTE* S_SignIn::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _username, currentByte);
    Serializer::Serialize(buffer, _password, currentByte);
    Serializer::Serialize(buffer, _isSuccess, currentByte);
    Serializer::Serialize(buffer, _errorReason, currentByte);
    return buffer;
}

void S_SignIn::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _username);
    DeSerializer::Deserialize(buffer, currentByte, _password);
    DeSerializer::Deserialize(buffer, currentByte, _isSuccess);
    DeSerializer::Deserialize(buffer, currentByte, _errorReason);
}

uint16_t S_SignIn::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
           Serializer::GetPacketSize(_username) +
           Serializer::GetPacketSize(_password) +
           Serializer::GetPacketSize(_isSuccess) +
           Serializer::GetPacketSize(_errorReason);
}

BYTE* C_Login::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _username, currentByte);
    Serializer::Serialize(buffer, _password, currentByte);
    return buffer;
    
}

void C_Login::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _username);
    DeSerializer::Deserialize(buffer, currentByte, _password);
    
}

uint16_t C_Login::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
        Serializer::GetPacketSize(_username)+
        Serializer::GetPacketSize(_password);
    
}

BYTE* S_Login::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _username, currentByte);
    Serializer::Serialize(buffer, _password, currentByte);
    Serializer::Serialize(buffer, _isSuccess, currentByte);
    Serializer::Serialize(buffer, _errorReason, currentByte);
    return buffer;
}

void S_Login::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte=0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _username);
    DeSerializer::Deserialize(buffer, currentByte, _password);
    DeSerializer::Deserialize(buffer, currentByte, _isSuccess);
    DeSerializer::Deserialize(buffer, currentByte, _errorReason);
    
}

uint16_t S_Login::GetSize()
{
    return Serializer::GetPacketSize(_userId)+
        Serializer::GetPacketSize(_username)+
            Serializer::GetPacketSize(_password)+
                Serializer::GetPacketSize(_isSuccess)+
                    Serializer::GetPacketSize(_errorReason);
}

BYTE* C_GetMyAwardInfo::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    return buffer;
}

void C_GetMyAwardInfo::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
}

uint16_t C_GetMyAwardInfo::GetSize()
{
    return Serializer::GetPacketSize(_userId);
}

BYTE* S_GetMyAwardInfo::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer,_awardCnt,currentByte);
    Serializer::Serialize(buffer,_awardIdArr,_awardCnt,currentByte);
    Serializer::Serialize(buffer,_awardNameArr,_awardCnt,currentByte);
    Serializer::Serialize(buffer,_awardInfoArr,_awardCnt,currentByte);
    return buffer;
}

void S_GetMyAwardInfo::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer,currentByte,_userId);
    DeSerializer::Deserialize(buffer,currentByte,_awardCnt);

    _awardNameArr = new std::string[_awardCnt];
    _awardInfoArr = new std::string[_awardCnt];
    _awardIdArr = new uint32_t[_awardCnt];

    DeSerializer::Deserialize(buffer,currentByte,_awardIdArr);
    DeSerializer::Deserialize(buffer,currentByte,_awardNameArr);
    DeSerializer::Deserialize(buffer,currentByte,_awardInfoArr);
    
}

uint16_t S_GetMyAwardInfo::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
        Serializer::GetPacketSize(_awardCnt) +
            Serializer::GetPacketSize(_awardIdArr,_awardCnt)+
                Serializer::GetPacketSize(_awardNameArr,_awardCnt)+
                    Serializer::GetPacketSize(_awardInfoArr,_awardCnt);
}

// C_MatchingCancel
BYTE* C_MatchingCancel::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _matchingQueueId, currentByte);
    return buffer;
}

void C_MatchingCancel::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _matchingQueueId);
}

uint16_t C_MatchingCancel::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
           Serializer::GetPacketSize(_matchingQueueId);
}

// S_MatchingCancel
BYTE* S_MatchingCancel::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _matchingQueueId, currentByte);
    Serializer::Serialize(buffer, _isSuccess, currentByte);
    return buffer;
}

void S_MatchingCancel::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _matchingQueueId);
    DeSerializer::Deserialize(buffer, currentByte, _isSuccess);
}

uint16_t S_MatchingCancel::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
           Serializer::GetPacketSize(_matchingQueueId) +
           Serializer::GetPacketSize(_isSuccess);
}

// C_JoinGame
BYTE* C_JoinGame::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _matchingQueueId, currentByte);
    return buffer;
}

void C_JoinGame::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _matchingQueueId);
}

uint16_t C_JoinGame::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
           Serializer::GetPacketSize(_matchingQueueId);
}

// S_JoinGame
BYTE* S_JoinGame::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _matchingQueueId, currentByte);
    Serializer::Serialize(buffer, _inGameId, currentByte);
    return buffer;
}

void S_JoinGame::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _matchingQueueId);
    DeSerializer::Deserialize(buffer, currentByte, _inGameId);
}

uint16_t S_JoinGame::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
           Serializer::GetPacketSize(_matchingQueueId) +
           Serializer::GetPacketSize(_inGameId);
}

BYTE* C_SignIn::Serialize(BYTE* buffer)
{
    int currentByte=0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _username, currentByte);
    Serializer::Serialize(buffer, _password, currentByte);
    return buffer;
}

void C_SignIn::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte=0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _username);
    DeSerializer::Deserialize(buffer, currentByte, _password);
}

uint16_t C_SignIn::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
    +Serializer::GetPacketSize(_username) +
        Serializer::GetPacketSize(_password);
}

// C_SelectCharacter
BYTE* C_SelectCharacter::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _characterId, currentByte);
    return buffer;
}

void C_SelectCharacter::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _characterId);
}

uint16_t C_SelectCharacter::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
           Serializer::GetPacketSize(_characterId);
}

// S_SelectCharacter
BYTE* S_SelectCharacter::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _characterId, currentByte);
    Serializer::Serialize(buffer, _isSuccess, currentByte);
    Serializer::Serialize(buffer, _errorReason, currentByte);
    return buffer;
}

void S_SelectCharacter::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _characterId);
    DeSerializer::Deserialize(buffer, currentByte, _isSuccess);
    DeSerializer::Deserialize(buffer, currentByte, _errorReason);
}

uint16_t S_SelectCharacter::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
           Serializer::GetPacketSize(_characterId) +
           Serializer::GetPacketSize(_isSuccess) +
           Serializer::GetPacketSize(_errorReason);
}

// C_SelectWeapon
BYTE* C_SelectWeapon::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _weaponId, currentByte);
    return buffer;
}

void C_SelectWeapon::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _weaponId);
}

uint16_t C_SelectWeapon::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
           Serializer::GetPacketSize(_weaponId);
}

// S_SelectWeapon
BYTE* S_SelectWeapon::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _weaponId, currentByte);
    Serializer::Serialize(buffer, _isSuccess, currentByte);
    Serializer::Serialize(buffer, _errorReason, currentByte);
    return buffer;
}

void S_SelectWeapon::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _weaponId);
    DeSerializer::Deserialize(buffer, currentByte, _isSuccess);
    DeSerializer::Deserialize(buffer, currentByte, _errorReason);
}

uint16_t S_SelectWeapon::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
           Serializer::GetPacketSize(_weaponId) +
           Serializer::GetPacketSize(_isSuccess) +
           Serializer::GetPacketSize(_errorReason);
}

// C_BuyWeapon
BYTE* C_BuyWeapon::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _weaponId, currentByte);
    return buffer;
}

void C_BuyWeapon::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _weaponId);
}

uint16_t C_BuyWeapon::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
           Serializer::GetPacketSize(_weaponId);
}

// S_BuyWeapon
BYTE* S_BuyWeapon::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _weaponId, currentByte);
    Serializer::Serialize(buffer, _isSuccess, currentByte);
    Serializer::Serialize(buffer, _errorReason, currentByte);
    return buffer;
}

void S_BuyWeapon::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _weaponId);
    DeSerializer::Deserialize(buffer, currentByte, _isSuccess);
    DeSerializer::Deserialize(buffer, currentByte, _errorReason);
}

uint16_t S_BuyWeapon::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
           Serializer::GetPacketSize(_weaponId) +
           Serializer::GetPacketSize(_isSuccess) +
           Serializer::GetPacketSize(_errorReason);
}

// C_CacheFulling
BYTE* C_CacheFulling::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _cacheAmount, currentByte);
    return buffer;
}

void C_CacheFulling::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _cacheAmount);
}

uint16_t C_CacheFulling::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
           Serializer::GetPacketSize(_cacheAmount);
}

// S_CacheFulling
BYTE* S_CacheFulling::Serialize(BYTE* buffer)
{
    int currentByte = 0;
    Serializer::Serialize(buffer, _userId, currentByte);
    Serializer::Serialize(buffer, _cacheAmount, currentByte);
    Serializer::Serialize(buffer, _isSuccess, currentByte);
    Serializer::Serialize(buffer, _errorReason, currentByte);
    return buffer;
}

void S_CacheFulling::Deserialize(BYTE* buffer, int32_t len)
{
    int currentByte = 0;
    DeSerializer::Deserialize(buffer, currentByte, _userId);
    DeSerializer::Deserialize(buffer, currentByte, _cacheAmount);
    DeSerializer::Deserialize(buffer, currentByte, _isSuccess);
    DeSerializer::Deserialize(buffer, currentByte, _errorReason);
}

uint16_t S_CacheFulling::GetSize()
{
    return Serializer::GetPacketSize(_userId) +
           Serializer::GetPacketSize(_cacheAmount) +
           Serializer::GetPacketSize(_isSuccess) +
           Serializer::GetPacketSize(_errorReason);
}