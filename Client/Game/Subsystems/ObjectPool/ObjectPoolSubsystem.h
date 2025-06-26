#pragma once
#include "Subsystem/WorldSubsystem.h"

class ObjectPoolSubsystem : public WorldSubsystem
{
    GENERATED_BODY(ObjectPoolSubsystem, WorldSubsystem)
    
public:
    ObjectPoolSubsystem();
    virtual ~ObjectPoolSubsystem() override = default;

    bool GetFromPool(const rttr::type& type, std::shared_ptr<Actor>& out_actor);
    bool ReturnToPool(const std::shared_ptr<Actor>& actor);

    static ObjectPoolSubsystem* Get();

private:
    std::unordered_map<rttr::type::type_id, std::queue<std::weak_ptr<Actor>>> pool_map_;
    
};
