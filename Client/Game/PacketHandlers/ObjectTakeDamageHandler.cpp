#include "pch.h"
#include "ObjectTakeDamageHandler.h"
#include <CustomPacket.h>

#include "Actor/Component/TransformComponent.h"
#include "Actors/Damage.h"
#include "Actors/NetworkActor.h"
#include "Actors/ServerActor.h"
#include "Subsystems/NetworkSubsystem.h"

bool ObjectTakeDamageHandler::Handle(Net::IPacket* packet)
{
    ObjectTakeDamagePacket* received_packet = dynamic_cast<ObjectTakeDamagePacket*>(packet);
    if (!received_packet) return false;

    auto object = NetworkSubsystem::Get()->FindNetworkActor(received_packet->object_id);
    if (!object) return true;

    ServerActor* server_actor = dynamic_cast<ServerActor*>(object.get());
    if (!server_actor) return true;

    server_actor->OnTakeDamage(received_packet->damage_amount);
    return true;
}
