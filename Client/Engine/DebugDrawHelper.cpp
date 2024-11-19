#include "pch.h"
#include "DebugDrawHelper.h"

#include "Level/World.h"
#include "Math/Math.h"
#include "Windows/DX/Shape.h"

DebugDrawHelper::DebugDrawHelper() :
    polygon_vertices_(),
    polygon_indices_(),
    circle_vertices_(),
    circle_indices_(),
    capsule_vertices_(),
    capsule_indices_(),
    segment_vertices_()
{
    polygon_shape_ = std::make_shared<Shape>();
    circle_shape_ = std::make_shared<Shape>();
    capsule_shape_ = std::make_shared<Shape>();

    segment_shape_ = std::make_shared<Shape>();
    segment_shape_->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}
void DebugDrawHelper::DrawBox(const Math::Vector2& kCenter, const Math::Vector2& kSize, const Math::Color& kColor)
{
    const Math::Vector2 kHalfSize = kSize * .5f;

    const Math::Vector2 vertices[4] = {
        kCenter - kHalfSize,
        kCenter + Math::Vector2(kHalfSize.x, -kHalfSize.y),
        kCenter + kHalfSize,
        kCenter + Math::Vector2(-kHalfSize.x, kHalfSize.y)
    };

    Math::Vector2 p1 = vertices[3];
    for (int i = 0; i < 4; ++i)
    {
        Math::Vector2 p2 = vertices[i];
        DrawSegment(p1, p2, kColor);
        p1 = p2;
    }
}

void DebugDrawHelper::DrawCircle(const Math::Vector2& kCenter, float radius, const Math::Color& kColor)
{
    const int kSegment = 16;
    const float kIncrement = 2.f * MATH_PI / kSegment;
    float sin_increment = sin(kIncrement);
    float cos_increment = cos(kIncrement);

    Math::Vector2 r1 = {radius, 0.f};
    Math::Vector2 v1 = kCenter + r1;

    for (int i = 0; i < kSegment; ++i)
    {
        Math::Vector2 r2;
        r2.x = r1.x * cos_increment - r1.y * sin_increment;
        r2.y = r1.x * sin_increment + r1.y * cos_increment;

        Math::Vector2 v2 = kCenter + r2;
        DrawSegment(v1, v2, kColor);

        r1 = r2;
        v1 = v2;
    }

    Math::Vector2 start = kCenter - Math::Vector2(radius, 0.f);
    Math::Vector2 end = kCenter + Math::Vector2(radius, 0.f);
    DrawSegment(start, end, kColor);

    start = kCenter + Math::Vector2(0.f, radius);
    end = kCenter - Math::Vector2(0.f, radius);
    DrawSegment(start, end, kColor);
}

void DebugDrawHelper::DrawSegment(const Math::Vector2& kStart, const Math::Vector2& kEnd, const Math::Color& kColor)
{
    segment_vertices_.push_back({
        {kStart.x, kStart.y, 0.f},
        {kColor.r / 255.f, kColor.g / 255.f, kColor.b / 255.f, kColor.a / 255.f}
    });

    segment_vertices_.push_back({
        {kEnd.x, kEnd.y, 0.f},
        {kColor.r / 255.f, kColor.g / 255.f, kColor.b / 255.f, kColor.a / 255.f}
    });
}

void DebugDrawHelper::DrawRay(const Math::Vector2& kStart, const Math::Vector2& kDirection, const Math::Color& kColor)
{
    DrawSegment(kStart, kStart + kDirection, kColor);
}

void DebugDrawHelper::Clear()
{
    polygon_shape_->SetVertices(polygon_vertices_);
    polygon_shape_->SetIndices(polygon_indices_);

    circle_shape_->SetVertices(circle_vertices_);
    circle_shape_->SetIndices(circle_indices_);

    capsule_shape_->SetVertices(capsule_vertices_);
    capsule_shape_->SetIndices(capsule_indices_);

    segment_shape_->SetVertices(segment_vertices_);

    // World::Get()->AddShape(polygon_shape_);
    // World::Get()->AddShape(circle_shape_);
    // World::Get()->AddShape(capsule_shape_);
    // World::Get()->AddShape(segment_shape_);

    polygon_vertices_.clear();
    polygon_indices_.clear();

    circle_vertices_.clear();
    circle_indices_.clear();

    capsule_vertices_.clear();
    capsule_indices_.clear();

    segment_vertices_.clear();
}
