#pragma	 once
#include <vector>
#include <string>

struct CharacterInfo
{
    int unique_id;
    int account_id;
    std::wstring name;
    int lv;
    int job;
    int map_id;
    float last_position_x;
    float last_position_y;

    inline bool IsValid() const { return unique_id > 0; }
};

struct ItemInfo
{
    uint8_t inventory_type; // 0: 장비, 1: 소비, 2: 기타
    
    uint32_t item_id;
    uint32_t slot_index;
    uint32_t count;
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
    kHP = (0x01<<0),
    kMaxHP = (0x01<<1),
    kExp = (0x01<<2),
    kLv = (0x01<<3)
};

enum class MobState : uint8_t
{
    kIdle = 0,
    kWalk,
    kHit,
    kDie
};
