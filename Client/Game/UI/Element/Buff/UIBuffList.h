#pragma once
#include "Subsystems/Publisher/PublisherSubsystem.h"
#include "UI/UIContainer.h"

class UIBuffIcon;

class UIBuffList : public UIContainer
{
    GENERATED_BODY(UIBuffList, UIContainer)
    
public:
    UIBuffList(const std::wstring& name);
    virtual ~UIBuffList() override = default;

    UIBuffIcon* AddBuff(int32_t id, float expire_time);

private:
    void Init() override;
    void Uninit() override;
    void OnEvent(const EventData& data);
    void UpdateLayout();

    std::unordered_map<int32_t, UIBuffIcon*> buff_icons;
    
};
