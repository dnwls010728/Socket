#include "pch.h"
#include "PhysicsDebugDrawHelper.h"

#include "imgui/imgui.h"
#include "Level/World.h"
#include "Windows/DX/Shape.h"

FORCEINLINE static Math::Color MakeRGBA8(b2HexColor color, float alpha)
{
    return {
        static_cast<Type::uint8>((color >> 16) & 0xFF),
        static_cast<Type::uint8>((color >> 8) & 0xFF),
        static_cast<Type::uint8>(color & 0xFF),
        static_cast<Type::uint8>(0xFF * alpha)
    };
}

void PhysicsDebugDrawHelper::AddPolygon(const b2Vec2* vertices, int vertexCount, b2HexColor color)
{
    b2Vec2 p1 = vertices[vertexCount - 1];
    for (int i = 0; i < vertexCount; ++i)
    {
        b2Vec2 p2 = vertices[i];
        AddSegment(p1, p2, color);
        p1 = p2;
    }
}

void PhysicsDebugDrawHelper::AddSolidPolygon(b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color)
{
    Math::Color fill_color = MakeRGBA8(color, .6f);
    Math::Color border_color = MakeRGBA8(color, 1.f);

    // Fill
    for (int i = 0; i < vertexCount; ++i)
    {
        b2Vec2 p = b2Add(transform.p, b2RotateVector(transform.q, vertices[i]));
        polygon_vertices_.push_back({
            {p.x, p.y, 0.f},
            {fill_color.r / 255.f, fill_color.g / 255.f, fill_color.b / 255.f, fill_color.a / 255.f}
        });
    }

    int last_idx = polygon_vertices_.size() - vertexCount;

    for (int i = 0; i < vertexCount - 2; ++i)
    {
        polygon_indices_.push_back(last_idx);
        polygon_indices_.push_back(last_idx + i + 1);
        polygon_indices_.push_back(last_idx + i + 2);
    }

    // Border
    b2Vec2 p1 = b2Add(transform.p, b2RotateVector(transform.q, vertices[vertexCount - 1]));
    for (int i = 0; i < vertexCount; ++i)
    {
        b2Vec2 p2 = b2Add(transform.p, b2RotateVector(transform.q, vertices[i]));
        segment_vertices_.push_back({
            {p1.x, p1.y, 0.f},
            {border_color.r / 255.f, border_color.g / 255.f, border_color.b / 255.f, border_color.a / 255.f}
        });

        segment_vertices_.push_back({
            {p2.x, p2.y, 0.f},
            {border_color.r / 255.f, border_color.g / 255.f, border_color.b / 255.f, border_color.a / 255.f}
        });
        
        p1 = p2;
    }
}

void PhysicsDebugDrawHelper::AddCircle(b2Vec2 center, float radius, b2HexColor color)
{
    const int kSegments = 16;
    const float kIncrement = 2.f * b2_pi / kSegments;
    float sin_increment = sinf(kIncrement);
    float cos_increment = cosf(kIncrement);

    b2Vec2 r1 = {radius, 0.f};
    b2Vec2 v1 = b2Add(center, r1);

    for (int i = 0; i < kSegments; ++i)
    {
        b2Vec2 r2;
        r2.x = cos_increment * r1.x - sin_increment * r1.y;
        r2.y = sin_increment * r1.x + cos_increment * r1.y;

        b2Vec2 v2 = b2Add(center, r2);
        AddSegment(v1, v2, color);
        
        r1 = r2;
        v1 = v2;
    }
}

