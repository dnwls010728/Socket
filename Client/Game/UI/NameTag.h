#pragma once
#include "UI/Widget.h"

namespace UI_OLD
{
    class NameTag : public Widget
    {
        SHADER_CLASS_HELPER(NameTag)
        GENERATED_BODY(NameTag, Widget)
        
    public:
        NameTag(const std::wstring& name);
        virtual ~NameTag() override = default;

        void SetText(const std::wstring& text);
        
        FORCEINLINE const std::wstring& GetText() const { return text_; }

        static std::shared_ptr<NameTag> Create(const std::wstring& name);

    protected:
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;

    private:
        std::wstring text_;
        
        std::vector<float> advances_;

        float total_advance_;
    
    };
}
