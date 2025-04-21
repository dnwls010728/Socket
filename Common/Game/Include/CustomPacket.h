#pragma once
#include "IPacket.h"
#include "Serializer.h"
#include "CustomSerializer.h"

// TODO : 오류코드 추가해서 클라이언트에서 오류 메시지 출력.......

// 메시지 패킷
struct MessagePacket : public Net::IPacket
{
    std::string message;
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, message)
    REGISTER_PACKET(MessagePacket, 100)
};

// 회원가입 요청
struct RegisterRequest : public Net::IPacket
{
    std::wstring id;
    std::wstring password;
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, id, password)
    REGISTER_PACKET(RegisterRequest, 200)
};

// 회원가입 응답
struct RegisterResponse : public Net::IPacket
{
    bool is_success;
    std::wstring message;
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, is_success, message)
    REGISTER_PACKET(RegisterResponse, 201)
};

// 로그인 요청
struct LoginRequest : public Net::IPacket
{
    std::wstring id;
    std::wstring password;
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, id, password)
    REGISTER_PACKET(LoginRequest, 202)
};

// 로그인 응답
struct LoginResponse : public Net::IPacket
{
    bool is_success;
    std::wstring message;
    std::vector<CharacterInfo> characters;
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, is_success, message, characters)
    REGISTER_PACKET(LoginResponse, 203)
};

// 캐릭터 생성 요청
// 캐릭터 생성 응답

// 캐릭터 선택 요청
struct SelectCharacterRequest : public Net::IPacket
{
    uint32_t unique_id;
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, unique_id)
    REGISTER_PACKET(SelectCharacterRequest, 206)
};

// 캐릭터 선택 응답
struct SelectCharacterResponse : public Net::IPacket
{
    bool is_success;
    std::wstring message;
    CharacterInfo character_info;
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, is_success, message, character_info)
    REGISTER_PACKET(SelectCharacterResponse, 207)
};

struct MapLoadCompletePacket : public Net::IPacket
{
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer)
    REGISTER_PACKET(MapLoadCompletePacket, 208)
};

// 플레이어 스폰 패킷
struct SpawnPlayerPacket : public Net::IPacket
{
    CharacterInfo character_info;
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, character_info)
    REGISTER_PACKET(SpawnPlayerPacket, 209)
};

// 플레이어 파괴 패킷
struct DestroyPlayerPacket : public Net::IPacket
{
    uint32_t unique_id;
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, unique_id)
    REGISTER_PACKET(DestroyPlayerPacket, 210)
};

struct MovePlayerPacket : public Net::IPacket
{
    uint32_t unique_id;
    float x;
    float y;
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, unique_id, x, y)
    REGISTER_PACKET(MovePlayerPacket, 211)
};
