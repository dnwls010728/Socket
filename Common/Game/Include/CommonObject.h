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
    uint32_t item_id;
    uint32_t slot_index;
    uint32_t count;
};

struct InventoryChange
{
    uint32_t dest;
    uint32_t arg;
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
    kMob
};

struct MobInfo
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
        MobInfo mob;
    } info;
};
