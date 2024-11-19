#include "pch.h"
#include "MainMenu.h"

#include "../../SocketCore/SocketSession.h"
#include "../SocketCore/ServerPacketHandler.h"
#include "Level/World.h"
#include "Resource/ResourceManager.h"
#include "UI/Canvas.h"
#include "UI/Widget.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/EditableTextBox.h"
#include "UI/Widget/Image.h"
#include "UI/Widget/ScrollBox.h"
#include "UI/Widget/Text.h"
#include "Windows/WindowsWindow.h"
#include "Windows/DX/UITexture.h"

MainMenu::MainMenu(const std::wstring& kName) :
    Level(kName)
{
}

void MainMenu::Load()
{
    Level::Load();
    
    Canvas* canvas = Canvas::Get();

    UITexture* texture = ResourceManager::Get()->Load<UITexture>(L"Sprites\\UI\\Panel.png");
    texture->SetSlice9Rect({10.f, 10.f, 44.f, 44.f});

    ScrollBox* scroll_box = canvas->AddWidget<ScrollBox>(L"Scroll Box");
    scroll_box->AttachToWidget(canvas->GetRootWidget());
    scroll_box->SetAnchorPreset(AnchorPreset::kLeft | AnchorPreset::kTop, true);
    scroll_box->SetAnchoredPosition({0.f, 0.f});
    scroll_box->SetSize({100.f, 200.f});

    // Scroll Box 테스트 코드
    for (int i = 0; i < 10; ++i)
    {
        Image* id_image = canvas->AddWidget<Image>(L"ID Image");
        id_image->AttachToWidget(scroll_box);
        id_image->SetAnchoredPosition({-50.f, -25.f});
        id_image->SetSize({200.f, 50.f});
        id_image->SetDrawMode(DrawMode::kSliced);
        id_image->SetTexture(texture);
    
        EditableTextBox* id_text_box = canvas->AddWidget<EditableTextBox>(L"ID Text Box");
        id_text_box->AttachToWidget(id_image);
        id_text_box->SetAnchorPreset(AnchorPreset::kStretch);
        id_text_box->SetAnchoredPosition({10.f, 10.f});
        id_text_box->SetSize({10.f, 10.f});
        id_text_box->SetPlaceholder(L"ID");
    }

    Image* id_image = canvas->AddWidget<Image>(L"ID Image");
    id_image->AttachToWidget(canvas->GetRootWidget());
    id_image->SetAnchoredPosition({-50.f, -25.f});
    id_image->SetSize({200.f, 50.f});
    id_image->SetDrawMode(DrawMode::kSliced);
    id_image->SetTexture(texture);
    
    EditableTextBox* id_text_box = canvas->AddWidget<EditableTextBox>(L"ID Text Box");
    id_text_box->AttachToWidget(id_image);
    id_text_box->SetAnchorPreset(AnchorPreset::kStretch);
    id_text_box->SetAnchoredPosition({10.f, 10.f});
    id_text_box->SetSize({10.f, 10.f});
    id_text_box->SetPlaceholder(L"ID");

    Image* pw_image = canvas->AddWidget<Image>(L"Password Image");
    pw_image->AttachToWidget(canvas->GetRootWidget());
    pw_image->SetAnchoredPosition({-50.f, 25.f});
    pw_image->SetSize({200.f, 50.f});
    pw_image->SetDrawMode(DrawMode::kSliced);
    pw_image->SetTexture(texture);

    EditableTextBox* pw_text_box = canvas->AddWidget<EditableTextBox>(L"Password Text Box");
    pw_text_box->AttachToWidget(pw_image);
    pw_text_box->SetAnchorPreset(AnchorPreset::kStretch);
    pw_text_box->SetAnchoredPosition({10.f, 10.f});
    pw_text_box->SetSize({10.f, 10.f});
    pw_text_box->SetPlaceholder(L"Password");
    pw_text_box->SetContentType(ContentType::Password);
    
    Button* login_button = canvas->AddWidget<Button>(L"Login Button");
    login_button->AttachToWidget(canvas->GetRootWidget());
    login_button->SetAnchoredPosition({100.f, 0.f});
    login_button->SetSize({100.f, 100.f});
    login_button->SetTexture(texture);
    login_button->SetDrawMode(DrawMode::kSliced);
    login_button->OnMouseReleased.Add([]()
    {
        if(!GSocketSession->Connect())
        {
            //TODO: 여기서 튕기는 코드 작성 해 주세요
        }
        else
        {
            C_EnterPacket pkt;
            pkt.SetId("Sundaekyung");
            pkt.SetName("Sundaekyung");
            std::shared_ptr<SendBuffer> sendBuffer = ServerPacketHandler::MakeSendBuffer<C_EnterPacket>(pkt,C_PKT_ENTER);
            GSocketSession->Send(sendBuffer);
        }
        
        World::Get()->OpenLevel(LevelType::kDefault);
    });

    Text* login_text = canvas->AddWidget<Text>(L"Login Text");
    login_text->AttachToWidget(login_button);
    login_text->SetAnchorPreset(AnchorPreset::kStretch);
    login_text->SetSize({0.f, 0.f});
    login_text->SetColor(Math::Color::Black);
    login_text->SetText(L"LOGIN");
    login_text->SetAlignment(Text::kMiddleCenter);

    Button* editor_button = canvas->AddWidget<Button>(L"Editor Button");
    editor_button->AttachToWidget(canvas->GetRootWidget());
    editor_button->SetAnchoredPosition({-75.f, 75.f});
    editor_button->SetTexture(texture);
    editor_button->SetDrawMode(DrawMode::kSliced);
    editor_button->OnMouseReleased.Add([]()
    {
        World::Get()->OpenLevel(LevelType::kEditor);
    });
    
    Text* editor_text = canvas->AddWidget<Text>(L"Editor Text");
    editor_text->AttachToWidget(editor_button);
    editor_text->SetAnchorPreset(AnchorPreset::kStretch);
    editor_text->SetSize({0.f, 0.f});
    editor_text->SetColor(Math::Color::Black);
    editor_text->SetText(L"EDITOR");
    editor_text->SetAlignment(Text::kMiddleCenter);
    
    Button* exit_button = canvas->AddWidget<Button>(L"Exit Button");
    exit_button->AttachToWidget(canvas->GetRootWidget());
    exit_button->SetAnchoredPosition({75.f, 75.f});
    exit_button->SetTexture(texture);
    exit_button->SetDrawMode(DrawMode::kSliced);
    exit_button->OnMouseReleased.Add([]()
    {
        WindowsWindow* window = World::Get()->GetWindow();
        PostMessage(window->GetHWnd(), WM_USER, 0, 0);
    });
    
    Text* exit_text = canvas->AddWidget<Text>(L"Exit Text");
    exit_text->AttachToWidget(exit_button);
    exit_text->SetAnchorPreset(AnchorPreset::kStretch);
    exit_text->SetSize({0.f, 0.f});
    exit_text->SetColor(Math::Color::Black);
    exit_text->SetText(L"EXIT");
    exit_text->SetAlignment(Text::kMiddleCenter);
    
}

RTTR_REGISTRATION
{
    rttr::registration::class_<MainMenu>("MainMenu")
        .constructor<const std::wstring&>()
        (
            rttr::policy::ctor::as_std_shared_ptr
        );
}
