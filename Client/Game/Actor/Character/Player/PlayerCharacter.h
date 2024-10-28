#pragma once
#include "Actor/Character/CharacterBase.h"
#include "UI/Widget/Text.h"

namespace UI
{
    class Text;
}
extern uint32_t currentPlayerId;
class PlayerCharacter : public CharacterBase
{
    SHADER_CLASS_HELPER(PlayerCharacter)
    GENERATED_BODY(PlayerCharacter, CharacterBase)
    
public:
    PlayerCharacter(const std::wstring& kName);
    virtual ~PlayerCharacter() override = default;

    FORCEINLINE void SetPacketId (int packetId) { packet_id_ = packetId; }
    FORCEINLINE int GetPacketId () const { return packet_id_; }
    FORCEINLINE void SetNickname(const std::wstring& kNick)
    {
        nickname_widget_->SetText(kNick);
    }

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float delta_time) override;
    virtual void PostTick(float delta_time) override;
    
    class Sprite* sprite_;

    UI::Text* nickname_widget_;

    int packet_id_;
    
};
