#pragma once
#include <cstdint>
#include <memory>
#include <windows.h>

#include "box2d/id.h"
#include "Engine/Misc/EngineMacros.h"
#include "rttr/rttr_enable.h"
#include "rttr/registration_friend.h"
#include "rttr/registration.h"
#include "Engine/Enums.h"
#include "Math/Vector2.h"


class Map;
 
class Actor : public std::enable_shared_from_this<Actor>
{
    friend class MapBase;
    friend class World;
    friend class TransformComponent;
    friend class ColliderComponent;
    friend class BoxColliderComponent;
    friend class CircleColliderComponent;
    friend class CapsuleColliderComponent;
    friend class RigidBody2DComponent;
    friend class Physics2D;
public:
    SHADER_CLASS_HELPER(Actor)
    GENERATED_BODY(Actor)
    
    Actor(const std::wstring& kName);
    virtual ~Actor() = default;
    
    void Destroy();

    void SetActive(bool is_active);
    inline bool IsActive()      const { return is_active_; }
    
    inline uint32_t GetUniqueID()   const { return unique_id_; }
    inline void SetUinqueID(const uint32_t kUniqueID) { unique_id_ = kUniqueID; }

    inline Map* GetMap()        const { return map_; }
    inline void SetMap(Map* map) { map_ = map; }
    
    inline Math::Vector2 GetLocation() const { return location_; }
    inline void SetLocation(Math::Vector2 new_location) { location_ = new_location; }
    
    inline Math::Vector2 GetVelocity() const { return velocity_; }
    inline void SetVelocity(Math::Vector2 new_velocity) { velocity_ = new_velocity; }
   
protected:
    virtual void Tick(float delta_time);
    virtual void PhysicsTick(float delta_time);
    virtual void PostTick(float delta_time);
    virtual void OnEnable();
    virtual void OnDisable();
    virtual void BeginPlay();
    virtual void EndPlay(EndPlayReason reason);
    virtual void InitializeActor();
    
    virtual void CreateBody();
private:
    Math::Vector2 location_;
    Math::Vector2 velocity_;

    b2BodyId        body_id_;
    uint32_t        unique_id_;
    std::wstring    name_;
    bool            is_active_;
    
    Map*            map_;
};