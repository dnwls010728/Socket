#include "pch.h"
#include "ServerList.h"

#include "Math/Color.h"
#include "Windows/DX/Renderer.h"

UI::ServerList::ServerList(const std::wstring& kName) :
    Widget(kName),
    scroll_offset_y_(0.f)
{
}

void UI::ServerList::Render(Renderer* renderer, WindowsWindow* window)
{
    Widget::Render(renderer, window);

    const Math::Rect kRect = GetRect();

    renderer->DrawBox(window, kRect, Math::Vector2::Zero(), Math::Color::Black, 0.f, 1.f);

    renderer->BeginLayer(GetRect());

    for (Type::uint32 i = 0; i < 10; ++i)
    {
        Math::Rect item_rect = {
            kRect.x,
            kRect.y + (i * 30.f) + scroll_offset_y_,
            kRect.width,
            30.f
        };

        renderer->DrawString(window, L"Server " + std::to_wstring(i), item_rect, {0.f, 1.f}, Math::Color::White, 0.f, L"Nanum18", DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }

    if (IsFocused())
    {
        Math::Vector2 mouse_position = Mouse::Get()->GetMousePosition();
        for (Type::uint32 i = 0; i < 10; ++i)
        {
            Math::Rect item_rect = {
                kRect.x,
                kRect.y + (i * 30.f) + scroll_offset_y_,
                kRect.width,
                30.f
            };

            if (Math::Rect::Contains(item_rect, mouse_position))
            {
                renderer->DrawSolidBox(window, item_rect, {.5f, 1.f}, Math::Color(255, 255, 255, 100), 0.f);
            }
        }
    }
    
    renderer->EndLayer();
}

bool UI::ServerList::OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed)
{
    Widget::OnMouseButton(kPosition, button, is_pressed);

    if (button == MouseButton::kLeft && is_pressed)
    {
        Math::Rect kRect = GetRect();
        for (Type::uint32 i = 0; i < 10; ++i)
        {
            Math::Rect item_rect = {
                kRect.x,
                kRect.y + (i * 30.f) + scroll_offset_y_,
                kRect.width,
                30.f
            };

            if (Math::Rect::Contains(item_rect, kPosition))
            {
                // Handle item click
                Logger::Print(L"Clicked on Server %d", i);
            }
        }
        
        return true;
    }
    
    return false;
}

bool UI::ServerList::OnScroll(const Math::Vector2& kPosition, const Math::Vector2& kVelocity)
{
    scroll_offset_y_ += kVelocity.y * 10.f;
    
    return true;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI::ServerList>("UI::ServerList")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
