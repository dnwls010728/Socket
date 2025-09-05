#include "pch.h"
#include "UIEquipment.h"

#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIText.h"
#include "Windows/DX/UISprite.h"

UIEquipment::UIEquipment(const std::wstring& name) :
    UIContainer(name)
{
    SetSize({164.f, 246.f});
    
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    
    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);
    
    UIText* t_title = AddChild<UIText>(UIText::StaticClass(), L"Title");
    t_title->SetRelativePosition({ 8.f, 0.f });
    t_title->SetSize({ 142.f, 20.f });
    t_title->SetColor(Math::Color::White);
    t_title->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    t_title->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    t_title->SetText(L"캐릭터");
    t_title->SetIgnoreRayCast(true);
}

void UIEquipment::Init()
{
    background_->SetSize(GetSize());
    
    UIContainer::Init();
}

bool UIEquipment::OnDragBegin(const Math::Vector2& position)
{
    return true;
}

bool UIEquipment::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    Math::Vector2 new_position = GetRelativePosition() + delta;
    SetRelativePosition(new_position);
    return true;
}

bool UIEquipment::OnDragEnd(const Math::Vector2& position)
{
    return true;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIEquipment>("UIEquipment")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
