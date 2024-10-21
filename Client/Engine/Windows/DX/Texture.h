#pragma once
#include "Renderer.h"

#include <d3d11.h>
#include <string>
#include <vector>
#include <wrl/client.h>

#include "Vertex.h"
#include "Misc/Type.h"
#include "Resource/Resource.h"

enum class WrapMode
{
    kReapet,
    kClamp
};

enum class FilterMode
{
    kPoint,
    kBilinear
};

class Texture : public Resource
{
    GENERATED_BODY(Texture, Resource)
    
public:
    Texture();
    virtual ~Texture() override = default;
    
    virtual bool Load(const std::wstring& kPath) override;

    void Bind() const;

    inline Type::uint32 GetWidth() const { return width_; }
    inline Type::uint32 GetHeight() const { return height_; }

    inline const std::vector<DefaultVertex>& GetVertices() const { return vertices_; }
    inline const std::vector<Type::uint32>& GetIndices() const { return indices_; }

    inline WrapMode GetWrapMode() const { return wrap_mode_; }
    inline void SetWrapMode(WrapMode mode) { wrap_mode_ = mode; }

    inline FilterMode GetFilterMode() const { return filter_mode_; }
    inline void SetFilterMode(FilterMode mode) { filter_mode_ = mode; }

protected:
    friend class Editor;
    
    Microsoft::WRL::ComPtr<ID3D11Resource> resource_;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resource_view_;
    
    Type::uint32 width_;
    Type::uint32 height_;

    std::vector<DefaultVertex> vertices_;
    std::vector<Type::uint32> indices_;

    WrapMode wrap_mode_;
    FilterMode filter_mode_;
    
};
