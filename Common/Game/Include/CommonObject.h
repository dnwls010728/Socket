#pragma	 once
#include <array>
#include <vector>
#include <string>

struct CharacterProfile
{
    uint32_t character_id;
    uint32_t map_id;
    std::wstring name;

    struct
    {
        float x;
        float y;
    } position;

    std::wstring body_color;

    struct
    {
        int32_t hp;
        int32_t max_hp;
        int32_t exp;
        int32_t lv;
    } stats;
};

struct ItemInfo
{
    uint8_t inventory_type; // 0: 장비, 1: 소비, 2: 기타
    
    uint32_t item_id;
    uint32_t slot_id;
    
    int32_t count;
};

enum class ItemMoveType : uint8_t
{
    kNone = 0,
    kMove,
    kDrop
};

enum class ObjectType : uint8_t
{
    kNone = 0,
    kPlayer,
    kMob,
    kDroppedItem
};

struct PlayerInfo
{
    wchar_t name[256];
    wchar_t body_color[256];
};

struct MobInfo
{
    uint32_t mob_id;
    wchar_t animation_name[256];
    bool is_fliped;
};

struct DroppedItemInfo
{
    uint32_t item_id;
    float dropper_position_x;
    float dropper_position_y;
    int32_t color;
};

struct DroppedItemDestroyInfo
{
};

struct ObjectInfo
{
    ObjectType type;
    uint32_t object_id;
    float position_x;
    float position_y;

    union
    {
        PlayerInfo player;
        MobInfo mob;
        DroppedItemInfo dropped_item;
    } info;
};

struct ObjectDestroyInfo
{
    ObjectType type;
    uint32_t object_id;

    union
    {
        struct
        {
            uint32_t character_id;
        } dropped_item;
    } info;
};

struct CardSelectInfo
{
    uint32_t card_id;
    uint32_t level;
};

enum class PlayerStat : uint8_t
{
    kNone = 0,
    kHP = (0x01<<0),
    kMaxHP = (0x01<<1),
    kExp = (0x01<<2),
    kLv = (0x01<<3),
    kAtk = (0x01<<4),
    kDef = (0x01<<5),
    kDig = (0x01<<6)
};

enum class MobState : uint8_t
{
    kIdle = 0,
    kWalk,
    kHit,
    kDie
};

enum class InventoryType : uint8_t
{
    kNone = 0,
    kEquip,
    kUse,
    kEtc,
    kEquipped,
    kCount
};

enum class InventoryAction : uint8_t
{
    kNone = 0,
    kAdd,
    kChangeCount,
    kMove,
    kRemove
};

enum class EquipSlot : uint8_t
{
    kNone = 0,
    kHat,
    kTop,
    kWeapon,
    kCount
};

struct InventoryChange
{
    uint8_t inventory_type;
    InventoryAction action;

    union
    {
        struct
        {
            uint32_t slot_id;
            uint32_t item_id;
            int32_t count;
        } add;

        struct
        {
            uint32_t slot_id;
            int32_t count;
        } change_count;

        struct
        {
            uint32_t first_slot;
            uint32_t second_slot;
        } move;

        struct
        {
            uint32_t slot_id;
        } remove;
    };
};

struct PartyMemberInfo
{
    uint32_t character_id;
    std::wstring name;
    std::wstring body_color;
    int32_t lv;
    int32_t hp;
    int32_t max_hp;
};

enum class PartyMemberChangeType : uint8_t
{
    kJoin = 0,
    kLeave,
    kUpdate
};

enum class PartyStatType : uint8_t
{
    kHP = 0,
    kMaxHP,
    kLv
};

enum class BuffStat : uint8_t
{
    kAtk = 0,
    kDef,
    kDig
};

enum class PartyInfoType : uint8_t
{
    kHostID = 0,
    kPartyName
};

struct KeyBindingInfo
{
    uint32_t scancode;
    uint8_t type;
    int32_t action;
};
