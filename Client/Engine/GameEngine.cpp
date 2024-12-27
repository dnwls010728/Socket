#include "pch.h"
#include "GameEngine.h"

#include "Level/World.h"
#include "Audio/AudioManager.h"
#include "Event/EventManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Input/Keyboard.h"
#include "Level/Level.h"
#include "Math/Math.h"
#include "UI/UICanvas.h"
#include "UI_OLD/Canvas.h"
#include "Windows/WindowsWindow.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/ShapeBatch.h"

GameEngine::GameEngine() :
    game_window_(nullptr),
    shape_batch_(nullptr),
    accumulator_(0.f)
{
}

GameEngine::~GameEngine()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void GameEngine::Init(const std::shared_ptr<WindowsWindow>& kWindow)
{
    CHECK(AudioManager::Get()->Init());
    
    game_window_ = kWindow;
    World::Get()->Init(game_window_);

    shape_batch_ = std::make_shared<ShapeBatch>();
    CHECK_IF(shape_batch_, L"Failed to create ShapeBatch.");
    shape_batch_->Init();
    
#pragma region ImGui 초기화
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    ImGuiIO& io = ImGui::GetIO(); static_cast<void>(io);
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.Fonts->AddFontFromFileTTF(".\\Content\\Fonts\\NanumBarunGothic.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesKorean());
    io.FontDefault = io.Fonts->Fonts[0];

    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(game_window_->GetHWnd());
    ImGui_ImplDX11_Init(Renderer::Get()->GetDevice(), Renderer::Get()->GetDeviceContext());
#pragma endregion
    
}

void GameEngine::GameLoop(float delta_time)
{
    StartFrame();
    
    World::Get()->TransitionLevel();
    
    Tick(delta_time);

    // 물리 시뮬레이션으로 인해 발생한 오차를 보정하기 위해 alpha를 계산
    float alpha = accumulator_ / ProjectSettings::kFixedTimeStep;
    Render(alpha);
    
    EndFrame();
}

void GameEngine::OnQuit()
{
    World::Get()->GetLevel()->Unload(EndPlayReason::kQuit);
}

void GameEngine::StartFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void GameEngine::Tick(float delta_time)
{
    // 죽음의 나선형을 방지하기 위해 delta_time을 제한
    const float kLimitFrameTime = Math::Min(delta_time, .25f);
    accumulator_ += kLimitFrameTime;
    
    AudioManager::Get()->Tick();

    while (accumulator_ >= ProjectSettings::kFixedTimeStep)
    {
        World::Get()->PhysicsTick(ProjectSettings::kFixedTimeStep);
        accumulator_ -= ProjectSettings::kFixedTimeStep;
    }

    World::Get()->Tick(delta_time);
    World::Get()->PostTick(delta_time);

    Canvas::Get()->Tick(delta_time);
}

void GameEngine::Render(float alpha)
{
    ImGui::Render();
    
    Renderer::Get()->BeginRender(game_window_);
    World::Get()->Render(alpha);
    
    Renderer::Get()->BeginRenderD2D(game_window_);
    Canvas::Get()->Render();
    UICanvas::Get()->Render();
    Renderer::Get()->EndRenderD2D();
    
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    
    Renderer::Get()->EndRender();
}

void GameEngine::EndFrame()
{
    World::Get()->SpawnActors();
    World::Get()->ProcessActorActivation();
    World::Get()->DestroyActors();
}
