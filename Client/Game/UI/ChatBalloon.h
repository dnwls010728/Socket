#pragma once
#include "UI/Widget.h"

class UITexture;

namespace UI_OLD
{
    class ChatBalloon : public Widget
    {
        SHADER_CLASS_HELPER(ChatBalloon)
        GENERATED_BODY(ChatBalloon, Widget)
        
    public:
        ChatBalloon(const std::wstring& name);
        virtual ~ChatBalloon() override = default;

        void SetText(const std::wstring& text);

        static std::shared_ptr<ChatBalloon> Create(const std::wstring& name);

    protected:
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;

    private:
        UITexture* arrow_texture_;
        UITexture* body_texture_;

        std::wstring text_;

        std::vector<float> advances_;

        float total_advance_;
    
    };
}
