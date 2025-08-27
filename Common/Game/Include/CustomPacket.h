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

struct PopupPacket : public Net::IPacket
{
    std::wstring text;
    SERIALIZABLE_FIELDS(text)
    REGISTER_PACKET(PopupPacket, 101)
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
    std::wstring body_color;
    
    SERIALIZABLE_FIELDS(name, body_color)
    REGISTER_PACKET(CreateCharacterRequest, 206)
};

// 캐릭터 생성 응답
struct CreateCharacterResponse : public Net::IPacket
{
    CharacterProfile profile;

    SERIALIZABLE_FIELDS(profile);
    REGISTER_PACKET(CreateCharacterResponse, 207)
};

// 캐릭터 삭제 요청
struct DeleteCharacterRequest : public Net::IPacket
{
    uint32_t character_id;
    
    SERIALIZABLE_FIELDS(character_id)
    REGISTER_PACKET(DeleteCharacterRequest, 208)
};

// 캐릭터 삭제 응답
struct DeleteCharacterResponse : public Net::IPacket
{
    uint32_t character_id;
    
    SERIALIZABLE_FIELDS(character_id)
    REGISTER_PACKET(DeleteCharacterResponse, 209)
};

// 캐릭터 선택 요청
struct SelectCharacterRequest : public Net::IPacket
{
    uint32_t character_id;
    
    SERIALIZABLE_FIELDS(character_id)
    REGISTER_PACKET(SelectCharacterRequest, 210)
};

// 캐릭터 선택 응답
struct SelectCharacterResponse : public Net::IPacket
{
    std::wstring name;
    std::wstring body_color;

    uint32_t character_id;
    uint32_t map_id;
    
    int32_t lv;
    int32_t hp;
    int32_t max_hp;
    int32_t exp;
    int32_t color;

    struct
    {
        float x;
        float y;
    } spawn_position;

    uint32_t equip_slot_capacity;
    uint32_t use_slot_capacity;
    uint32_t etc_slot_capacity;

    std::vector<ItemInfo> inventory;
    
    SERIALIZABLE_FIELDS(name, body_color, character_id, lv, hp, max_hp, exp, color, map_id, spawn_position, equip_slot_capacity, use_slot_capacity, etc_slot_capacity, inventory)
    REGISTER_PACKET(SelectCharacterResponse, 211)
};

struct ChangeMapPacket : public Net::IPacket
{
    int32_t portal_id;
    
    SERIALIZABLE_FIELDS(portal_id)
    REGISTER_PACKET(ChangeMapPacket, 212)
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
    REGISTER_PACKET(MapLoadPacket, 213)
};

struct MapLoadCompletePacket : public Net::IPacket
{
    SERIALIZABLE_FIELDS()
    REGISTER_PACKET(MapLoadCompletePacket, 214)
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

struct ObjectSpawnPacket : public Net::IPacket
{
    ObjectInfo object_info;
    
    SERIALIZABLE_FIELDS(object_info)
    REGISTER_PACKET(ObjectSpawnPacket, 230)
};

struct ObjectDestroyPacket : public Net::IPacket
{
    ObjectDestroyInfo object_info;

    SERIALIZABLE_FIELDS(object_info)
    REGISTER_PACKET(ObjectDestroyPacket, 231)
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

struct DropItemPacket : public Net::IPacket
{
    uint8_t inventory_type;
    
    uint32_t slot_id;
    int32_t count;
    
    SERIALIZABLE_FIELDS(inventory_type, slot_id, count)
    REGISTER_PACKET(DropItemPacket, 302)
};

struct InventoryUpdatePacket: public Net::IPacket
{
    std::vector<InventoryChange> changes;
    
    SERIALIZABLE_FIELDS(changes)
    REGISTER_PACKET(InventoryUpdatePacket, 303)
};

struct UseItemPacket : public Net::IPacket
{
    uint32_t slot_id;
    
    SERIALIZABLE_FIELDS(slot_id)
    REGISTER_PACKET(UseItemPacket, 304)
};

struct PickupItemPacket : public Net::IPacket
{
    uint32_t object_id;
    
    SERIALIZABLE_FIELDS(object_id)
    REGISTER_PACKET(PickupItemPacket, 305)
};

struct ColorGainPacket : public Net::IPacket
{
    int32_t color;
    
