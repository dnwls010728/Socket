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

    float offset_y = 0;
    auto &damages = received_packet->damage_amount;
    for (int32_t damage_amount : damages)
    {
        std::shared_ptr<Damage> damage = World::Get()->SpawnActor<Damage>(Damage::StaticClass());
        if (!IsValid(damage)) continue;

        damage->SetDamage(damage_amount);
        Math::Vector2 position = object->GetTransform()->GetPosition() + Math::Vector2::Up() * 2.f;
        position.y += offset_y;
        damage->GetTransform()->SetPosition( position);
        offset_y += 1.f;
    }

    return true;
}
