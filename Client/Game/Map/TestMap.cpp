#include "pch.h"
#include "TestMap.h"

#include "Math/Math.h"
#include "UI/Canvas.h"
#include "UI/Widget/EditableTextBox.h"
#include "UI/Widget/Image.h"
#include "UI/Widget/ScrollBox.h"
#include "UI/Widget/Text.h"

TestMap::TestMap(const std::wstring& kName) :
    Level(kName),
    message_input_background_(nullptr),
    message_history_(nullptr),
    resize_handle_(nullptr),
    message_input_(nullptr),
    message_scroll_area_(nullptr)
{
}

void TestMap::Load()
{
    Level::Load();

    Canvas* canvas = Canvas::Get();

    message_input_background_ = canvas->AddWidget<Image>(L"MessageInputBackground");
    message_input_background_->AttachToWidget(canvas->GetRootWidget());
    message_input_background_->SetAnchorPreset(AnchorPreset::kBottom | AnchorPreset::kStretch, true);
    message_input_background_->SetAnchoredPosition({0.f, 0.f});
    message_input_background_->SetSize({0.f, 24.f});

    message_input_ = canvas->AddWidget<EditableTextBox>(L"MessageInput");
    message_input_->AttachToWidget(message_input_background_);
    message_input_->SetAnchorPreset(AnchorPreset::kStretch);
    message_input_->SetAnchoredPosition({0.f, 0.f});
    message_input_->SetSize({0.f, 0.f});
    message_input_->SetPlaceholder(L"메시지를 입력해 주세요...");
    
    message_history_ = canvas->AddWidget<Image>(L"MessageHistory");
    message_history_->AttachToWidget(canvas->GetRootWidget());
    message_history_->SetAnchorPreset(AnchorPreset::kBottom | AnchorPreset::kStretch, true);
    message_history_->SetAnchoredPosition({0.f, -24.f});
    message_history_->SetSize({0.f, 48.f});
    message_history_->SetColor({255, 255, 255, 100});

    message_scroll_area_ = canvas->AddWidget<ScrollBox>(L"MessageScrollArea");
    message_scroll_area_->AttachToWidget(message_history_);
    message_scroll_area_->SetAnchorPreset(AnchorPreset::kStretch);
    message_scroll_area_->SetAnchoredPosition({0.f, 0.f});
    message_scroll_area_->SetSize({0.f, 0.f});

    resize_handle_ = canvas->AddWidget<Image>(L"ResizeHandle");
    resize_handle_->AttachToWidget(canvas->GetRootWidget());
    resize_handle_->SetAnchorPreset(AnchorPreset::kBottom | AnchorPreset::kStretch, true);
    resize_handle_->SetAnchoredPosition({0.f, -72.f});
    resize_handle_->SetSize({0.f, 16.f});
    resize_handle_->SetRayCastTarget(true);
    resize_handle_->OnDrag.Add([=](const Math::Vector2& kDelta)
    {
        Math::Vector2 position = resize_handle_->GetAnchoredPosition();
        position.y = Math::Clamp(position.y + kDelta.y, -196.f, -72.f);
        resize_handle_->SetAnchoredPosition(position);

        Math::Vector2 size = message_history_->GetSize();
        size.y = Math::Clamp(size.y - kDelta.y, 48.f, 172.f);
        message_history_->SetSize(size);
    });

    for (Type::uint32 i = 0; i < 10; ++i)
    {
        Text* text = canvas->AddWidget<Text>(L"MessageText" + std::to_wstring(i));
        text->AttachToWidget(message_scroll_area_);
        text->SetSize({300.f, 30.f});
        text->SetAlignment(Text::kMiddleLeft);
        text->SetText(std::to_wstring(i) + L" - Hello World!");
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<TestMap>("TestMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
