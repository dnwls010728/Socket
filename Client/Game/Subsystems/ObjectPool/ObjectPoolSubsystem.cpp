#include "pch.h"
#include "ObjectPoolSubsystem.h"

#include "IPoolable.h"

ObjectPoolSubsystem::ObjectPoolSubsystem() :
    pool_map_()
{
}

bool ObjectPoolSubsystem::GetFromPool(const rttr::type& type, std::shared_ptr<Actor>& out_actor)
{
    std::shared_ptr<Actor> result_actor = nullptr;
    
    auto it = pool_map_.find(type.get_id());
    Logger::Print(L"Pool Size: %d", it != pool_map_.end() ? it->second.size() : 0);
    
    if (it == pool_map_.end() || it->second.empty())
    {
        result_actor = World::Get()->SpawnActor<Actor>(type);
        pool_map_[type.get_id()] = {};
    }
    else
    {
        std::weak_ptr<Actor> weak_actor = it->second.front();
        it->second.pop();

        result_actor = weak_actor.lock();
    }

    if (!IsValid(result_actor)) return false;
    if (auto poolable = std::dynamic_pointer_cast<IPoolable>(result_actor))
    {
        poolable->OnActivate();
    }

    out_actor = result_actor;
    return true;
}

bool ObjectPoolSubsystem::ReturnToPool(const std::shared_ptr<Actor>& actor)
{
    if (!IsValid(actor)) return false;
    
    auto it = pool_map_.find(actor->get_type().get_id());
    if (it == pool_map_.end()) return false;

    it->second.push(actor);
    if (auto poolable = std::dynamic_pointer_cast<IPoolable>(actor))
    {
        poolable->OnDeactivate();
    }
    
    return true;
}

ObjectPoolSubsystem* ObjectPoolSubsystem::Get()
{
    return World::Get()->GetSubsystem<ObjectPoolSubsystem>();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<ObjectPoolSubsystem>("ObjectPoolSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
