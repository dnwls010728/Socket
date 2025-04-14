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

// 방 목록 요청 패킷 -> RoomListPacketAck로 바로 응답
struct RoomListPacketReq : public Net::IPacket
{
    int page_index;    // 방 목록 인덱스
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, page_index)
    REGISTER_PACKET(RoomListPacketReq, 200)
};

// 방 목록 자동 동기화 요청 패킷 -> 해당 페이지가 갱신될 때 서버가 자동으로 RoomListPacketAck 전송
// -1을 전송하면 자동 동기화 종료
struct RoomListSyncPacket : public Net::IPacket
{
    int page_index;    // 방 목록 인덱스
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, page_index)
    REGISTER_PACKET(RoomListSyncPacket, 201)
};

// 방 정보 요청 응답. 사용자가 요청한 인덱스의 방 목록이 없으면 가장 가까운 인덱스 전달
struct RoomListPacketAck : public Net::IPacket
{
    RoomList room_list; 
	SERIALIZABLE_PACKET_FIELDS(CustomSerializer, room_list)
    REGISTER_PACKET(RoomListPacketAck, 202)
};

// 방 생성 요청
struct CreateRoomPacketReq : public Net::IPacket
{
    std::wstring room_title;
    int max_user_count;
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, room_title, max_user_count)
    REGISTER_PACKET(CreateRoomPacketReq, 203)
};

// 방 생성 응답. result true시 생성한 사람은 바로 입장하면 됨
struct CreateRoomPacketAck : public Net::IPacket
{
    int room_number;
    int room_title;
    int max_user_count;
	bool result;
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, room_number, result, room_title, max_user_count)
    REGISTER_PACKET(CreateRoomPacketAck, 204)
};

// 방 입장 요청
struct RoomEnterPacketReq : public Net::IPacket
{
    int room_number;
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, room_number)
    REGISTER_PACKET(RoomEnterPacketReq, 205)
};

// 방 입장 요청 응답
struct RoomEnterPacketAck : public Net::IPacket
{
    bool result;
    // TODO : 유저 정보..
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, result)
    REGISTER_PACKET(RoomEnterPacketAck, 206)
};

// 방 퇴장
struct RoomExitPacket : public Net::IPacket
{
    int room_number;
    SERIALIZABLE_PACKET_FIELDS(CustomSerializer, room_number)
    REGISTER_PACKET(RoomExitPacket, 207)
};

// 게임 시작 클릭 ( 클라 > 서버 )
struct DoGameStartPacket : public Net::IPacket
{
    SERIALIZABLE_PACKET_NONE_FIELDS()
    REGISTER_PACKET(DoGameStartPacket, 300)
};

// 게임 시작 ( 서버 > 클라 )
struct GameStartPacket : public Net::IPacket
{
    // 방장 정보
    SERIALIZABLE_PACKET_NONE_FIELDS()
    REGISTER_PACKET(GameStartPacket, 301)
};


