#pragma once
#include "Renderer.h"

#include <d3d11.h>
#include <string>
#include <vector>
#include <wrl/client.h>

#include "Vertex.h"
#include "Asset/Asset.h"

class Texture : public Asset
{
    GENERATED_BODY(Texture, Asset)
    
public:
    enum class WrapMode : uint8_t
    {
        kReapet,
        kClamp
    };

    enum class FilterMode : uint8_t
    {
        kPoint,
        kBilinear
    };
    
    Texture();
    virtual ~Texture() override = default;
    
    virtual bool Load(const std::wstring& kPath) override;

    void Bind() const;

    FORCEINLINE uint32_t GetWidth() const { return width_; }
    FORCEINLINE uint32_t GetHeight() const { return height_; }

    FORCEINLINE const std::vector<DefaultVertex>& GetVertices() const { return vertices_; }
    FORCEINLINE const std::vector<uint32_t>& GetIndices() const { return indices_; }

    FORCEINLINE WrapMode GetWrapMode() const { return wrap_mode_; }
    FORCEINLINE void SetWrapMode(WrapMode mode) { wrap_mode_ = mode; }

    FORCEINLINE FilterMode GetFilterMode() const { return filter_mode_; }
    FORCEINLINE void SetFilterMode(FilterMode mode) { filter_mode_ = mode; }

protected:
    friend class Editor;
    
    Microsoft::WRL::ComPtr<ID3D11Resource> resource_;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resource_view_;
    
    uint32_t width_;
    uint32_t height_;

    std::vector<DefaultVertex> vertices_;
    std::vector<uint32_t> indices_;

    WrapMode wrap_mode_;
    FilterMode filter_mode_;
    
};
