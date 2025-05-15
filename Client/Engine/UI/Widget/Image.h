#pragma once
#include "UI/Widget.h"

class UITexture;

namespace UI_OLD
{
    class Image : public Widget
    {
        SHADER_CLASS_HELPER(Image)
        GENERATED_BODY(Image, Widget)
        
    public:
        Image(const std::wstring& kName);
        virtual ~Image() override = default;

        void SetTexture(const std::wstring& path);

        static std::shared_ptr<Image> Create(const std::wstring& name);

    protected:
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;

    private:
        UITexture* texture_;
    
    };
}
