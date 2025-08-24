#pragma once

class PostProcessingSettings : public Singleton<PostProcessingSettings>
{
public:
    PostProcessingSettings();
    virtual ~PostProcessingSettings() override = default;

    FORCEINLINE float GetBlurRadius() const { return blur_radius_; }
    FORCEINLINE void SetBlurRadius(float radius) { blur_radius_ = radius; }

    FORCEINLINE float GetVignetteStrength() const { return vignette_strength_; }
    FORCEINLINE void SetVignetteStrength(float strength) { vignette_strength_ = strength; }

    FORCEINLINE float GetGamma() const { return gamma_; }
    FORCEINLINE void SetGamma(float gamma) { gamma_ = gamma; }

    FORCEINLINE float GetGrayscale() const { return grayscale_; }
    FORCEINLINE void SetGrayscale(float grayscale) { grayscale_ = grayscale; }

private:
    float blur_radius_;
    float vignette_strength_;
    float gamma_;
    float grayscale_;
    
};
