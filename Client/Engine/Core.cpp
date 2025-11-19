#include "pch.h"
#include "Core.h"

#include "GameEngine.h"
#include "Audio/AudioManager.h"
#include "Event/EventManager.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Math/Vector2.h"
#include "Asset/AssetManager.h"
#include "imgui/imgui.h"
#include "Time/Time.h"
#include "UI/UI.h"
#include "Windows/WindowDefinition.h"
#include "Windows/WindowsWindow.h"
#include "Windows/DX/Renderer.h"

ImGuiKey MapToImGuiKey(uint32_t vk, uint32_t scancode, bool is_extended);

Core::Core() :
    game_window_(),
    game_engine_(nullptr),
    is_running_(false),
    main_thread_(),
    mutex_(),
    current_time_(0.),
    last_time_(0.),
    delta_time_(0.)
{
}

void Core::Init()
{
    ProjectSettings::Get()->Init();
    
    WindowsApplication* application = WindowsApplication::Get();
    application->AddMessageHandler(*this);

    // DirectX 11 렌더러 초기화
    CHECK_IF(Renderer::Get()->Init(), L"Failed to initialize renderer.");

    const int32_t screen_width = GetSystemMetrics(SM_CXSCREEN);
    const int32_t screen_height = GetSystemMetrics(SM_CYSCREEN);

    EngineSettings* settings = EngineSettings::Get();

    // 게임 윈도우 정의 생성
    std::shared_ptr<WindowDefinition> definition = std::make_shared<WindowDefinition>();
    definition->title = settings->GetWindowTitle();
    definition->screen_x = screen_width * .5f - settings->GetScreenWidth() * .5f;
    definition->screen_y = screen_height * .5f - settings->GetScreenHeight() * .5f;
    definition->width = settings->GetScreenWidth();
    definition->height = settings->GetScreenHeight();

    // 게임 윈도우 생성
    std::shared_ptr<WindowsWindow> new_window = application->MakeWindow();
    application->InitWindow(new_window, definition, nullptr);

    // 렌더러에 뷰포트 생성
    CHECK_IF(Renderer::Get()->CreateViewport(new_window, {definition->width, definition->height}), L"Failed to create viewport.");
    CHECK_IF(Renderer::Get()->CreateD2DViewport(new_window), L"Failed to create D2D viewport.");

    game_window_ = new_window;

    // 게임 엔진 생성
    game_engine_ = std::make_shared<GameEngine>();
    game_engine_->Init(new_window);

    current_time_ = Time::Init();

    Start();
}

// extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool Core::ProcessMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, uint32_t handler_result)
{
    // if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)) return true;
    if (EventManager::Get()->ProcessMessage(hWnd, message, wParam, lParam, handler_result)) return true;

    if (message == WM_SETFOCUS)
    {
        AudioManager::Get()->SetAllMutes(false);
        return true;
    }

    if (message == WM_KILLFOCUS)
    {
        EventManager::Get()->Clear();
        Keyboard::Get()->Clear();
        Mouse::Get()->Clear();
        
        AudioManager::Get()->SetAllMutes(true);
        return true;
    }

    if (message == WM_DESTROY)
    {
        if (const auto kWindow = game_window_.lock())
        {
            if (kWindow->GetHWnd() == hWnd)
            {
                Stop();

                World::Get()->Release();
                Renderer::Get()->Release();
                AssetManager::Get()->Release();
            }
        }
        return true;
    }

    return false;
}

void Core::MainThread()
{
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (!is_running_)
            {
                game_engine_->OnQuit();
                break;
            }
        }
        
#pragma region DeltaTime
        last_time_ = current_time_;
        current_time_ = Time::Seconds();

        double elapsed_time = current_time_ - last_time_;
        delta_time_ = elapsed_time;
#pragma endregion
        
        if (const auto& kWindow = game_window_.lock())
        {
            ImGuiIO& io = ImGui::GetIO();
            
            // Windows 이벤트 처리
            Event event;
            while (EventManager::Get()->PollEvent(event))
            {
                const uint32_t& kType = event.type;
                if (kType == static_cast<uint32_t>(EventType::kWindowSize))
                {
                    const WindowEvent& kWindowEvent = event.window;
            
                    Renderer::Get()->ResizeViewport(kWindow, kWindowEvent.data1, kWindowEvent.data2);
                }

                Keyboard::Get()->OnEvent(event);
                Mouse::Get()->OnEvent(event);
                UI::Get()->OnEvent(event);

                switch (event.type)
                {
                case static_cast<uint32_t>(EventType::kMouseMotion):
                    io.AddMousePosEvent(event.motion.x, event.motion.y);
                    break;
                case static_cast<uint32_t>(EventType::kMouseWheel):
                    io.AddMouseWheelEvent(event.wheel.x, event.wheel.y);
                    break;
                case static_cast<uint32_t>(EventType::kMousePressed):
                    io.AddMouseButtonEvent(static_cast<int32_t>(event.button.button), true);
                    break;
                case static_cast<uint32_t>(EventType::kMouseReleased):
                    io.AddMouseButtonEvent(static_cast<int32_t>(event.button.button), false);
                    break;
                case static_cast<uint32_t>(EventType::kKeyPressed):
                    io.AddKeyEvent(MapToImGuiKey(event.key.key_code, event.key.scancode, true), true);
                    break;
                case static_cast<uint32_t>(EventType::kKeyReleased):
                    io.AddKeyEvent(MapToImGuiKey(event.key.key_code, event.key.scancode, true), false);
                    break;
                case static_cast<uint32_t>(EventType::kText):
                    io.AddInputCharacter(event.text.character);
                    break;
                // case EventType::kWindowSize: /* 필요시 io.DisplaySize 갱신 or 백엔드가 처리 */
                //     break;
                // case EventType::kFocusGained:
                //     io.AddFocusEvent(true);
                //     break;
                // case EventType::kFocusLost:
                //     io.AddFocusEvent(false);
                //     break;
                }
            }
            
            game_engine_->GameLoop(delta_time_);
            
            Keyboard::Get()->UpdateKeyStates();
            Mouse::Get()->UpdateButtonStates();
        }
    }
}

