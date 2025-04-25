#pragma once
#include "Subsystems/GameInstanceSubsystem.h"

#define GET_IN_GAME_UI() GameInstance::Get()->GetSubsystem<InGameUISubsystem>()

namespace UI
{
    class ListBox;
    class EditableTextBox;
}

class InGameUISubsystem : public GameInstanceSubsystem
{
    GENERATED_BODY(InGameUISubsystem, Subsystem)
    
public:
    InGameUISubsystem();
    virtual ~InGameUISubsystem() override = default;

    virtual void Init() override;
    virtual void Deinit() override;

    FORCEINLINE std::shared_ptr<UI::ListBox> GetChatHistory() const { return chat_history_; }
    FORCEINLINE std::shared_ptr<UI::EditableTextBox> GetChatInput() const { return chat_input_; }

private:
#pragma region 위젯
    std::shared_ptr<UI::ListBox> chat_history_;
    std::shared_ptr<UI::EditableTextBox> chat_input_;
#pragma endregion
    
};
