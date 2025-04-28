#pragma once
#include "UI/Widget.h"

namespace UI
{
    class MiniMap : public Widget
    {
        SHADER_CLASS_HELPER(MiniMap)
        GENERATED_BODY(MiniMap, Widget)
        
    public:
        MiniMap(const std::wstring& name);
        virtual ~MiniMap() override = default;

        static std::shared_ptr<MiniMap> Create(const std::wstring& name);

    protected:
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;
    
    };
}
