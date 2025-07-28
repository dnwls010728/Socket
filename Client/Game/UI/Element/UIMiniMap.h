#pragma once
#include "UI/UIContainer.h"

class UIText;
class UIImage;

class UIMiniMap : public UIContainer
{
    GENERATED_BODY(UIMiniMap, UIContainer)
    
public:
    enum class MarkerType : uint8_t
    {
        kNone = 0,
        kPlayer,
        kOtherPlayer
    };
    
    struct Marker
    {
        MarkerType type;
        Math::Vector2 position;
    };
    
    UIMiniMap(const std::wstring& name);
    virtual ~UIMiniMap() override = default;

    FORCEINLINE void SetMarkers(const std::vector<Marker>& markers) { markers_ = markers; }

protected:
    virtual void Init() override;
    virtual void Render() override;

private:
    UIImage* frame_;
    UIImage* map_;

    UIText* map_name_text_;

    std::vector<Marker> markers_;
    
};
