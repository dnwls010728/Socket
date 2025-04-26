#pragma once
#include "ActorLayer.h"
#include "Singleton.h"
#include "World/World.h"

class Level;

class EngineSettings : public Singleton<EngineSettings>
{
public:
    EngineSettings();
    virtual ~EngineSettings() override = default;

    void SetScreenSize(Type::uint32 width, Type::uint32 height);

    template<std::derived_from<Level> T>
    void AddLevel(const std::wstring& kName);

    FORCEINLINE void SetWindowTitle(const std::wstring& kWindowTitle) { window_title_ = kWindowTitle; }
    FORCEINLINE const std::wstring& GetWindowTitle() const { return window_title_; }

    FORCEINLINE void SetDefaultLevel(const std::wstring& kLevel) { default_level_ = kLevel; }
    FORCEINLINE const std::wstring& GetDefaultLevel() const { return default_level_; }

    FORCEINLINE Type::uint32 GetScreenWidth() const { return screen_width_; }
    FORCEINLINE Type::uint32 GetScreenHeight() const { return screen_height_; }

    FORCEINLINE void UseVSync(bool use_vsync) { use_vsync_ = use_vsync; }
    FORCEINLINE bool IsUseVSync() const { return use_vsync_; }

    FORCEINLINE void SetFixedTimeStep(float fixed_time_step) { fixed_time_step_ = fixed_time_step; }
    FORCEINLINE float GetFixedTimeStep() const { return fixed_time_step_; }

    FORCEINLINE void AddCollisionLayer(ActorLayer layer, ActorLayer collision) { layer_collision_matrix_[layer] = collision; }
    FORCEINLINE ActorLayer GetCollisionLayer(ActorLayer layer) const { return layer_collision_matrix_.at(layer); }

private:
    std::wstring window_title_;
    std::wstring default_level_;
    
    Type::uint32 screen_width_;
    Type::uint32 screen_height_;

    bool use_vsync_;

    float fixed_time_step_;
    
    std::unordered_map<ActorLayer, ActorLayer> layer_collision_matrix_;
    
};

template <std::derived_from<Level> T>
void EngineSettings::AddLevel(const std::wstring& kName)
{
    World::Get()->AddLevel<T>(kName);
}
