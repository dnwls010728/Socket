#pragma once
#include <vector>

#include "Math/Vector2.h"

class Actor;

struct HitResult
{
    Actor* actor = nullptr;
 
    Math::Vector2 start;
    Math::Vector2 end;
    Math::Vector2 point;
    Math::Vector2 normal;

    b2ShapeId shape_id;
 
    float distance;
};

class Physics2D
{
public:
    /**
     * 상자에 겹치는 액터를 찾습니다.
     * @param kCenter 상자의 중심
     * @param kSize 상자의 크기
     * @param output_actor 겹치는 액터
     * @param layer 충돌 레이어
     * @return 겹치는 액터가 있으면 true, 없으면 false
     */
    static bool OverlapBox(const Math::Vector2& kCenter, const Math::Vector2& kSize, Actor** output_actor, uint16_t layer = 0xFFFF);

    /**
     * 상자에 겹치는 모든 액터를 찾습니다.
     * @param kCenter 상자의 중심
     * @param kSize 상자의 크기
     * @param output_actors 겹치는 액터들
     * @param layer 충돌 레이어
     * @return 겹쳐진 액터들이 있으면 true, 없으면 false
     */
    static bool OverlapBoxAll(const Math::Vector2& kCenter, const Math::Vector2& kSize, std::vector<Actor*>& output_actors, uint16_t layer = 0xFFFF);

    /**
     * 원에 겹치는 액터를 찾습니다.
     * @param kCenter 원의 중심
     * @param radius 원의 반지름
     * @param output_actor 겹치는 액터
     * @param layer 충돌 레이어
     * @return 겹치는 액터가 있으면 true, 없으면 false
     */
    static bool OverlapCircle(const Math::Vector2& kCenter, float radius, Actor** output_actor, uint16_t layer = 0xFFFF);

    /**
     * 원에 겹치는 모든 액터를 찾습니다.
     * @param kCenter 원의 중심
     * @param radius 원의 반지름
     * @param output_actors 겹치는 액터들
     * @param layer 충돌 레이어
     * @return 겹쳐진 액터들이 있으면 true, 없으면 false
     */
    static bool OverlapCircleAll(const Math::Vector2& kCenter, float radius, std::vector<Actor*>& output_actors, uint16_t layer = 0xFFFF);

    /**
     * 광선을 쏴서 충돌하는 액터를 찾습니다.
     * @param hit_result 충돌 결과
     * @param kOrigin 광선의 시작 위치
     * @param kDirection 광선의 방향
     * @param max_distance 광선의 최대 거리
     * @param layer 충돌 레이어
     * @return 충돌이 있으면 true, 없으면 false
     */
    static bool RayCast(HitResult& hit_result, const Math::Vector2& kOrigin, const Math::Vector2& kDirection, float max_distance, uint16_t layer = 0xFFFF);

    /**
     * 광선을 쏴서 충돌하는 모든 액터를 찾습니다.
     * @param hit_results 충돌 결과들
     * @param kOrigin 광선의 시작 위치
     * @param kDirection 광선의 방향
     * @param max_distance 광선의 최대 거리
     * @param layer 충돌 레이어
     * @return 충돌이 있으면 true, 없으면 false
     */
    static bool RayCastAll(std::vector<HitResult>& hit_results, const Math::Vector2& kOrigin, const Math::Vector2& kDirection, float max_distance, uint16_t layer = 0xFFFF);

    /**
     * 상자를 쏴서 충돌하는 액터를 찾습니다.
     * @param hit_result 충돌 결과
     * @param kSize 상자의 크기
     * @param angle 상자의 회전 각도
     * @param kOrigin 상자의 시작 위치
     * @param kDirection 상자의 방향
     * @param max_distance 상자의 최대 거리
     * @param layer 충돌 레이어
     * @return 충돌이 있으면 true, 없으면 false
     */
    static bool BoxCast(HitResult& hit_result, const Math::Vector2& kSize, float angle, const Math::Vector2& kOrigin, const Math::Vector2& kDirection, float max_distance, uint16_t layer = 0xFFFF);

    /**
     * 상자를 쏴서 충돌하는 모든 액터를 찾습니다.
     * @param hit_results 충돌 결과들
     * @param kSize 상자의 크기
     * @param angle 상자의 회전 각도
     * @param kOrigin 상자의 시작 위치
     * @param kDirection 상자의 방향
     * @param max_distance 상자의 최대 거리
     * @param layer 충돌 레이어
     * @return 충돌이 있으면 true, 없으면 false
     */
    static bool BoxCastAll(std::vector<HitResult>& hit_results, const Math::Vector2& kSize, float angle, const Math::Vector2& kOrigin, const Math::Vector2& kDirection, float max_distance, uint16_t layer = 0xFFFF);

    /**
     * 원을 쏴서 충돌하는 액터를 찾습니다.
     * @param hit_result 충돌 결과
     * @param radius 원의 반지름
     * @param angle 원의 회전 각도
     * @param kOrigin 원의 시작 위치
     * @param kDirection 원의 방향
     * @param max_distance 원의 최대 거리
     * @param layer 충돌 레이어
     * @return 충돌이 있으면 true, 없으면 false
     */
    static bool CircleCast(HitResult& hit_result, float radius, float angle, const Math::Vector2& kOrigin, const Math::Vector2& kDirection, float max_distance, uint16_t layer = 0xFFFF);

    /**
     * 원을 쏴서 충돌하는 모든 액터를 찾습니다.
     * @param hit_results 충돌 결과들
     * @param radius 원의 반지름
     * @param angle 원의 회전 각도
     * @param kOrigin 원의 시작 위치
     * @param kDirection 원의 방향
     * @param max_distance 원의 최대 거리
     * @param layer 충돌 레이어
     * @return 충돌이 있으면 true, 없으면 false
     */
    static bool CircleCastAll(std::vector<HitResult>& hit_results, float radius, float angle, const Math::Vector2& kOrigin, const Math::Vector2& kDirection, float max_distance, uint16_t layer = 0xFFFF);
};
