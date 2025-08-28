#include "pch.h"
#include "UIBuffIcon.h"

#include "Asset/AssetManager.h"
#include "Math/Math.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/UISprite.h"

UIBuffIcon::UIBuffIcon(const std::wstring& name) :
    UIContainer(name),
    expire_time_(0.f)
{
    SetSize({ 32.f, 32.f });
    
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");

    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Frame");
    background_->SetSize(GetSize());
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);

    icon_ = AddChild<UIImage>(UIImage::StaticClass(), L"Icon");
    icon_->SetSize(GetSize());
    
    text_ = AddChild<UIText>(UIText::StaticClass(), L"Text");
    text_->SetSize(GetSize());
    text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    text_->SetColor(Math::Color::Red);
}

void UIBuffIcon::Init(int32_t id, float expire_time)
{
    if (id < 0) // 아이템 버프
    {
    }
    else if (id > 0) // 스킬 버프
    {
    }
    
    expire_time_ = expire_time;
}

void UIBuffIcon::Tick(float delta_time)
{
    UIContainer::Tick(delta_time);
    
    if (expire_time_ > 0.f && Math::IsEqual(GetRemainingTime(), 0.f))
        RemoveFromParent();

    text_->SetText(std::to_wstring(static_cast<int32_t>(GetRemainingTime()) + 1));
}

float UIBuffIcon::GetRemainingTime() const
{
    float server_now = SessionSubsystem::Get()->GetServerTime();
    return Math::Max(expire_time_ - server_now, 0.f);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIBuffIcon>("UIBuffIcon")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
