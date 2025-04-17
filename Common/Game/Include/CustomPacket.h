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
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, is_success, message)
    REGISTER_PACKET(LoginResponse, 203)
};
