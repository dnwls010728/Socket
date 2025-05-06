#pragma once
#include <cstdint>
#include <memory>
#include <windows.h>
#include "Engine/Misc/EngineMacros.h"
#include "rttr/rttr_enable.h"
#include "rttr/registration_friend.h"
#include "Engine/Misc/Type.h"
#include "Engine/Enums.h"


class Map;
 
class MapObject : public std::enable_shared_from_this<MapObject>
{
public:
    SHADER_CLASS_HELPER(MapObject)
    GENERATED_BODY(MapObject)
    
    MapObject(const std::wstring& name);
    virtual ~MapObject() = default;

    inline uint32_t GetUniqueID() const { return unique_id_; }
    inline Map* GetMap() const { return map_; }
protected:
    virtual void Tick(float delta_time);
    virtual void PhysicsTick(float delta_time);
    virtual void PostTick(float delta_time);
    virtual void OnEnable(){};
    virtual void OnDisable(){};
    virtual void BeginPlay(){};
    virtual void EndPlay(EndPlayReason reason){};
private:
    friend class MapBase;
    friend class World;
    
    inline void SetUniqueID(const uint32_t unique_id) { unique_id_ = unique_id; }
    inline void SetMap(Map* map) { map_ = map; }
    
    uint32_t unique_id_;
    Map* map_;
 
};