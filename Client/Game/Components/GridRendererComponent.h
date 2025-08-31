#pragma once
#include "Math/Color.h"

class GridRendererComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(GridRendererComponent)
    GENERATED_BODY(GridRendererComponent, ActorComponent)
    
public:
    GridRendererComponent(Actor* owner, const std::wstring& name);
    virtual ~GridRendererComponent() override = default;
    
    FORCEINLINE uint32_t GetRows() const { return rows_; }
    FORCEINLINE uint32_t GetCols() const { return cols_; }

    FORCEINLINE void SetColor(const Math::Color& color) { color_ = color; }
    FORCEINLINE const Math::Color& GetColor() const { return color_; }

protected:
    virtual void InitializeComponent() override;
    virtual void UninitializeComponent() override;
    virtual void OnEnable() override;
    virtual void OnDisable() override;

private:
    void Refresh();
    
    std::vector<DefaultVertex> vertices_;
    std::vector<uint32_t> indices_;

    std::shared_ptr<Shape> shape_;
    
    Math::Color color_;

    uint32_t rows_;
    uint32_t cols_;
    
};
