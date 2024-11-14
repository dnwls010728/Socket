#include "pch.h"
#include "MainMap.h"

#include "Actor/Tilemap.h"
#include "Actor/Character/Player/PlayerCharacter.h"
#include "Actor/Network/Network.h"
#include "Input/Keyboard.h"
#include "Resource/ResourceManager.h"
#include "UI/Canvas.h"
#include "UI/Widget/EditableTextBox.h"
#include "UI/Widget/Image.h"
#include "UI/Widget/Text.h"
#include "Windows/DX/UITexture.h"

MainMap::MainMap(const std::wstring& kName) : Level(kName)
{
}

void MainMap::Load()
{
    Level::Load();
    
    Canvas* canvas = Canvas::Get();
    
    UITexture* texture = ResourceManager::Get()->Load<UITexture>(L"Sprites\\UI\\Panel.png");
    texture->SetSlice9Rect({10.f, 10.f, 44.f, 44.f});

    Image* chat_input_box_image = canvas->AddWidget<Image>(L"Chat Input Box Image");
    chat_input_box_image->AttachToWidget(canvas->GetRootWidget());
    chat_input_box_image->SetAnchorPreset(AnchorPreset::kStretch | AnchorPreset::kBottom, true);
    chat_input_box_image->SetSize({0.f, 50.f});
    chat_input_box_image->SetDrawMode(DrawMode::kSliced);
    chat_input_box_image->SetTexture(texture);

    chat_input_box_ = canvas->AddWidget<EditableTextBox>(L"Chat Input Box");
    chat_input_box_->AttachToWidget(chat_input_box_image);
    chat_input_box_->SetAnchorPreset(AnchorPreset::kStretch);
    chat_input_box_->SetAnchoredPosition({10.f, 10.f});
    chat_input_box_->SetSize({10.f, 10.f});
    chat_input_box_->SetPlaceholder(L"Enter chat message...");

    chat_text_ = canvas->AddWidget<Text>(L"Chat Text");
    chat_text_->AttachToWidget(canvas->GetRootWidget());
    chat_text_->SetAnchorPreset(AnchorPreset::kStretch | AnchorPreset::kTop, true);
    chat_text_->SetText(L"");

    Network* network = AddActor<Network>(L"Network");
    Tilemap* tilemap = AddActor<Tilemap>(L"Tilemap");
    PlayerCharacter* player = AddActor<PlayerCharacter>(L"Player");
}

void MainMap::Tick(float delta_time)
{
    Level::Tick(delta_time);

    Keyboard* keyboard = Keyboard::Get();
    if (chat_input_box_->IsFocused() && keyboard->GetKeyDown(VK_RETURN))
    {
        std::wstring temp = chat_text_->GetText();
        temp += chat_input_box_->GetText() + L"\n";
        
        chat_text_->SetText(temp);
        chat_input_box_->SetText(L"");
    }

}

RTTR_REGISTRATION
{
    rttr::registration::class_<MainMap>("MainMap")
        .constructor<const std::wstring&>()
        (
            rttr::policy::ctor::as_std_shared_ptr
        );
}
