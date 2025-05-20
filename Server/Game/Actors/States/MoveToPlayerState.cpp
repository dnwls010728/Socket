#include "pch.h"
#include "MoveToPlayerState.h"

#include "Actor/Component/TransformComponent.h"
#include "Map/Map.h"
#include "Session/Player.h"

MoveToPlayerState::MoveToPlayerState(const std::shared_ptr<CharacterBase>& kCharacter)
    : BaseState(kCharacter)
{
}

void MoveToPlayerState::PhysicsTick(float)
{
    auto owner = character_.lock();
    if (!IsValid(owner))
        return;

    Map* map = owner->GetMap();
    if (!map)
        return;

    Math::Vector2 owner_position = owner->GetTransform()->GetPosition();
    Math::Vector2 nearest_position = Math::Vector2::Zero();
    float nearest_distance = 0.f;
    for (const auto& player_weak : map->GetPlayers())
    {
        auto player = player_weak.lock();
        if (!player)
            continue;

        Math::Vector2 player_position = player->GetPosition();
        float distance = Math::Vector2::Distance(player_position,owner_position);
        if (nearest_distance == 0.f || distance < nearest_distance)
        {
            nearest_distance = distance;
            nearest_position = player_position;
        }
    }

    if (nearest_distance == 0.f)
    {
        return;
    }

    if (nearest_position.x < owner_position.x)
        owner->SetMovementInput({-1.0f,0.0f});
    else
        owner->SetMovementInput({1.0f,0.0f});
}
