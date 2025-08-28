#include "pch.h"
#include "PlayerBuffHandler.h"

#include <CustomPacket.h>

#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Subsystems/NetworkSubsystem.h"
#include "UI/UI.h"
#include "UI/UIState.h"
#include "UI/Element/Buff/UIBuffList.h"

bool PlayerBuffHandler::Handle(Net::IPacket* packet)
{
    PlayerBuffPacket* received_packet = dynamic_cast<PlayerBuffPacket*>(packet);
    if (!received_packet) return false;

    int32_t effect_id = received_packet->effect_id;
    float expire_time = received_packet->server_time + received_packet->duration;

    auto player = NetworkSubsystem::Get()->GetPlayer();
    player->buff_effects_.insert_or_assign(effect_id, expire_time);

    if (auto* state = UI::Get()->GetState())
    {
        if (auto* element = state->FindElement<UIBuffList>(L"BuffList"))
            element->AddBuff(effect_id, expire_time);
    }

    return true;
}
