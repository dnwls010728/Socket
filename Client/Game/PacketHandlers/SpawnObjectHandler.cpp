#include "pch.h"
#include "SpawnObjectHandler.h"

#include <CustomPacket.h>

#include "Actor/Component/TransformComponent.h"
#include "Actors/Mobs/MobBase.h"
#include "Subsystems/ObjectPool/ObjectPoolSubsystem.h"

bool SpawnObjectHandler::Handle(Net::IPacket* packet)
{
    SpawnObjectPacket* received_packet = dynamic_cast<SpawnObjectPacket*>(packet);
    if (!received_packet) return false;

    const ObjectInfo& object_info = received_packet->object_info;

    std::shared_ptr<Actor> out_actor = nullptr;
    if (!ObjectPoolSubsystem::Get()->GetFromPool(MobBase::StaticClass(), out_actor)) return false;
    
    std::shared_ptr<NetworkActor> network_actor = std::dynamic_pointer_cast<NetworkActor>(out_actor);
    if (IsValid(network_actor))
    {
        network_actor->SetObjectID(object_info.object_id);
        network_actor->GetTransform()->SetPosition({object_info.position_x, object_info.position_y});

        // 테스트
        const auto& mob = std::dynamic_pointer_cast<MobBase>(network_actor);
        mob->Init(object_info.info.mob.mob_id);
    }

    return true;
}
