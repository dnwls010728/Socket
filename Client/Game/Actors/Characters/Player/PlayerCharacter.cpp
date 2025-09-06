#include "pch.h"
#include "PlayerCharacter.h"

#include <CustomPacket.h>
#include <algorithm>
#include <numbers>

#include "DebugDrawHelper.h"
#include "Actor/Portal.h"
#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Actors/Damage.h"
#include "Actors/DroppedItem.h"
#include "Actors/Effect.h"
#include "Actors/Characters/Components/Controller2DComponent.h"
#include "Actors/Components/StateMachineComponent.h"
#include "Actors/Mobs/MobBase.h"
#include "Asset/AssetManager.h"
#include "Audio/Audio.h"
#include "Audio/AudioManager.h"
#include "DirectXTK/Mouse.h"
#include "FSM/Condition.h"
#include "imgui/imgui.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Math/Math.h"
#include "Physics/Physics2D.h"
#include "State/PlayerDeathState.h"
#include "State/PlayerFallState.h"
#include "State/PlayerIdleState.h"
#include "State/PlayerWalkState.h"
#include "Subsystems/NetworkSubsystem.h"
#include "Subsystems/PartySubsystem.h"
#include "Subsystems/PlayerSubsystem.h"
#include "Subsystems/SessionSubsystem.h"
#include "Time/Time.h"
#include "UI/UI.h"
#include "UI/Element/UIContextMenu.h"
#include "UI/UIInGameState.h"
#include "UI/Element/UIPopup.h"
#include "Windows/DX/Sprite.h"

PlayerCharacter::PlayerCharacter(const std::wstring& kName) :
    CharacterBase(kName),
    buff_effects_(),
    move_axis_(Math::Vector2::Zero()),
    last_position_(Math::Vector2::Zero()),
    was_grounded_(false),
    was_moving_(false),
    last_flip_(false),
    is_dead_(false),
    movement_sync_accumulator_(0.f),
    invincible_time_(0.f),
    prev_animation{0,},
    color_(Math::Color::White),
    bonus_jumps_(1)
{
    SetLayer(ActorLayer::kPlayer);
    
    collider_->SetOffset({ 0.f, .5f });
    collider_->SetSize({1.f, 1.f});

    AnimationPack* animation_pack = AssetManager::Get()->Load<AnimationPack>(L"Sprites\\Characters\\Player\\PlayerSheet.png.apack");
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
            MovementSnapshot snapshot;
            snapshot.position.x = move_player_packet->position_x;
            snapshot.position.y = move_player_packet->position_y;
            snapshot.velocity.x = move_player_packet->velocity_x;
            snapshot.velocity.y = move_player_packet->velocity_y;
            snapshot.server_time =  move_player_packet->server_time;
            snapshot.time_update =  move_player_packet->time_update;
            movement_snapshots_.push_back(snapshot);
        }
        break;
    case PlayerAnimationPacket::StaticPacketID:
        {
            PlayerAnimationPacket* player_packet = static_cast<PlayerAnimationPacket*>(packet);
            AnimationSnapshot snapshot;
			snapshot.animation = player_packet->animation;
			snapshot.is_flipped = player_packet->is_flipped;
			snapshot.server_time = player_packet->server_time;
			animation_snapshots_.push_back(snapshot);
        }
        break;
    case SkillCastPacket::StaticPacketID:
        {
            SkillCastPacket* skill_packet = static_cast<SkillCastPacket*>(packet);
            auto effect = World::Get()->SpawnActor<Effect>(Effect::StaticClass(), L"Effect");
            if (IsValid(effect))
            {
                effect->GetTransform()->SetPosition(GetTransform()->GetPosition());
                effect->SetFlipX(renderer_->IsFlipX());
            }
        }
        break;
        
    default:
        break;
    }
}

void PlayerCharacter::TakeDamage(int32_t damage_amount, float server_time)
{
    if (damage_amount == 0) return;
    if (IsMine())
    {
        Audio* audio = AssetManager::Get()->Load<Audio>(L"Audio\\SE\\p_hit.mp3");
        AudioManager::Get()->PlaySound2D(audio, ChannelGroup::kSE);
    }

    invincible_time_ = server_time + 1.f;
}