    SERIALIZABLE_FIELDS(color)
    REGISTER_PACKET(ColorGainPacket, 306)
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
    int32_t damage_amount;
    float server_time;
    
    SERIALIZABLE_FIELDS(object_id, damage_amount, server_time)
    REGISTER_PACKET(TakeDamagePacket, 401)
};

struct PlayerDeathPacket : public Net::IPacket
{
    SERIALIZABLE_FIELDS()
    REGISTER_PACKET(PlayerDeathPacket, 402)
};

struct PlayerRespawnPacket : public Net::IPacket
{
    SERIALIZABLE_FIELDS()
    REGISTER_PACKET(PlayerRespawnPacket, 403)
};

struct PlayerStatsUpdatePacket : public Net::IPacket
{
    PlayerStat mask = PlayerStat::kNone;

    int32_t hp;
    int32_t max_hp;
    int32_t exp;
    int32_t lv;
    
    SERIALIZABLE_FIELDS(mask, hp, max_hp, exp, lv)
    REGISTER_PACKET(PlayerStatsUpdatePacket, 500)
};

struct PlayerBuffPacket : public Net::IPacket
{
    int32_t effect_id;
    float duration;
    float server_time;
    std::vector<std::pair<BuffStat, int32_t>> stat_changes;
    
    SERIALIZABLE_FIELDS(effect_id, duration, server_time, stat_changes)
    REGISTER_PACKET(PlayerBuffPacket, 501)
};

struct PartyInviteRequest : public Net::IPacket
{
    uint32_t invitee_id;
    
    SERIALIZABLE_FIELDS(invitee_id)
    REGISTER_PACKET(PartyInviteRequest, 600)
};

struct PartyInviteNotify : public Net::IPacket
{
    std::wstring inviter_name;
    uint32_t inviter_id;
    uint32_t party_id;
    std::wstring party_name;
    
    SERIALIZABLE_FIELDS(inviter_name,inviter_id,  party_id, party_name)
    REGISTER_PACKET(PartyInviteNotify, 602)
};

struct PartyInviteNotifyResponse : public Net::IPacket
{
    uint32_t party_id;
    uint32_t inviter_id;
    bool result;
    
    SERIALIZABLE_FIELDS(party_id, inviter_id, result)
    REGISTER_PACKET(PartyInviteNotifyResponse, 603)
};

struct PartyCreateRequest : public Net::IPacket
{
    std::wstring party_name;
    
    SERIALIZABLE_FIELDS(party_name)
    REGISTER_PACKET(PartyCreateRequest, 604)
};

struct PartyJoinPacket : public Net::IPacket
{
    uint32_t party_id;
    std::wstring party_name;
    uint32_t host_id;
    std::vector<PartyMemberInfo> members;
    
    SERIALIZABLE_FIELDS(party_id, party_name, host_id, members)
    REGISTER_PACKET(PartyJoinPacket, 605)
};

struct PartyMemberChangedPacket : public Net::IPacket
{
    PartyMemberChangeType change;
    PartyMemberInfo member;

    SERIALIZABLE_FIELDS(change, member)
    REGISTER_PACKET(PartyMemberChangedPacket, 606)
};

struct PartyLeavePacket : public Net::IPacket
{
    SERIALIZABLE_FIELDS()
    REGISTER_PACKET(PartyLeavePacket, 607)
};

struct PartyMemberStatChangedPacket : public Net::IPacket
{
    uint32_t member_id;
    PartyStatType stat;
    std::wstring value;

    SERIALIZABLE_FIELDS(member_id, stat, value)
    REGISTER_PACKET(PartyMemberStatChangedPacket, 608)
};

struct PartyKickRequest : public Net::IPacket
{
    uint32_t member_id;

    SERIALIZABLE_FIELDS(member_id)
    REGISTER_PACKET(PartyKickRequest, 609)
};

struct PartyDelegateRequest : public Net::IPacket
{
    uint32_t member_id;

    SERIALIZABLE_FIELDS(member_id)
    REGISTER_PACKET(PartyDelegateRequest, 610)
};

struct PartyInfoChangedPacket :  public Net::IPacket
{
    PartyInfoType type;
    std::wstring value;
    
    SERIALIZABLE_FIELDS(type, value)
    REGISTER_PACKET(PartyInfoChangedPacket, 611)
};
