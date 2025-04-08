#include "pch.h"
#include "Dummy.h"

#include "Actor/Component/CircleColliderComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "UI/UIManager.h"
#include "UI/Widget/TextBox.h"
#include "Windows/DX/Renderer.h"

Dummy::Dummy(const std::wstring& kName) :
    Actor(kName),
    text_box_(nullptr)
{
    collider_ = AddComponent<CircleColliderComponent>(L"CircleCollider");
    collider_->SetRadius(1.f);
    collider_->SetTrigger(true);
}

void Dummy::Show()
{
    if (!UI::Manager::Get()->IsInViewport(text_box_))
    {
        UI::Manager::Get()->AddToViewport(text_box_);
    }
}

void Dummy::Hide()
{
    if (UI::Manager::Get()->IsInViewport(text_box_))
    {
        UI::Manager::Get()->RemoveFromViewport(text_box_);
    }
}

void Dummy::BeginPlay()
{
    Actor::BeginPlay();

    text_box_ = UI::TextBox::Create(L"Text Box");
    text_box_->SetSize({100.f, 50.f});
    text_box_->SetText(L"Pick!");
}

void Dummy::Tick(float delta_time)
{
    Actor::Tick(delta_time);
    
    text_box_->SetPosition(Renderer::Get()->ConvertWorldToScreen(GetTransform()->GetPosition()));
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Dummy>("Dummy")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
