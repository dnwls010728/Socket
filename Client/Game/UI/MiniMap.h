#pragma once
#include "UI/Widget.h"

class Tilemap;

namespace UI_OLD
{
    class MiniMap : public Widget
    {
        SHADER_CLASS_HELPER(MiniMap)
        GENERATED_BODY(MiniMap, Widget)
        
    public:
        MiniMap(const std::wstring& name);
        virtual ~MiniMap() override = default;

        void SetTilemap(Tilemap* tilemap);

        FORCEINLINE Tilemap* GetTilemap() const { return tilemap_; }

        static std::shared_ptr<MiniMap> Create(const std::wstring& name);

    protected:
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;

    private:
        Tilemap* tilemap_;
    
    };
}
