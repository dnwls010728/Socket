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
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, is_success, message)
    REGISTER_PACKET(SelectCharacterResponse, 207)
};

// 맵이 변경되었을 때
struct ChangeMapPacket : public Net::IPacket
{
    uint32_t map_id;
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, map_id)
    REGISTER_PACKET(ChangeMapPacket, 208)
};

// 채팅 메시지를 보낼 때
struct ChatMessagePacket : public Net::IPacket
{
    std::wstring message;
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, message)
    REGISTER_PACKET(ChatMessagePacket, 209)
};

// 채팅 메시지를 받을 때
struct ChatMessageReceivePacket : public Net::IPacket
{
    std::wstring message;
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, message)
    REGISTER_PACKET(ChatMessageReceivePacket, 210)
};
