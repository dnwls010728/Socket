#pragma once
#include <d3d11.h>
#include <vector>

#include "Vertex.h"
#include "Math/Bounds.h"
#include "Math/Color.h"
#include "Math/Vector2.h"

class PixelShader;
class VertexShader;
class Texture;

class Shape
{
public:
    Shape();
    virtual ~Shape() = default;

    void SetPosition(const Math::Vector2& position);
    void SetScale(const Math::Vector2& scale);
    void SetPivot(const Math::Vector2& pivot);
    void SetAngle(float angle);

    Bounds GetBounds() const;

    FORCEINLINE void SetVertices(const std::vector<DefaultVertex>& kVertices) { vertices_ = kVertices; }
    FORCEINLINE void SetIndices(const std::vector<uint32_t>& kIndices) { indices_ = kIndices; }
    FORCEINLINE void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY primitive_topology) { primitive_topology_ = primitive_topology; }

    FORCEINLINE const std::vector<DefaultVertex>& GetVertices() const { return vertices_; }
    FORCEINLINE const std::vector<uint32_t>& GetIndices() const { return indices_; }

    FORCEINLINE const DirectX::XMMATRIX& GetWorldMatrix() const { return world_matrix_; }

    FORCEINLINE D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const { return primitive_topology_; }

    FORCEINLINE const Math::Vector2& GetPosition() const { return position_; }
    FORCEINLINE const Math::Vector2& GetScale() const { return scale_; }
    FORCEINLINE const Math::Vector2& GetPivot() const { return pivot_; }

    FORCEINLINE void SetUVOffset(const Math::Vector2& uv_offset) { uv_offset_ = uv_offset; }
    FORCEINLINE const Math::Vector2& GetUVOffset() const { return uv_offset_; }

    FORCEINLINE void SetUVScale(const Math::Vector2& uv_scale) { uv_scale_ = uv_scale; }
    FORCEINLINE const Math::Vector2& GetUVScale() const { return uv_scale_; }

    FORCEINLINE void SetColor(const Math::Color& kColor) { color_ = kColor; }
    FORCEINLINE const Math::Color& GetColor() const { return color_; }
    
    FORCEINLINE float GetRotation() const { return angle_; }

    FORCEINLINE void SetTexture(Texture* kTexture) { texture_ = kTexture; }
    FORCEINLINE const Texture* GetTexture() const { return texture_; }

    FORCEINLINE void SetZOrder(int32_t z_order) { z_order_ = z_order; }
    FORCEINLINE int32_t GetZOrder() const { return z_order_; }

    static bool CompareZOrder(const std::shared_ptr<Shape>& kLHS, const std::shared_ptr<Shape>& kRHS);

protected:
    virtual void UpdateMatrixx();
    
    std::vector<DefaultVertex> vertices_;
    std::vector<uint32_t> indices_;

    DirectX::XMMATRIX world_matrix_;

    D3D11_PRIMITIVE_TOPOLOGY primitive_topology_;

    Math::Vector2 position_;
    Math::Vector2 scale_;
    Math::Vector2 pivot_;
    Math::Vector2 uv_offset_;
    Math::Vector2 uv_scale_;

    Math::Color color_;
    
    float angle_;

    Texture* texture_;

    int32_t z_order_;
    
};