void PlayerCharacter::Init(const std::wstring& name, const std::wstring& body_color, const Math::Vector2& position)
{
    character_name_ = name;

    color_ = Math::Color::HexToColor(body_color);
    renderer_->SetColor(color_);
    
    GetTransform()->SetPosition(position);
    last_position_ = position;
}

void PlayerCharacter::UpdateFlip() const
{
    if (move_axis_.x != 0.f) renderer_->SetFlipX(move_axis_.x < 0.f);
}

void PlayerCharacter::SetDead()
{
    is_dead_ = true;
}

void PlayerCharacter::BeginPlay()
{
    CharacterBase::BeginPlay();

    if (IsMine())
    {
        std::shared_ptr<PlayerIdleState> idle_state = std::make_shared<PlayerIdleState>(GetSharedThis(), animator_);
        std::shared_ptr<PlayerWalkState> walk_state = std::make_shared<PlayerWalkState>(GetSharedThis(), animator_);
        std::shared_ptr<PlayerFallState> fall_state = std::make_shared<PlayerFallState>(GetSharedThis(), animator_);
        std::shared_ptr<PlayerDeathState> death_state = std::make_shared<PlayerDeathState>(GetSharedThis(), animator_);

        state_machine_->AddState(idle_state);
        state_machine_->AddState(walk_state);
        state_machine_->AddState(fall_state);
        state_machine_->AddState(death_state);

        state_machine_->AddTransition(idle_state, walk_state, [&]() { return !Math::IsEqual(move_axis_.x, 0.f); });
        state_machine_->AddTransition(idle_state, fall_state, [&]() { return !controller_->GetCollisions().is_below; });
        
        state_machine_->AddTransition(walk_state, idle_state, [&]() { return Math::IsEqual(move_axis_.x, 0.f); });
        state_machine_->AddTransition(walk_state, fall_state, [&]() { return !controller_->GetCollisions().is_below; });

        state_machine_->AddTransition(fall_state, idle_state, [&]() { return controller_->GetCollisions().is_below; });

        state_machine_->AddAnyTransition(death_state, [&]() { return is_dead_; });
        
        state_machine_->SetState(idle_state);

        const auto& node = animator_->GetOrAddNode(L"Walk");
        node->AddCallback(4, this, &PlayerCharacter::OnFootstep);
        node->AddCallback(7, this, &PlayerCharacter::OnFootstep);
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
        was_grounded_ = collisions.is_below;
        
        velocity_.y += gravity_ * delta_time;
        controller_->Move(velocity_ * delta_time, move_axis_);
        
        if (collisions.is_above || collisions.is_below) velocity_.y = 0.f;
        
        if (!was_grounded_ && collisions.is_below)
        {
            Audio* audio = AssetManager::Get()->Load<Audio>(L"Audio\\SE\\landing.mp3");
            AudioManager::Get()->PlaySound2D(audio, ChannelGroup::kSE);

            bonus_jumps_ = 1;
        }
    }
    
    CharacterBase::PhysicsTick(delta_time);
}

