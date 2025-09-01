#pragma once

class GridRendererComponent;

class Grid : public Actor
{
    SHADER_CLASS_HELPER(Grid)
    GENERATED_BODY(Grid, Actor)
    
public:
    Grid(const std::wstring& name);
    virtual ~Grid() override = default;

private:
    std::shared_ptr<GridRendererComponent> grid_renderer_;
    
};
