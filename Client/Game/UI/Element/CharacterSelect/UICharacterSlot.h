#pragma once
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "UI/UIContainer.h"

class UICharacterSelect;
class UIText;
class UIImage;

class UICharacterSlot : public UIContainer
{
    GENERATED_BODY(UICharacterSlot, UIContainer)
    
public:
    UICharacterSlot(const std::wstring& name);
    virtual ~UICharacterSlot() override = default;

    void InitSlot(UICharacterSelect* character_select, uint32_t slot_id, const CharacterProfile& profile);
    void ClearSlot();

    FORCEINLINE uint32_t GetSlotID() const { return slot_id_; }
    FORCEINLINE uint32_t GetCharacterID() const { return character_id_; }

protected:
    virtual void Init() override;
    virtual void Tick(float delta_time) override;

    virtual bool OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp) override;

private:
    UICharacterSelect* character_select_;
    
    UIImage* background_;
    UIImage* character_;

    UIText* empty_text_;
    UIText* lv_text_;
    UIText* name_text_;
    UIText* color_code_text_;

    uint32_t slot_id_;
    uint32_t character_id_;

    double last_time_;
    
    float timer_;

    int32_t frame_index_;
    
};
