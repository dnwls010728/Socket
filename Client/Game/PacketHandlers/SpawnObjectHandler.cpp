#include "pch.h"
#include "SpawnObjectHandler.h"

#include <CustomPacket.h>

#include "Actor/Component/TransformComponent.h"
#include "Actors/DroppedItem.h"
#include "Actors/Mobs/MobBase.h"
#include "Subsystems/NetworkSubsystem.h"
#include "Subsystems/ObjectPool/ObjectPoolSubsystem.h"

bool SpawnObjectHandler::Handle(Net::IPacket* packet)
{
    SpawnObjectPacket* received_packet = dynamic_cast<SpawnObjectPacket*>(packet);
    if (!received_packet) return false;

    const ObjectInfo& object_info = received_packet->object_info;

    std::shared_ptr<Actor> out_actor = nullptr;
    switch (object_info.type)
    {
    case ObjectType::kMob:
        {
            if (!ObjectPoolSubsystem::Get()->GetFromPool(MobBase::StaticClass(), out_actor)) return false;
            if (auto mob = std::dynamic_pointer_cast<MobBase>(out_actor))
            {
                mob->SetObjectID(object_info.object_id);
                mob->GetTransform()->SetPosition({ object_info.position_x, object_info.position_y });
                
                mob->Init(object_info.info.mob.mob_id);
            }
        }
        break;

    case ObjectType::kDroppedItem:
        {
            if (!ObjectPoolSubsystem::Get()->GetFromPool(DroppedItem::StaticClass(), out_actor)) return false;
            if (auto dropped_item = std::dynamic_pointer_cast<DroppedItem>(out_actor))
            {
                DroppedItemInfo info = object_info.info.dropped_item;
                
                dropped_item->SetObjectID(object_info.object_id);
                dropped_item->GetTransform()->SetPosition({ info.dropper_position_x, info.dropper_position_y });

                dropped_item->Init(info.item_id, { object_info.position_x, object_info.position_y });
            }
        }
        break;
    }
    
    return true;
}