void PlayerCharacter::Tick(float delta_time)
{
    CharacterBase::Tick(delta_time);

    float server_now = SessionSubsystem::Get()->GetServerTime();
    if (invincible_time_ > server_now)
    {
        float alpha = 1.f - (invincible_time_ - server_now) / 2.f;
        float phase = alpha * 10 * Math::PI(); // 10회
        float value = .9f - .5f * std::abs(std::sin(phase)); // 0.4 ~ 0.9 사이의 값
        
        uint8_t r = static_cast<uint8_t>(value * color_.r);
        uint8_t g = static_cast<uint8_t>(value * color_.g);
        uint8_t b = static_cast<uint8_t>(value * color_.b);
        renderer_->SetColor(Math::Color(r, g, b, color_.a));
    }
    else renderer_->SetColor(color_);

    SyncCharacterMovement(delta_time);
    
    if (IsMine())
    {
        const Controller2DComponent::CollisionInfo& collisions = controller_->GetCollisions();
        
        Keyboard* keyboard = Keyboard::Get();
        Mouse* mouse = Mouse::Get();

        if (!is_dead_ && !UI::Get()->IsEditingText())
        {
            move_axis_.x = keyboard->GetKey(Scancode::kKeyRight) - keyboard->GetKey(Scancode::kKeyLeft);
            move_axis_.y = keyboard->GetKey(Scancode::kKeyUp) - keyboard->GetKey(Scancode::kKeyDown);

            PlayerSubsystem* player_subsystem = PlayerSubsystem::Get();
            float portal_cooldown = player_subsystem->GetPortalCooldown();

            if (keyboard->GetKey(Scancode::kKeyUp) && portal_cooldown - Time::Seconds() <= 0.f)
            {
                Math::Vector2 center = GetTransform()->GetPosition();
                Math::Vector2 size = {1.f, 1.f};
                
                Actor* out_actor = nullptr;
                bool is_hit = Physics2D::OverlapBox(
                    center,
                    size,
                    &out_actor,
                    static_cast<uint16_t>(ActorLayer::kPortal)
                );

                if (is_hit)
                {
                    if (auto* portal = dynamic_cast<Portal*>(out_actor))
                    {
                        Audio* audio = AssetManager::Get()->Load<Audio>(L"Audio\\SE\\portal.mp3");
                        AudioManager::Get()->PlaySound2D(audio, ChannelGroup::kSE);
                        
                        NetworkSubsystem::Get()->ChangeMap(portal->GetID());
                    }

                    player_subsystem->SetPortalCooldown(Time::Seconds() + .8f);
                }
            }

            if (keyboard->GetKey(Scancode::kKeyC) && collisions.is_below)
            {
                Audio* audio = AssetManager::Get()->Load<Audio>(L"Audio\\SE\\jump.mp3");
                AudioManager::Get()->PlaySound2D(audio, ChannelGroup::kSE);
                
                velocity_.y = 8.f;
            }

            if (keyboard->GetKeyDown(Scancode::kKeyC) && !collisions.is_below)
            {
                if (bonus_jumps_ > 0)
                {
                    Audio* audio = AssetManager::Get()->Load<Audio>(L"Audio\\SE\\doublejump.mp3");
                    AudioManager::Get()->PlaySound2D(audio, ChannelGroup::kSE);
                
                    velocity_.y = 8.f;
                    --bonus_jumps_;
                }
            }

            // 아이템 줍기
            if (keyboard->GetKeyDown(Scancode::kKeyZ))
            {
                Math::Vector2 center = GetTransform()->GetPosition();
                Math::Vector2 size = {1.f, 1.f};

                Actor* out_actor = nullptr;
                bool is_hit = Physics2D::OverlapBox(
                    center,
                    size,
                    &out_actor,
                    static_cast<uint16_t>(ActorLayer::kDroppedItem)
                );

                if (is_hit)
                {
                    DroppedItem* dropped_item = dynamic_cast<DroppedItem*>(out_actor);
                    if (IsValid(dropped_item))
                    {
                        PickupItemPacket request;
                        request.object_id = dropped_item->GetObjectID();
                        SendPacket(request);
                    }
                }
            }

            if (keyboard->GetKeyDown(Scancode::kKeyQ))
            {
                SkillCastRequest request;
                request.skill_id = 100000;
                SendPacket(request);
            }

            if (keyboard->GetKeyDown(Scancode::kKeyA))
            {
                auto effect = World::Get()->SpawnActor<Effect>(Effect::StaticClass(), L"Effect");
                if (IsValid(effect))
                {
                    effect->GetTransform()->SetPosition(GetTransform()->GetPosition());
                    effect->SetFlipX(renderer_->IsFlipX());
                }
            }

            // 공격 테스트
            if (keyboard->GetKeyDown(Scancode::kKeyX))
            {
                std::vector<Actor*> hit_actors;
                bool is_hit = Physics2D::OverlapBoxAll(
                    GetTransform()->GetPosition(),
                    {3.f, 2.f},
                    hit_actors,
                    static_cast<uint16_t>(ActorLayer::kMob)
                );

                if (is_hit)
                {
                    for (const auto& actor : hit_actors)
                    {
                        MobBase* mob = static_cast<MobBase*>(actor);
                        if (!IsValid(mob) || mob->IsDead()) continue;

                        AttackRequest request;
                        request.object_id = mob->GetObjectID();
                        SendPacket(request);

                        std::shared_ptr<Actor> damage = World::Get()->SpawnActor<Actor>(Damage::StaticClass());
                        if (IsValid(damage))
                        {
                            damage->GetTransform()->SetPosition(
                                mob->GetTransform()->GetPosition() + Math::Vector2::Up() * 2.f);
                        }
                    }
                }
            }
        }
        else
        {
            move_axis_ = Math::Vector2::Zero();
        }

        if (mouse->GetMouseButtonDown(MouseButton::kRight))
        {
            Math::Vector2 position = Renderer::Get()->ScreenToWorld(mouse->GetMousePosition());
            
            Actor* out_actor = nullptr;
            bool is_hit = Physics2D::OverlapPoint(position, &out_actor, static_cast<uint16_t>(ActorLayer::kPlayer));
            if (is_hit)
            {
                PlayerCharacter* player = dynamic_cast<PlayerCharacter*>(out_actor);
                if (player && player != this)
                {
                    UIInGameState* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState());
                    if (state)
                    {
                        UIContextMenu* menu = state->GetContextMenu();
                        menu->Clear();
                        menu->AddItem(L"파티에 초대", [this, player]()
                           {
                                if (PartySubsystem::Get()->IsJoinedParty() == false)
                                {
                                    UIPopup::PopupParam param;
                                    param.caption = L"현재 파티에 소속되어 있지 않습니다.\n 파티를 생성하시겠습니까?";
                                    param.option = UIPopup::PopupOption::Yes | UIPopup::PopupOption::No;
                                    param.callback = [this](const std::wstring& text, UIPopup::PopupOption option)
                                    {
                                        if (option  == UIPopup::PopupOption::Yes)
                                            StartCreateParty();
                                        return true;
                                    };
                                    UIPopup::ShowPopup(param);
                                }
                                else
                                {
                                    PartyInviteRequest request;
                                    request.invitee_id = player->GetObjectID();
                                    SendPacket(request);    
                                }
                           });
                        menu->Show(mouse->GetMousePosition());
                    }
                }
            }
        }

        if (mouse->GetMouseButtonDown(MouseButton::kLeft))
        {
            UIInGameState* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState());
            if (state)
            {
                UIContextMenu* menu = state->GetContextMenu();
                menu->Hide();
            }
        }

        float now = SessionSubsystem::Get()->GetServerTime();
        
        auto it = buff_effects_.begin();
        while (it != buff_effects_.end())
        {
            if (it->second > now)
            {
                ++it;
                continue;
            }

            Logger::Print(L"Expired Buff: %d", it->first);
            it = buff_effects_.erase(it);
        }

        // 공격 범위 확인용
        DebugDrawHelper::Get()->DrawBox(GetTransform()->GetPosition(), { 3.f, 2.f }, Math::Color::Red);
    }
    else
    {
    }
}

