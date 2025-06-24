#include "pch.h"
#include "PlayerCharacter.h"

#include <CustomPacket.h>

#include "DebugDrawHelper.h"
#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Actors/ItemDrop.h"
#include "Actors/Characters/Components/Controller2DComponent.h"
#include "Actors/Components/StateMachineComponent.h"
#include "Actors/Mobs/MobBase.h"
#include "Asset/AssetManager.h"
#include "FSM/Condition.h"
#include "Input/Keyboard.h"
#include "Math/Math.h"
#include "Physics/Physics2D.h"
#include "State/PlayerFallState.h"
#include "State/PlayerIdleState.h"
#include "State/PlayerWalkState.h"
#include "Subsystems/NetworkSubsystem.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/UIManager.h"
#include "Windows/DX/Sprite.h"

PlayerCharacter::PlayerCharacter(const std::wstring& kName) :
    CharacterBase(kName),
    movement_input_(Math::Vector2::Zero()),
    movement_sync_accumulator_(0.f),
    prev_is_moving(false),
    last_position_(Math::Vector2::Zero())
{
    SetLayer(ActorLayer::kCharacter);
    
    collider_->SetOffset({ 0.f, .5f });

    AnimationPack* animation_pack = AssetManager::Get()->Load<AnimationPack>(L"Sprites\\Characters\\Player\\PlayerSheet.png.animpack");
    if (animation_pack) animator_->SetAnimationPack(animation_pack);
}

void PlayerCharacter::ReceivePacket(Net::IPacket* packet)
{
    CharacterBase::ReceivePacket(packet);

    switch (packet->GetPacketID())
    {
    case MovePlayerPacket::StaticPacketID:
        {
            MovePlayerPacket* move_player_packet = static_cast<MovePlayerPacket*>(packet);
            Snapshot snapshot;
            snapshot.position.x = move_player_packet->position_x;
            snapshot.position.y = move_player_packet->position_y;
            snapshot.velocity.x = move_player_packet->velocity_x;
            snapshot.velocity.y = move_player_packet->velocity_y;
            snapshot.is_flipped = move_player_packet->is_flipped;
            snapshot.animation = move_player_packet->animation;
            snapshot.server_time =  move_player_packet->server_time;
            snapshot.time_update =  move_player_packet->time_update;
            snapshots_.push_back(snapshot);
        }
        break;
        
    default:
        break;
    }
}

void PlayerCharacter::InitSpawn(const std::wstring& name, const Math::Vector2& position)
{
    character_name_ = name;

    last_position_ = position;
    
    GetTransform()->SetPosition(position);
}

void PlayerCharacter::BeginPlay()
{
    CharacterBase::BeginPlay();

    if (IsMine())
    {
        std::shared_ptr<PlayerIdleState> idle_state = std::make_shared<PlayerIdleState>(GetSharedThis());
        std::shared_ptr<PlayerWalkState> walk_state = std::make_shared<PlayerWalkState>(GetSharedThis());
        std::shared_ptr<PlayerFallState> fall_state = std::make_shared<PlayerFallState>(GetSharedThis());

        state_machine_->AddTransition(idle_state, walk_state, [&]() { return !Math::IsEqual(movement_input_.x, 0.f); });
        state_machine_->AddTransition(idle_state, fall_state, [&]() { return !controller_->GetCollisions().is_below; });
        
        state_machine_->AddTransition(walk_state, idle_state, [&]() { return Math::IsEqual(movement_input_.x, 0.f); });
        state_machine_->AddTransition(walk_state, fall_state, [&]() { return !controller_->GetCollisions().is_below; });
        
        state_machine_->AddTransition(fall_state, idle_state, [&]() { return controller_->GetCollisions().is_below; });
        
        state_machine_->SetState(idle_state);
    }
    else
    {
        animator_->PlayAnimation(L"Idle");
    }
}

