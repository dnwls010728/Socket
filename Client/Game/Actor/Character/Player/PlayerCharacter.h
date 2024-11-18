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
    FORCEINLINE void SetLastRecentPosition(Math::Vector2 v){last_recent_position_=v;}
    FORCEINLINE void SetIsPostionUpdated(bool desire){is_position_updated_ = desire;}

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float delta_time) override;
    virtual void PostTick(float delta_time) override;
    
    class Sprite* sprite_;

    std::wstring nickname_;

    Text* nickname_text_;

    int packet_id_;

    Math::Vector2 last_recent_position_;
    bool is_position_updated_;
    
};

extern Type::uint32 current_player_id;
