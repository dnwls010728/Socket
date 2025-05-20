#pragma once
#include "Subsystem/GameInstanceSubsystem.h"

namespace UI_OLD
{
    class ListBox;
    class EditableTextBox;
    class MiniMap;
}

class InGameUISubsystem : public GameInstanceSubsystem
{
    GENERATED_BODY(InGameUISubsystem, GameInstanceSubsystem)
    
public:
    InGameUISubsystem();
    virtual ~InGameUISubsystem() override = default;

    virtual void Init() override;
    virtual void Deinit() override;

    void ShowChatUI();
    void HideChatUI();
    void ShowMiniMap();
    void HideMiniMap();

    FORCEINLINE std::shared_ptr<UI_OLD::EditableTextBox> GetChatInput() const { return chat_input_; }
    FORCEINLINE std::shared_ptr<UI_OLD::MiniMap> GetMiniMap() const { return mini_map_; }

    static InGameUISubsystem* Get();

private:
    void OnChatInputReturn(const std::wstring& text);
    
#pragma region 위젯
    std::shared_ptr<UI_OLD::EditableTextBox> chat_input_;
    std::shared_ptr<UI_OLD::MiniMap> mini_map_;
#pragma endregion
    
};
