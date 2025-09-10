#pragma once

class GridRendererComponent;

class Grid : public Actor
{
    SHADER_CLASS_HELPER(Grid)
    GENERATED_BODY(Grid, Actor)
    
public:
    Grid(const std::wstring& name);
    virtual ~Grid() override = default;

    void SetRows(uint32_t rows) const;
    void SetCols(uint32_t columns) const;

private:
    std::shared_ptr<GridRendererComponent> grid_renderer_;
    
};
