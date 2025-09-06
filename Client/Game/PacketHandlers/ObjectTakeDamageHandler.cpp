#include "pch.h"
#include "ObjectTakeDamageHandler.h"
#include <CustomPacket.h>

#include "Actor/Component/TransformComponent.h"
#include "Actors/Damage.h"
#include "Actors/NetworkActor.h"
#include "Subsystems/NetworkSubsystem.h"

bool ObjectTakeDamageHandler::Handle(Net::IPacket* packet)
{
    ObjectTakeDamagePacket* received_packet = dynamic_cast<ObjectTakeDamagePacket*>(packet);
    if (!received_packet) return false;

    auto object = NetworkSubsystem::Get()->FindNetworkActor(received_packet->object_id);
    if (!object) return true;
    
    std::shared_ptr<Damage> damage = World::Get()->SpawnActor<Damage>(Damage::StaticClass());
    if (!IsValid(damage)) return true;

    damage->SetDamage(received_packet->damage_amount);
    damage->GetTransform()->SetPosition( object->GetTransform()->GetPosition() + Math::Vector2::Up() * 2.f);
    
    return true;
}
