#include "pch.h"
#include "GridRendererComponent.h"

#include "Windows/DX/Shape.h"

GridRendererComponent::GridRendererComponent(Actor* owner, const std::wstring& name) :
    ActorComponent(owner, name),
    vertices_(),
    indices_(),
    shape_(nullptr),
    color_(Math::Color::White),
    rows_(0),
    cols_(0)
{
}

void GridRendererComponent::InitializeComponent()
{
    ActorComponent::InitializeComponent();

    shape_ = std::make_shared<Shape>();
    shape_->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    shape_->SetColor(color_);
    shape_->SetZOrder(std::numeric_limits<int32_t>::max());
    
    Refresh();
}

void GridRendererComponent::UninitializeComponent()
{
    ActorComponent::UninitializeComponent();
    
    World::Get()->RemoveShape(shape_);
}

void GridRendererComponent::OnEnable()
{
    ActorComponent::OnEnable();

    World::Get()->AddShape(shape_);
}

void GridRendererComponent::OnDisable()
{
    ActorComponent::OnDisable();

    World::Get()->RemoveShape(shape_);
}

void GridRendererComponent::Refresh()
{
    vertices_.clear();
    indices_.clear();

    for (uint32_t row = 0; row <= rows_; ++row)
    {
        float py = static_cast<float>(row);
        
        DefaultVertex v0 = {
            {0.f, py, 0.f}, {1.f, 1.f, 1.f, 1.f}
        };

        DefaultVertex v1 = {
            {static_cast<float>(cols_), py, 0.f}, {1.f, 1.f, 1.f, 1.f}
        };

        uint32_t base = static_cast<uint32_t>(vertices_.size());

        vertices_.push_back(v0);
        vertices_.push_back(v1);
        
        indices_.push_back(base);
        indices_.push_back(base + 1);
    }

    for (uint32_t col = 0; col <= cols_; ++col)
    {
        float px = static_cast<float>(col);

        DefaultVertex v0 = {
            {px, 0.f, 0.f}, {1.f, 1.f, 1.f, 1.f}
        };

        DefaultVertex v1 = {
            {px, static_cast<float>(rows_), 0.f}, {1.f, 1.f, 1.f, 1.f}
        };

        uint32_t base = static_cast<uint32_t>(vertices_.size());

        vertices_.push_back(v0);
        vertices_.push_back(v1);
        
        indices_.push_back(base);
        indices_.push_back(base + 1);
    }

    if (shape_)
    {
        shape_->SetVertices(vertices_);
        shape_->SetIndices(indices_);
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<GridRendererComponent>("GridRendererComponent")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