void PhysicsDebugDrawHelper::AddSolidCircle(b2Transform transform, b2Vec2 center, float radius, b2HexColor color)
{
    Math::Color rgba8 = MakeRGBA8(color, .6f);

    b2Vec2 final_center = b2Add(transform.p, b2RotateVector(transform.q, center));

    const int kSegments = 16;
    const float kIncrement = 2.f * b2_pi / kSegments;
    float sin_increment = sinf(kIncrement);
    float cos_increment = cosf(kIncrement);

    b2Vec2 r = {radius, 0.f};
    b2Vec2 v = b2Add(final_center, r);

    for (int i = 0; i < kSegments; ++i)
    {
        circle_vertices_.push_back({
            {v.x, v.y, 0.f},
            {rgba8.r / 255.f, rgba8.g / 255.f, rgba8.b / 255.f, rgba8.a / 255.f}
        });
        
        b2Vec2 t_r;
        t_r.x = cos_increment * r.x - sin_increment * r.y;
        t_r.y = sin_increment * r.x + cos_increment * r.y;

        b2Vec2 t_v = b2Add(final_center, t_r);
        r = t_r;
        v = t_v;
    }
    
    circle_vertices_.push_back({
        {v.x, v.y, 0.f},
        {rgba8.r / 255.f, rgba8.g / 255.f, rgba8.b / 255.f, rgba8.a / 255.f}
    });

    int last_idx = circle_vertices_.size() - (kSegments + 1);

    for (int i = 0; i < kSegments; ++i)
    {
        circle_indices_.push_back(last_idx);
        circle_indices_.push_back(last_idx + i + 1);
        circle_indices_.push_back(last_idx + i + 2);
    }

    AddCircle(final_center, radius, color);

    b2Vec2 axis = b2Rot_GetXAxis(transform.q);

    AddSegment(final_center, b2MulAdd(final_center, radius, axis), color);
}

void PhysicsDebugDrawHelper::AddCapsule(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color)
{
    float length;
    b2Vec2 axis = b2GetLengthAndNormalize(&length, b2Sub(p2, p1));

    const int kSegments = 16.f;
    const float kIncremnt = b2_pi / kSegments;
    float sin_incremnt = sinf(kIncremnt);
    float cos_incremnt = cosf(kIncremnt);

    b2Vec2 r1 = {-axis.y, axis.x};
    b2Vec2 v1 = b2MulAdd(p1, radius, r1);
    b2Vec2 a = v1;

    for (int i = 0; i < kSegments; ++i)
    {
        b2Vec2 r2;
        r2.x = cos_incremnt * r1.x - sin_incremnt * r1.y;
        r2.y = sin_incremnt * r1.x + cos_incremnt * r1.y;

        b2Vec2 v2 = b2MulAdd(p1, radius, r2);
        AddSegment(v1, v2, color);
        r1 = r2;
        v1 = v2;
    }

    b2Vec2 b = v1;

    r1 = {axis.y, -axis.x};
    v1 = b2MulAdd(p2, radius, r1);
    b2Vec2 c = v1;

    for (int i = 0; i < kSegments; ++i)
    {
        b2Vec2 r2;
        r2.x = cos_incremnt * r1.x - sin_incremnt * r1.y;
        r2.y = sin_incremnt * r1.x + cos_incremnt * r1.y;

        b2Vec2 v2 = b2MulAdd(p2, radius, r2);
        AddSegment(v1, v2, color);
        r1 = r2;
        v1 = v2;
    }

    b2Vec2 d = v1;

    AddSegment(a, d, color);
    AddSegment(b, c, color);
    AddSegment(p1, p2, color);
}

