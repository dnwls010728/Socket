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

    void SetTilemap(Tilemap* tilemap);

protected:
    virtual void Init() override;
    virtual void Render() override;

private:
    UIImage* frame_;
    UIImage* map_;

    UIText* map_name_text_;

    Tilemap* tilemap_;

    Math::Vector2 marker_scale_;
    
};
