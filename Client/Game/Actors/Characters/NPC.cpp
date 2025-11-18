#include "pch.h"
#include "NPC.h"

#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "Math/Color.h"
#include "Math/Math.h"
#include "Subsystems/DataSubsystem.h"
#include "Subsystems/SessionSubsystem.h"
#include "Time/Time.h"
#include "UI/Element/UINameTag.h"

NPC::NPC(const std::wstring& name) :
    CharacterBase(name),
    npc_id_(0),
    speeches_(),
    hide_duration_(5.f),
    show_duration_(5.f),
    is_showing_(false),
    timer_handle_(),
    last_pressed_time_(0.f)
{
    SetLayer(ActorLayer::kNPC);
}

void NPC::Init(uint32_t npc_id, const Math::Vector2& position)
{
    const NPCData* npc_data = DataSubsystem::Get()->GetNPC(npc_id);
    CHECK_IF(npc_id, L" NPC Data not found for id: %s", npc_id);

    npc_id_ = npc_id;

    for (const auto& speech : npc_data->speeches)
    {
        speeches_.emplace_back(speech);
    }

    AnimationPack* animation_pack = AssetManager::Get()->Load<AnimationPack>(npc_data->path);
    CHECK_IF(animation_pack, L" NPC AnimationPack not found for path: %s", npc_data->path.c_str());

    animator_->SetAnimationPack(animation_pack);
    animator_->PlayAnimation(L"Idle");

    GetTransform()->SetPosition(position);

    SetCharacterName(npc_data->name);
}

void NPC::BeginPlay()
{
    CharacterBase::BeginPlay();
    
    name_tag_->SetColor(Math::Color::Yellow);
    
    TimerManager::Get()->SetTimer(timer_handle_, [this]()
    {
        int32_t index = Math::RandRange(0, speeches_.size() - 1);
        Speak(speeches_[index], show_duration_);
    }, hide_duration_, false);
}

void NPC::Tick(float delta_time)
{
    CharacterBase::Tick(delta_time);

    Mouse* mouse = Mouse::Get();
    if (mouse->GetMouseButtonDown(MouseButton::kLeft))
    {
        if (Time::Seconds() - last_pressed_time_ < .2f)
        {
            ShopOpenRequest request;
            request.object_id = GetObjectID();
            
            SessionSubsystem::Get()->SendPacket(request);
        }
        else
        {
            last_pressed_time_ = Time::Seconds();
        }
    }
}

void NPC::EndPlay(EndPlayReason type)
{
    CharacterBase::EndPlay(type);
    
    TimerManager::Get()->ClearTimer(timer_handle_);
}

void NPC::OnSpeakEnd()
{
    CharacterBase::OnSpeakEnd();
    
    TimerManager::Get()->SetTimer(timer_handle_, [this]()
    {
        int32_t index = Math::RandRange(0, speeches_.size() - 1);
        Speak(speeches_[index], show_duration_);
    }, hide_duration_, false);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<NPC>("NPC")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
