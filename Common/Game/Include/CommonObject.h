#pragma	 once
#include <array>
#include <vector>
#include <string>

struct CharacterProfile
{
    uint32_t character_id;
    int32_t map_id;
    std::wstring name;

    struct
    {
        float x;
        float y;
    } position;

    std::wstring body_color;

    std::array<int32_t, 4> stats = {-1};
};

struct ItemInfo
{
    uint8_t inventory_type; // 0: 장비, 1: 소비, 2: 기타
    
    uint32_t item_id;
    uint32_t slot_index;
    
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
};

struct DroppedItemInfo
{
    uint32_t item_id;
    float dropper_position_x;
    float dropper_position_y;
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


enum class PlayerStat : uint8_t
{
    kHP = 0,
    kMaxHP,
    kExp,
    kLv
};

enum class MobState : uint8_t
{
    kIdle = 0,
    kWalk,
    kHit,
    kDie
};
