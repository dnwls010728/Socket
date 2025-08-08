#pragma once
#include "UI/UIContainer.h"

class Tilemap;
class UIText;
class UIImage;

class UIMiniMap : public UIContainer
{
    GENERATED_BODY(UIMiniMap, UIContainer)
    
public:
    UIMiniMap(const std::wstring& name);
    virtual ~UIMiniMap() override = default;

    void SetTilemap(const Tilemap* tilemap);

protected:
    virtual void Init() override;
    virtual void Render() override;

private:
    UIImage* frame_;
    UIImage* map_;

    UIText* map_name_text_;
    
};
