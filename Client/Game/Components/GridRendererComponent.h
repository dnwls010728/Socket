#pragma once
#include "Math/Color.h"

class GridRendererComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(GridRendererComponent)
    GENERATED_BODY(GridRendererComponent, ActorComponent)
    
public:
    GridRendererComponent(Actor* owner, const std::wstring& name);
    virtual ~GridRendererComponent() override = default;

    void SetRows(uint32_t rows);
    void SetCols(uint32_t cols);
    void SetColor(const Math::Color& color);
    
    FORCEINLINE uint32_t GetRows() const { return rows_; }
    FORCEINLINE uint32_t GetCols() const { return cols_; }

    FORCEINLINE const Math::Color& GetColor() const { return color_; }

protected:
    virtual void InitializeComponent() override;
    virtual void UninitializeComponent() override;
    virtual void OnEnable() override;
    virtual void OnDisable() override;
    virtual void TickComponent(float delta_time) override;
    virtual void Render(float alpha) override;

private:
    void RebuildGridGeometry();
    
    std::vector<DefaultVertex> vertices_;
    std::vector<uint32_t> indices_;

    std::shared_ptr<Shape> shape_;
    
    Math::Color color_;

    uint32_t rows_;
    uint32_t cols_;

    bool is_dirty_;
    
};
