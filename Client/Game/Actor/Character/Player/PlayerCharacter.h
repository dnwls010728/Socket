#pragma once
#include "Actor/Character/CharacterBase.h"

class Text;

class PlayerCharacter : public CharacterBase
{
    SHADER_CLASS_HELPER(PlayerCharacter)
    GENERATED_BODY(PlayerCharacter, CharacterBase)
    
public:
    PlayerCharacter(const std::wstring& kName);
    virtual ~PlayerCharacter() override = default;

    void SetNickname(const std::wstring& kNickname);

    FORCEINLINE void SetPacketId(int packet_id) { packet_id_ = packet_id; }
    FORCEINLINE int GetPacketId() const { return packet_id_; }

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float delta_time) override;
    virtual void PostTick(float delta_time) override;
    
    class Sprite* sprite_;

    std::wstring nickname_;

    Text* nickname_text_;

    int packet_id_;
    
};

extern Type::uint32 current_player_id;
