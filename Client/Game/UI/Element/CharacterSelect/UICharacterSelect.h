#pragma once
#include "UI/UIContainer.h"
#include "UI/Element/UIImage.h"
#include "UI/Model/CharacterSelectModel.h"

class UIButton;
class UICharacterSlot;

class UICharacterSelect : public UIContainer
{
    GENERATED_BODY(UICharacterSelect, UIContainer)
    
public:
    UICharacterSelect(const std::wstring& name);
    virtual ~UICharacterSelect() override = default;

    void InitSlots(const std::vector<CharacterProfile>& profiles);

protected:
    virtual void Init() override;
    virtual void Render() override;

private:
    friend class UICharacterSlot;
    
    void OnSlotSelected(uint32_t slot_id);
    void OnCharacterSelected() const;
    
    UIImage* background_;
    
    UIButton* delete_button_;
    UIButton* new_button_;
    UIButton* select_button_;
    
    std::vector<UICharacterSlot*> slots_;

    uint32_t selected_slot_id_;
};