void PlayerCharacter::PhysicsTick(float delta_time)
{
    std::shared_ptr<TransformComponent> transform = GetTransform();

    if (IsMine())
    {
        const Controller2DComponent::CollisionInfo& collisions = controller_->GetCollisions();
        
        velocity_.y += gravity_ * delta_time;
        controller_->Move(velocity_ * delta_time, movement_input_);
        
        if (collisions.is_above || collisions.is_below) velocity_.y = 0.f;
        
        Math::Vector2 position = transform->GetPosition();

        movement_sync_accumulator_ += delta_time;
        bool is_moving_now = !Math::IsEqual(velocity_.x, 0.f) || !Math::IsEqual(velocity_.y, 0.f);
        
        bool should_send = false;

        if (is_moving_now)
        {
            if (movement_sync_accumulator_ >= 0.1f)
                should_send = true;
        }
        
        bool is_moving_start = false;
        if (is_moving_now != prev_is_moving)
        {
            should_send = true;
            if (is_moving_now)
            {
                is_moving_start = true;
            }
        }
        
        if (should_send)
        {
            if (is_moving_start)
            {
                MovePlayerPacket dummy_packet;
                dummy_packet.position_x = last_position_.x;
                dummy_packet.position_y = last_position_.y;
                dummy_packet.velocity_x = velocity_.x;
                dummy_packet.velocity_y = velocity_.y;
                dummy_packet.is_flipped = renderer_->IsFlipX();
                dummy_packet.animation = animator_->GetCurrentState()->GetName();
                dummy_packet.server_time = SessionSubsystem::Get()->GetServerTime();
                dummy_packet.time_update = true;
                SendPacket(dummy_packet);
            }
            
            MovePlayerPacket move_player_packet;
            move_player_packet.position_x = position.x;
            move_player_packet.position_y = position.y;
            move_player_packet.velocity_x = velocity_.x;
            move_player_packet.velocity_y = velocity_.y;
            move_player_packet.is_flipped = renderer_->IsFlipX();
            move_player_packet.animation = animator_->GetCurrentState()->GetName();
            move_player_packet.server_time = SessionSubsystem::Get()->GetServerTime();
            move_player_packet.time_update = false;
            SendPacket(move_player_packet);

            prev_is_moving = is_moving_now;
            last_position_ = position;
            movement_sync_accumulator_ = 0.f;
        }
    }
    else
    {
        float server_now = SessionSubsystem::Get()->GetServerTime();

        float interpolation_time = server_now - EngineSettings::Get()->GetInterpolationDelay();
        
        while (snapshots_.size() >= 2 && snapshots_[1].server_time < interpolation_time)
        {
            snapshots_.pop_front(); 
        }

        if (snapshots_.size() >= 2 && snapshots_[1].time_update)
        {
            snapshots_.pop_front();
        }

        if (snapshots_.size() >= 2 && snapshots_[0].time_update)
        {
            snapshots_[0].server_time = snapshots_[1].server_time - EngineSettings::Get()->GetInterpolationDelay();
        }
        
        if (snapshots_.size() >= 2)
        {
            const Snapshot& from = snapshots_[0];
            const Snapshot& to = snapshots_[1];

            float t = (interpolation_time - from.server_time) / (to.server_time - from.server_time);
            t = Math::Clamp(t, 0.f, 1.f);

            bool is_flipped = t < .5f ? from.is_flipped : to.is_flipped;
            std::wstring animation = t < .5f ? from.animation : to.animation;

            Math::Vector2 position = Math::Vector2::Lerp(from.position, to.position, t);
            GetTransform()->SetPosition(position);

            renderer_->SetFlipX(is_flipped);
            animator_->PlayAnimation(animation);
        }
        /*
        else if (snapshots_.size() == 1)
        {
            const Snapshot& snapshot = snapshots_[0];

            float delta = interpolation_time - snapshot.server_time;
            delta = Math::Clamp(delta, 0.f, 0.2f);

            if (delta > 0.f)
            {
                Math::Vector2 position = snapshot.position + snapshot.velocity * delta;
                GetTransform()->SetPosition(position);

                renderer_->SetFlipX(snapshot.is_flipped);
                animator_->PlayAnimation(snapshot.animation);
            }
        }
        */
    }
    
    CharacterBase::PhysicsTick(delta_time);
}

void PlayerCharacter::Tick(float delta_time)
{
    CharacterBase::Tick(delta_time);

    if (IsMine())
    {
        UI_OLD::Manager* ui_manager = UI_OLD::Manager::Get();
        Keyboard* keyboard = Keyboard::Get();
        
        if (!ui_manager->HasFocus())
        {
            movement_input_.x = keyboard->GetKey(VK_RIGHT) - keyboard->GetKey(VK_LEFT);
            movement_input_.y = keyboard->GetKey(VK_UP) - keyboard->GetKey(VK_DOWN);

            if (keyboard->GetKeyDown('1'))
            {
                NetworkSubsystem::Get()->ChangeMap(0);
            }

            if (keyboard->GetKeyDown('2'))
            {
                NetworkSubsystem::Get()->ChangeMap(1);
            }

            // 공격 테스트
            if (keyboard->GetKeyDown('X'))
            {
                std::vector<Actor*> hit_actors;
                bool is_hit = Physics2D::OverlapBoxAll(
                    GetTransform()->GetPosition(),
                    { 3.f, 2.f },
                    hit_actors,
                    static_cast<uint16_t>(ActorLayer::kMob)
                );

                if (is_hit)
                {
                    for (const auto& actor : hit_actors)
                    {
                        MobBase* mob = static_cast<MobBase*>(actor);

                        AttackRequest request;
                        request.object_id = mob->GetObjectID();
                        SendPacket(request);
                    }
                }
            }
        }
        else
        {
            movement_input_.x = 0.f;
            movement_input_.y = 0.f;
        }

        // 공격 범위 확인용
        DebugDrawHelper::Get()->DrawBox(GetTransform()->GetPosition(), { 3.f, 2.f }, Math::Color::Red);
    }
    else
    {
    }
}

void PlayerCharacter::PostTick(float delta_time)
{
    CharacterBase::PostTick(delta_time);

    if (movement_input_.x != 0.f)
    {
        renderer_->SetFlipX(movement_input_.x < 0.f);
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<PlayerCharacter>("PlayerCharacter")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
