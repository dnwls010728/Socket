#pragma once
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "UI/UIContainer.h"
#include "UI/Element/UIImage.h"

class UIButton;
class UICharacterSlot;

class UICharacterSelect : public UIContainer
{
    GENERATED_BODY(UICharacterSelect, UIContainer)
    
public:
    UICharacterSelect(const std::wstring& name);
    virtual ~UICharacterSelect() override = default;

    void InitSlots(const std::vector<CharacterProfile>& profile);
    void RefreshSlots(const std::vector<CharacterProfile>& profile);

protected:
    virtual void Init() override;
    virtual void Render() override;

private:
    friend class UICharacterSlot;

    void OnCreateCharacter();
    void OnDeleteCharacter() const;
    void OnSlotSelected(uint32_t slot_id);
    void OnCharacterSelected() const;
    
    UIImage* background_;
    
    UIButton* delete_button_;
    UIButton* new_button_;
    UIButton* select_button_;
    
    std::vector<UICharacterSlot*> slots_;

    uint32_t selected_slot_id_;
};