void PhysicsDebugDrawHelper::AddSolidCapsule(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color)
{
    Math::Color fill_color = MakeRGBA8(color, .6f);
    
    float length;
    b2Vec2 axis = b2GetLengthAndNormalize(&length, b2Sub(p2, p1));

    const int kSegments = 16.f;
    const float kIncremnt = b2_pi / kSegments;
    float sin_incremnt = sinf(kIncremnt);
    float cos_incremnt = cosf(kIncremnt);

    b2Vec2 r = {-axis.y, axis.x};
    b2Vec2 v = b2MulAdd(p1, radius, r);

    for (int i = 0; i < kSegments; ++i)
    {
        capsule_vertices_.push_back({
            {v.x, v.y, 0.f},
            {fill_color.r / 255.f, fill_color.g / 255.f, fill_color.b / 255.f, fill_color.a / 255.f}
        });

        b2Vec2 t_r;
        t_r.x = cos_incremnt * r.x - sin_incremnt * r.y;
        t_r.y = sin_incremnt * r.x + cos_incremnt * r.y;
        
        b2Vec2 t_v = b2MulAdd(p1, radius, r);
        r = t_r;
        v = t_v;
    }
    
    capsule_vertices_.push_back({
        {v.x, v.y, 0.f},
        {fill_color.r / 255.f, fill_color.g / 255.f, fill_color.b / 255.f, fill_color.a / 255.f}
    });

    r = {axis.y, -axis.x};
    v = b2MulAdd(p2, radius, r);

    for (int i = 0; i < kSegments; ++i)
    {
        capsule_vertices_.push_back({
            {v.x, v.y, 0.f},
            {fill_color.r / 255.f, fill_color.g / 255.f, fill_color.b / 255.f, fill_color.a / 255.f}
        });

        b2Vec2 t_r;
        t_r.x = cos_incremnt * r.x - sin_incremnt * r.y;
        t_r.y = sin_incremnt * r.x + cos_incremnt * r.y;
        
        b2Vec2 t_v = b2MulAdd(p2, radius, r);
        r = t_r;
        v = t_v;
    }
    
    capsule_vertices_.push_back({
        {v.x, v.y, 0.f},
        {fill_color.r / 255.f, fill_color.g / 255.f, fill_color.b / 255.f, fill_color.a / 255.f}
    });

    int last_idx = capsule_vertices_.size() - ((kSegments * 2.f) + 2.f);

    for (int i = 0; i < kSegments * 2.f; ++i)
    {
        capsule_indices_.push_back(last_idx);
        capsule_indices_.push_back(last_idx + i + 1);
        capsule_indices_.push_back(last_idx + i + 2);
    }

    AddCapsule(p1, p2, radius, color);
}

void PhysicsDebugDrawHelper::AddSegment(b2Vec2 p1, b2Vec2 p2, b2HexColor color)
{
    Math::Color rgba8 = MakeRGBA8(color, 1.f);

    segment_vertices_.push_back({
        {p1.x, p1.y, 0.f},
        {rgba8.r / 255.f, rgba8.g / 255.f, rgba8.b / 255.f, rgba8.a / 255.f}
    });

    segment_vertices_.push_back({
        {p2.x, p2.y, 0.f},
        {rgba8.r / 255.f, rgba8.g / 255.f, rgba8.b / 255.f, rgba8.a / 255.f}
    });
}

void PhysicsDebugDrawHelper::AddTransform(b2Transform transform)
{
    const float kAxisScale = .2f;
    b2Vec2 p1 = transform.p;

    b2Vec2 p2 = b2MulAdd(p1, kAxisScale, b2Rot_GetXAxis(transform.q));
    AddSegment(p1, p2, b2_colorRed);

    p2 = b2MulAdd(p1, kAxisScale, b2Rot_GetYAxis(transform.q));
    AddSegment(p1, p2, b2_colorGreen);
}

void PhysicsDebugDrawHelper::AddPoint(b2Vec2 p, float size, b2HexColor color)
{
}

void PhysicsDebugDrawHelper::AddString(b2Vec2 p, const char* string, ...)
{
}

void PhysicsDebugDrawHelper::Clear()
{
    std::shared_ptr<Shape> polygon = std::make_shared<Shape>();
    polygon->SetVertices(polygon_vertices_);
    polygon->SetIndices(polygon_indices_);

    std::shared_ptr<Shape> circle = std::make_shared<Shape>();
    circle->SetVertices(circle_vertices_);
    circle->SetIndices(circle_indices_);

    std::shared_ptr<Shape> capsule = std::make_shared<Shape>();
    capsule->SetVertices(capsule_vertices_);
    capsule->SetIndices(capsule_indices_);

    std::shared_ptr<Shape> segment = std::make_shared<Shape>();
    segment->SetVertices(segment_vertices_);
    segment->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    
    // World::Get()->AddShape(polygon);
    // World::Get()->AddShape(circle);
    // World::Get()->AddShape(capsule);
    // World::Get()->AddShape(segment);
    
    polygon_vertices_.clear();
    polygon_indices_.clear();

    circle_vertices_.clear();
    circle_indices_.clear();

    capsule_vertices_.clear();
    capsule_indices_.clear();

    segment_vertices_.clear();
}
