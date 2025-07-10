#pragma once
#include "IPacket.h"
#include "Serializer.h"
#include "CustomSerializer.h"

#include <array>

// TODO : 오류코드 추가해서 클라이언트에서 오류 메시지 출력.......

// 메시지 패킷
struct MessagePacket : public Net::IPacket
{
    std::string message;
    SERIALIZABLE_FIELDS(message)
    REGISTER_PACKET(MessagePacket, 100)
};

struct DisconnectPacket : public Net::IPacket
{
    SERIALIZABLE_FIELDS()
    REGISTER_PACKET(DisconnectPacket, 101)
};

// 회원가입 요청
struct RegisterRequest : public Net::IPacket
{
    std::wstring id;
    std::wstring password;
    
    SERIALIZABLE_FIELDS(id, password)
    REGISTER_PACKET(RegisterRequest, 200)
};

// 회원가입 응답
struct RegisterResponse : public Net::IPacket
{
    bool is_success;
    std::wstring message;
    
    SERIALIZABLE_FIELDS(is_success, message)
    REGISTER_PACKET(RegisterResponse, 201)
};

// 로그인 요청
struct LoginRequest : public Net::IPacket
{
    std::wstring id;
    std::wstring password;
    
    SERIALIZABLE_FIELDS(id, password)
    REGISTER_PACKET(LoginRequest, 202)
};

// 로그인 응답
struct LoginResponse : public Net::IPacket
{
    bool is_success;
    std::wstring message;
    std::vector<CharacterInfo> characters;
    
    SERIALIZABLE_FIELDS(is_success, message, characters)
    REGISTER_PACKET(LoginResponse, 203)
};

// 캐릭터 생성 요청
// 캐릭터 생성 응답

// 캐릭터 선택 요청
struct SelectCharacterRequest : public Net::IPacket
{
    uint32_t unique_id;
    
    SERIALIZABLE_FIELDS(unique_id)
    REGISTER_PACKET(SelectCharacterRequest, 206)
};

// 캐릭터 선택 응답
struct SelectCharacterResponse : public Net::IPacket
{
    bool is_success;
    
    std::wstring message;
    std::wstring name;

    uint32_t character_id;
    uint32_t lv;
    uint32_t hp;
    uint32_t max_hp;
    uint32_t exp;
    uint32_t color;

    float position_x;
    float position_y;

    std::vector<ItemInfo> inventory;
    
    SERIALIZABLE_FIELDS(is_success, message, name, character_id, lv, hp, max_hp, exp, color, position_x, position_y, inventory)
    REGISTER_PACKET(SelectCharacterResponse, 207)
};

// 인 게임 맵으로 전환이 완료되었을 때
struct InGameReadyPacket : public Net::IPacket
{
    SERIALIZABLE_FIELDS()
    REGISTER_PACKET(InGameReadyPacket, 208)
};

// 맵 전환 요청
struct ChangeMapRequest : public Net::IPacket
{
    uint32_t map_id;
    
    SERIALIZABLE_FIELDS(map_id)
    REGISTER_PACKET(ChangeMapRequest, 209)
};

// 맵 전환 응답
struct ChangeMapResponse : public Net::IPacket
{
    bool is_success;
    uint32_t map_id;
    
    SERIALIZABLE_FIELDS(is_success, map_id)
    REGISTER_PACKET(ChangeMapResponse, 210)
};

// 플레이어 스폰 패킷
struct SpawnPlayerPacket : public Net::IPacket
{
    uint32_t character_id;

    std::wstring name;
    
    float position_x;
    float position_y;
    
    SERIALIZABLE_FIELDS(character_id, name, position_x, position_y)
    REGISTER_PACKET(SpawnPlayerPacket, 211)
};

// 플레이어 파괴 패킷
struct DestroyPlayerPacket : public Net::IPacket
{
    uint32_t unique_id;
    
    SERIALIZABLE_FIELDS(unique_id)
    REGISTER_PACKET(DestroyPlayerPacket, 212)
};

struct MovePlayerPacket : public Net::IPacket
{
    uint32_t unique_id;
    float position_x;
    float position_y;
    float velocity_x;
    float velocity_y;
    float server_time;
    bool time_update;
    
    SERIALIZABLE_FIELDS(unique_id, position_x, position_y, velocity_x, velocity_y, server_time, time_update)
    REGISTER_PACKET(MovePlayerPacket, 213)
};

struct PlayerAnimationPacket : public Net::IPacket
{
    uint32_t unique_id;
    bool is_flipped;
    std::wstring animation;
    float server_time;
    
    SERIALIZABLE_FIELDS(unique_id, is_flipped, animation, server_time)
    REGISTER_PACKET(PlayerAnimationPacket, 214)
};

struct ChatMessagePacket : public Net::IPacket
{
    uint32_t unique_id;
    std::wstring message;
    
    SERIALIZABLE_FIELDS(unique_id, message)
    REGISTER_PACKET(ChatMessagePacket, 215)
};

struct SpawnObjectPacket : public Net::IPacket
{
    ObjectInfo object_info;
    
    SERIALIZABLE_FIELDS(object_info)
    REGISTER_PACKET(SpawnObjectPacket, 230)
};

struct DestroyObjectPacket : public Net::IPacket
{
    uint32_t object_id;

    SERIALIZABLE_FIELDS(object_id)
    REGISTER_PACKET(DestroyObjectPacket, 231)
};

struct ObjectPositionPacket : public Net::IPacket
{
    uint32_t object_id;
    float position_x;
    float position_y;
    float velocity_x;
    float velocity_y;
    float server_time;
    bool time_update;

    SERIALIZABLE_FIELDS(object_id, position_x, position_y, velocity_x, velocity_y, server_time, time_update)
    REGISTER_PACKET(ObjectPositionPacket, 232)
};

struct ObjectAnimationPacket : public Net::IPacket
{
    uint32_t object_id;
    bool is_flipped;
    std::wstring animation;
    float server_time;
    
    SERIALIZABLE_FIELDS(object_id, is_flipped, animation, server_time)
    REGISTER_PACKET(ObjectAnimationPacket, 233)
};

struct MoveItemRequest : public Net::IPacket
{
    ItemMoveType type;
    uint32_t src;
    uint32_t dest;
    uint32_t count;
    
    SERIALIZABLE_FIELDS(type, src, dest, count)
    REGISTER_PACKET(MoveItemRequest, 300)
};

struct MoveItemResponse : public Net::IPacket
{
    std::vector<InventoryChange> changes;
    
    SERIALIZABLE_FIELDS(changes)
    REGISTER_PACKET(MoveItemResponse, 301)
};

struct AttackRequest : public Net::IPacket
{
    uint32_t object_id;
    
    SERIALIZABLE_FIELDS(object_id)
    REGISTER_PACKET(AttackRequest, 400)
};

struct PlayerStatsUpdatePacket : public Net::IPacket
{
    uint8_t flags = 0;
    std::array<uint32_t, 4> stats = {0};
    
    SERIALIZABLE_FIELDS(flags, stats)
    REGISTER_PACKET(PlayerStatsUpdatePacket, 500)
};