void PlayerCharacter::StartCreateParty()
{
    UIPopup::PopupParam param;
    param.caption = L"파티 이름을 입력하세요.";
    param.option = UIPopup::PopupOption::OK | UIPopup::PopupOption::Cancel | UIPopup::PopupOption::Edit;
    param.callback = [this](const std::wstring& text,  UIPopup::PopupOption option)
    {
        if (option == UIPopup::PopupOption::OK)
        {
            PartyCreateRequest request;
            request.party_name = text;
            SendPacket(request);
        }
        return true;
    };
    UIPopup::ShowPopup(param);
}

void PlayerCharacter::SyncCharacterMovement(float delta_time)
{
    std::shared_ptr<TransformComponent> transform = GetTransform();
    
    if (IsMine())
    {
        Math::Vector2 position = transform->GetPosition();
        movement_sync_accumulator_ += delta_time;
        
        bool is_moving_now = last_position_ != position;
        bool is_moving_start = (is_moving_now && !was_moving_);
        bool is_interval_elapsed = (is_moving_now && movement_sync_accumulator_ >= 0.1f);
        
        // 전송할지 결정
        if (is_moving_start || is_interval_elapsed)
        {
            if (is_moving_start)
            {
                MovePlayerPacket dummy_packet;
                dummy_packet.position_x = last_position_.x;
                dummy_packet.position_y = last_position_.y;
                dummy_packet.velocity_x = velocity_.x;
                dummy_packet.velocity_y = velocity_.y;
                dummy_packet.server_time = SessionSubsystem::Get()->GetServerTime();
                dummy_packet.time_update = true;
                SendPacket(dummy_packet);
            }

            MovePlayerPacket move_player_packet;
            move_player_packet.position_x = position.x;
            move_player_packet.position_y = position.y;
            move_player_packet.velocity_x = velocity_.x;
            move_player_packet.velocity_y = velocity_.y;
            move_player_packet.server_time = SessionSubsystem::Get()->GetServerTime();
            move_player_packet.time_update = false;
            SendPacket(move_player_packet);

            was_moving_ = is_moving_now;
            last_position_ = position;
            movement_sync_accumulator_ = 0.f;
        }

        std::wstring current_anim = animator_->GetCurrentState()->GetName();
        bool is_flip = renderer_->IsFlipX();
        if (current_anim != last_animation_ || is_flip != last_flip_)
        {
            PlayerAnimationPacket anim_pkt;
            anim_pkt.is_flipped  = renderer_->IsFlipX();
            anim_pkt.animation   = current_anim;
            anim_pkt.server_time = SessionSubsystem::Get()->GetServerTime();
            SendPacket(anim_pkt);
            
            last_animation_ = current_anim;
            last_flip_ = is_flip;
        }
    }
    else
    {
        float server_now = SessionSubsystem::Get()->GetServerTime();

        float interpolation_time = server_now - EngineSettings::Get()->GetCharacterInterpolationDelay();

        // 오래된 스냅샷 제거
        while (movement_snapshots_.size() >= 2 &&
            movement_snapshots_[1].server_time < interpolation_time)
        {
            movement_snapshots_.pop_front(); 
        }

        if (movement_snapshots_.size() >= 2 &&
            movement_snapshots_[1].time_update)
        {
            movement_snapshots_.pop_front();
        }
        
        if (movement_snapshots_.size() >= 2)
        {
            if (movement_snapshots_[0].time_update)
            {
                movement_snapshots_[0].server_time = movement_snapshots_[1].server_time - EngineSettings::Get()->GetCharacterInterpolationDelay();
            }
            
            const MovementSnapshot& from = movement_snapshots_[0];
            const MovementSnapshot& to = movement_snapshots_[1];

            float t = (interpolation_time - from.server_time) / (to.server_time - from.server_time);
            t = Math::Clamp(t, 0.f, 1.f);

            Math::Vector2 position = Math::Vector2::Lerp(from.position, to.position, t);
            GetTransform()->SetPosition(position);
        }

        while (animation_snapshots_.size() >= 2 &&
           animation_snapshots_[1].server_time < interpolation_time)
        {
            animation_snapshots_.pop_front();
        }
        
        if (!animation_snapshots_.empty())
        {
            const auto& anim = animation_snapshots_.front();

            // 이전 스냅샷과 다를 경우에만 처리
            if ( prev_animation.server_time != anim.server_time)
            {
                renderer_->SetFlipX(anim.is_flipped);
                animator_->PlayAnimation(anim.animation);
                prev_animation = anim;
            }
        }
    }
}

void PlayerCharacter::OnFootstep() const
{
    Audio* audio = AssetManager::Get()->Load<Audio>(L"Audio\\SE\\move_default.mp3");
    AudioManager::Get()->PlaySound2D(audio, ChannelGroup::kSE);
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