void Core::Start()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        is_running_ = true;
    }
    
    main_thread_ = std::thread(&Core::MainThread, this);
}

void Core::Stop()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        is_running_ = false;
    }
    
    if (main_thread_.joinable())
    {
        main_thread_.join();
    }
}

ImGuiKey MapToImGuiKey(uint32_t vk, uint32_t scancode, bool is_extended)
{
    if (vk >= '0' && vk <= '9') return static_cast<ImGuiKey>(ImGuiKey_0 + (vk - '0'));
    if (vk >= 'A' && vk <= 'Z') return static_cast<ImGuiKey>(ImGuiKey_A + (vk - 'A'));

    if (vk >= VK_F1 && vk <= VK_F12) return static_cast<ImGuiKey>(ImGuiKey_F1 + (vk - VK_F1));

    switch (vk)
    {
    case VK_TAB: return ImGuiKey_Tab;
    case VK_LEFT: return ImGuiKey_LeftArrow;
    case VK_RIGHT: return ImGuiKey_RightArrow;
    case VK_UP: return ImGuiKey_UpArrow;
    case VK_DOWN: return ImGuiKey_DownArrow;
    case VK_PRIOR: return ImGuiKey_PageUp;
    case VK_NEXT: return ImGuiKey_PageDown;
    case VK_HOME: return ImGuiKey_Home;
    case VK_END: return ImGuiKey_End;
    case VK_INSERT: return ImGuiKey_Insert;
    case VK_DELETE: return ImGuiKey_Delete;
    case VK_BACK: return ImGuiKey_Backspace;
    case VK_SPACE: return ImGuiKey_Space;
    case VK_RETURN: return is_extended ? ImGuiKey_KeypadEnter : ImGuiKey_Enter;
    case VK_ESCAPE: return ImGuiKey_Escape;

    case VK_SHIFT: return (scancode == 0x36 ? ImGuiKey_RightShift : ImGuiKey_LeftShift);
    case VK_CONTROL: return (is_extended ? ImGuiKey_RightCtrl : ImGuiKey_LeftCtrl);
    case VK_MENU: return (is_extended ? ImGuiKey_RightAlt : ImGuiKey_LeftAlt);
    case VK_LWIN: return ImGuiKey_LeftSuper;
    case VK_RWIN: return ImGuiKey_RightSuper;
    case VK_APPS: return ImGuiKey_Menu;

    case VK_CAPITAL: return ImGuiKey_CapsLock;
    case VK_SCROLL: return ImGuiKey_ScrollLock;
    case VK_NUMLOCK: return ImGuiKey_NumLock;
    case VK_SNAPSHOT: return ImGuiKey_PrintScreen;
    case VK_PAUSE: return ImGuiKey_Pause;

    case VK_NUMPAD0: return ImGuiKey_Keypad0;
    case VK_NUMPAD1: return ImGuiKey_Keypad1;
    case VK_NUMPAD2: return ImGuiKey_Keypad2;
    case VK_NUMPAD3: return ImGuiKey_Keypad3;
    case VK_NUMPAD4: return ImGuiKey_Keypad4;
    case VK_NUMPAD5: return ImGuiKey_Keypad5;
    case VK_NUMPAD6: return ImGuiKey_Keypad6;
    case VK_NUMPAD7: return ImGuiKey_Keypad7;
    case VK_NUMPAD8: return ImGuiKey_Keypad8;
    case VK_NUMPAD9: return ImGuiKey_Keypad9;
    case VK_DECIMAL: return ImGuiKey_KeypadDecimal;
    case VK_DIVIDE: return ImGuiKey_KeypadDivide;
    case VK_MULTIPLY: return ImGuiKey_KeypadMultiply;
    case VK_SUBTRACT: return ImGuiKey_KeypadSubtract;
    case VK_ADD: return ImGuiKey_KeypadAdd;

    case VK_OEM_1: return ImGuiKey_Semicolon;
    case VK_OEM_PLUS: return ImGuiKey_Equal;
    case VK_OEM_COMMA: return ImGuiKey_Comma;
    case VK_OEM_MINUS: return ImGuiKey_Minus;
    case VK_OEM_PERIOD: return ImGuiKey_Period;
    case VK_OEM_2: return ImGuiKey_Slash;
    case VK_OEM_3: return ImGuiKey_GraveAccent;
    case VK_OEM_4: return ImGuiKey_LeftBracket;
    case VK_OEM_5: return ImGuiKey_Backslash;
    case VK_OEM_6: return ImGuiKey_RightBracket;
    case VK_OEM_7: return ImGuiKey_Apostrophe;
    default: return ImGuiKey_None;
    }
}
