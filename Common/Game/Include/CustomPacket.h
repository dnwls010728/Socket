#pragma once
#include "IPacket.h"
#include "Serializer.h"
#include "CustomSerializer.h"

// TODO : 오류코드 추가해서 클라이언트에서 오류 메시지 출력.......

// 메시지 패킷
struct MessagePacket : public Net::IPacket
{
    std::string message;
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer,message)
    REGISTER_PACKET(MessagePacket, 100)
};

// 방 목록 요청 패킷 ( 클라 > 서버 )
struct RoomListPacketReq : public Net::IPacket
{
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer)
    REGISTER_PACKET(RoomListPacketReq, 200)
};

// 방 목록 요청 응답 ( 서버 > 클라 )
struct RoomListPacketAck : public Net::IPacket
{
    RoomList room_list; 
	SERIALIZABLE_PACKET_FIELDS(CustomSerializer, room_list)
    REGISTER_PACKET(RoomListPacketAck, 201)
};

// 방 목록 변경 구독 ( 서버 > 클라 )
struct ObserveRoomListPacket : public Net::IPacket
{
    bool is_observe;
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, is_observe)
    REGISTER_PACKET(ObserveRoomListPacket, 202)
};

// 방 목록 자동 동기화 요청 패킷 ( 클라 > 서버 )
struct OnUpdateRoomListPacket : public Net::IPacket
{
	RoomInfo room_info;
    RoomListUpdateType update_type;
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, update_type, room_info)
    REGISTER_PACKET(OnUpdateRoomListPacket, 203)
};

// 방 생성 요청 ( 클라 > 서버 )
struct CreateRoomPacketReq : public Net::IPacket
{
    std::wstring room_title;
    int max_user_count;
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, room_title, max_user_count)
    REGISTER_PACKET(CreateRoomPacketReq, 210)
};

// 방 생성 응답 ( 서버 > 클라 ) result true시 생성한 사람은 바로 입장하면 됨
struct CreateRoomPacketAck : public Net::IPacket
{
    int room_number;
    std::wstring room_title;
    int max_user_count;
	bool result;
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, room_number, result, room_title, max_user_count)
    REGISTER_PACKET(CreateRoomPacketAck, 211)
};

// 방 입장 요청 ( 클라 > 서버 )
struct RoomEnterPacketReq : public Net::IPacket
{
    int room_number;
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, room_number)
    REGISTER_PACKET(RoomEnterPacketReq, 220)
};

// 방 입장 요청 응답 ( 서버 > 클라)
struct RoomEnterPacketAck : public Net::IPacket
{
    bool result;
    RoomInfoEx room_info_ex;
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, result, room_info_ex)
    REGISTER_PACKET(RoomEnterPacketAck, 221)
};

// 방 퇴장 ( 클라 > 서버 )
struct RoomExitPacket : public Net::IPacket
{
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer)
    REGISTER_PACKET(RoomExitPacket, 230)
};

// 다른 유저 방 퇴장 알림 ( 서버 > 클라 )
struct RoomExitOtherPacket : public Net::IPacket
{
    ClientData client_data;
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, client_data)
    REGISTER_PACKET(RoomExitOtherPacket, 231)
};

// 게임 시작 클릭 ( 클라 > 서버 )
struct DoGameStartPacket : public Net::IPacket
{
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer)
    REGISTER_PACKET(DoGameStartPacket, 240)
};

// 게임 시작 ( 서버 > 클라 )
struct GameStartPacket : public Net::IPacket
{
    // 방장 정보
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer)
    REGISTER_PACKET(GameStartPacket, 241)
};

struct RegisterPacketReq : public Net::IPacket
{
    std::wstring id;
    std::wstring password;
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, id, password)
    REGISTER_PACKET(RegisterPacketReq, 242)
};

struct RegisterPacketAck : public Net::IPacket
{
    bool result;
    std::wstring message;
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, result, message)
    REGISTER_PACKET(RegisterPacketAck, 243)
};

struct LoginPacketReq : public Net::IPacket
{
    std::wstring id;
    std::wstring password;
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, id, password)
    REGISTER_PACKET(LoginPacketReq, 244)
};

struct LoginPacketAck : public Net::IPacket
{
    bool result;
    std::wstring message;
    
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, result, message)
    REGISTER_PACKET(LoginPacketAck, 245)
};


