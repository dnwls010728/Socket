#pragma once
#include "Actor/Character/CharacterBase.h"
#include "Actor/Character/Player/PlayerCharacter.h"
#include "Actor/Component/TransformComponent.h"
#include "Level/World.h"
class Text;


class PlayerCharacter : public CharacterBase
{
    SHADER_CLASS_HELPER(PlayerCharacter)
    GENERATED_BODY(PlayerCharacter, CharacterBase)
    
public:
    PlayerCharacter(const std::wstring& kName);
    virtual ~PlayerCharacter() override = default;

    FORCEINLINE void SetPacketId(int packet_id) { packet_id_ = packet_id; }
    FORCEINLINE int GetPacketId() const { return packet_id_; }

protected:
    virtual void BeginPlay() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;

private:
    class Sprite* sprite_;

    int horizontal_axis_;

    float move_speed_;
    int packet_id_;

    Math::Vector2 last_recent_position_;
    bool is_position_updated_;
    
};

extern Type::uint32 current_player_id;
