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
    std::vector<CharacterProfile> profiles;
    
    SERIALIZABLE_FIELDS(is_success, message, profiles)
    REGISTER_PACKET(LoginResponse, 203)
};

// 닉네임 중복 확인 요청
struct CheckNameRequest : public Net::IPacket
{
    std::wstring name;
    
    SERIALIZABLE_FIELDS(name)
    REGISTER_PACKET(CheckNameRequest, 204)
};

// 닉네임 중복 확인 응답
struct CheckNameResponse : public Net::IPacket
{
    bool is_available;

    SERIALIZABLE_FIELDS(is_available)
    REGISTER_PACKET(CheckNameResponse, 205)
};

// 캐릭터 생성 요청
struct CreateCharacterRequest : public Net::IPacket
{
    std::wstring name;
    std::wstring character_color;
    
    SERIALIZABLE_FIELDS(name, character_color)
    REGISTER_PACKET(CreateCharacterRequest, 206)
};

// 캐릭터 생성 응답

// 캐릭터 선택 요청
struct SelectCharacterRequest : public Net::IPacket
{
    uint32_t character_id;
    
    SERIALIZABLE_FIELDS(character_id)
    REGISTER_PACKET(SelectCharacterRequest, 208)
};

// 캐릭터 선택 응답
struct SelectCharacterResponse : public Net::IPacket
{
    std::wstring name;
    std::wstring character_color;

    uint32_t character_id;
    uint32_t lv;
    uint32_t hp;
    uint32_t max_hp;
    uint32_t exp;
    uint32_t color;
    uint32_t map_id;

    struct
    {
        float x;
        float y;
    } spawn_position;

    std::vector<ItemInfo> inventory;
    
    SERIALIZABLE_FIELDS(name, character_color, character_id, lv, hp, max_hp, exp, color, map_id, spawn_position, inventory)
    REGISTER_PACKET(SelectCharacterResponse, 209)
};

struct ChangeMapPacket : public Net::IPacket
{
    uint32_t map_id;
    
    SERIALIZABLE_FIELDS(map_id)
    REGISTER_PACKET(ChangeMapPacket, 210)
};

struct MapLoadPacket : public Net::IPacket
{
    uint32_t map_id;

    struct
    {
        float x;
        float y;
    } spawn_position;
    
    SERIALIZABLE_FIELDS(map_id, spawn_position)
    REGISTER_PACKET(MapLoadPacket, 211)
};

struct MapLoadCompletePacket : public Net::IPacket
{
    SERIALIZABLE_FIELDS()
    REGISTER_PACKET(MapLoadCompletePacket, 212)
};

// 플레이어 스폰 패킷
struct SpawnPlayerPacket : public Net::IPacket
{
    uint32_t character_id;

    std::wstring name;
    
    float position_x;
    float position_y;
    
    SERIALIZABLE_FIELDS(character_id, name, position_x, position_y)
    REGISTER_PACKET(SpawnPlayerPacket, 213)
};

// 플레이어 파괴 패킷
struct DestroyPlayerPacket : public Net::IPacket
{
    uint32_t unique_id;
    
    SERIALIZABLE_FIELDS(unique_id)
    REGISTER_PACKET(DestroyPlayerPacket, 214)
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
    REGISTER_PACKET(MovePlayerPacket, 215)
};

struct PlayerAnimationPacket : public Net::IPacket
{
    uint32_t unique_id;
    bool is_flipped;
    std::wstring animation;
    float server_time;
    
    SERIALIZABLE_FIELDS(unique_id, is_flipped, animation, server_time)
    REGISTER_PACKET(PlayerAnimationPacket, 216)
};

struct ChatMessagePacket : public Net::IPacket
{
    uint32_t unique_id;
    std::wstring message;
    
    SERIALIZABLE_FIELDS(unique_id, message)
    REGISTER_PACKET(ChatMessagePacket, 217)
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
    bool instant_play;
    
    SERIALIZABLE_FIELDS(object_id, is_flipped, animation, server_time, instant_play)
    REGISTER_PACKET(ObjectAnimationPacket, 233)
};

struct MoveItemRequest : public Net::IPacket
{
    uint8_t inventory_type;
    
    uint32_t first_slot;
    uint32_t second_slot;
    
    SERIALIZABLE_FIELDS(inventory_type, first_slot, second_slot)
    REGISTER_PACKET(MoveItemRequest, 300)
};

struct MoveItemResponse : public Net::IPacket
{
    uint8_t inventory_type;
    
    uint32_t first_slot;
    uint32_t second_slot;
    
    SERIALIZABLE_FIELDS(inventory_type, first_slot, second_slot)
    REGISTER_PACKET(MoveItemResponse, 301)
};

struct DropItemRequest : public Net::IPacket
{
    uint8_t inventory_type;
    
    uint32_t slot_id;
    uint32_t count;
    
    SERIALIZABLE_FIELDS(inventory_type, slot_id, count)
    REGISTER_PACKET(DropItemRequest, 302)
};

struct DropItemResponse : public Net::IPacket
{
    uint8_t inventory_type;
    
    uint32_t slot_id;
    uint32_t count;
    
    SERIALIZABLE_FIELDS(inventory_type, slot_id, count)
    REGISTER_PACKET(DropItemResponse, 303)
};

struct PickupItemRequest : public Net::IPacket
{
    uint32_t object_id;
    
    SERIALIZABLE_FIELDS(object_id)
    REGISTER_PACKET(PickupItemRequest, 304)
};

struct AttackRequest : public Net::IPacket
{
    uint32_t object_id;
    
    SERIALIZABLE_FIELDS(object_id)
    REGISTER_PACKET(AttackRequest, 400)
};

struct TakeDamagePacket : public Net::IPacket
{
    uint32_t object_id;
    uint32_t updated_hp;
    uint32_t damage_amount;
    float server_time;
    
    SERIALIZABLE_FIELDS(object_id, updated_hp, damage_amount, server_time)
    REGISTER_PACKET(TakeDamagePacket, 401)
};

struct PlayerStatsUpdatePacket : public Net::IPacket
{
    std::array<int32_t, 4> stats = {-1}; // -1 일 경우 업데이트하지 않음
    
    SERIALIZABLE_FIELDS(stats)
    REGISTER_PACKET(PlayerStatsUpdatePacket, 500)
};
