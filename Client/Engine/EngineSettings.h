#pragma once
#include "ActorLayer.h"
#include "Singleton.h"
#include "Level/World.h"

class Level;

class EngineSettings : public Singleton<EngineSettings>
{
public:
    EngineSettings();
    virtual ~EngineSettings() override = default;

    void SetScreenSize(uint32_t width, uint32_t height);

    template<std::derived_from<Level> T>
    void AddLevel(const std::wstring& kName);

    FORCEINLINE void SetWindowTitle(const std::wstring& kWindowTitle) { window_title_ = kWindowTitle; }
    FORCEINLINE const std::wstring& GetWindowTitle() const { return window_title_; }

    FORCEINLINE void SetDefaultLevel(const std::wstring& kLevel) { default_level_ = kLevel; }
    FORCEINLINE const std::wstring& GetDefaultLevel() const { return default_level_; }

    FORCEINLINE uint32_t GetScreenWidth() const { return screen_width_; }
    FORCEINLINE uint32_t GetScreenHeight() const { return screen_height_; }

    FORCEINLINE void UseVSync(bool use_vsync) { use_vsync_ = use_vsync; }
    FORCEINLINE bool IsUseVSync() const { return use_vsync_; }

    FORCEINLINE void SetFixedTimeStep(float fixed_time_step) { fixed_time_step_ = fixed_time_step; }
    FORCEINLINE float GetFixedTimeStep() const { return fixed_time_step_; }

    FORCEINLINE void AddCollisionLayer(ActorLayer layer, ActorLayer collision) { layer_collision_matrix_[layer] = collision; }
    FORCEINLINE ActorLayer GetCollisionLayer(ActorLayer layer) const { return layer_collision_matrix_.at(layer); }

    FORCEINLINE void SetInterpolationDelay(float delay) { interpolationDelay_ = delay; }
    FORCEINLINE float GetInterpolationDelay() const { return interpolationDelay_; }
private:
    std::wstring window_title_;
    std::wstring default_level_;
    
    uint32_t screen_width_;
    uint32_t screen_height_;

    bool use_vsync_;

    float fixed_time_step_;
    float interpolationDelay_;
    
    std::unordered_map<ActorLayer, ActorLayer> layer_collision_matrix_;
    
};

template <std::derived_from<Level> T>
void EngineSettings::AddLevel(const std::wstring& kName)
{
    World::Get()->AddLevel<T>(kName);
}
