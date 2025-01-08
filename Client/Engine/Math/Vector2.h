#pragma once

namespace Math
{
    struct Vector2
    {
        Vector2();
        Vector2(float new_x, float new_y);

        Vector2& operator+=(const Vector2& kVector);
        Vector2& operator-=(const Vector2& kVector);
        Vector2& operator*=(const Vector2& kVector);
        Vector2& operator/=(const Vector2& kVector);
        Vector2& operator=(const Vector2& kVector);

        Vector2 operator+(const Vector2& kVector);
        Vector2 operator-(const Vector2& kVector);
        Vector2 operator*(const Vector2& kVector);
        Vector2 operator/(const Vector2& kVector);
     
        Vector2 operator+(const Vector2& kVector) const;
        Vector2 operator-(const Vector2& kVector) const;
        Vector2 operator*(const Vector2& kVector) const;
        Vector2 operator/(const Vector2& kVector) const;

        Vector2 operator+(float val);
        Vector2 operator-(float val);
        Vector2 operator*(float val);
        Vector2 operator/(float val);

        Vector2 operator+(float val) const;
        Vector2 operator-(float val) const;
        Vector2 operator*(float val) const;
        Vector2 operator/(float val) const;

        bool operator==(const Vector2& kVector) const;
        bool operator!=(const Vector2& kVector) const;

        ~Vector2() = default;

        /**
         * \brief Vector(0.f, 0.f)
         * \return Vector
         */
        static Vector2 Zero();

        /**
         * \brief Vector(1.f, 1.f)
         * \return Vector
         */
        static Vector2 One();

        /**
         * \brief Vector(-1.f, 0.f)
         * \return Vector
         */
        static Vector2 Left();

        /**
         * \brief Vector(0.f, 1.f)
         * \return Vector
         */
        static Vector2 Up();

        /**
         * \brief Vector(1.f, 0.f)
         * \return Vector
         */
        static Vector2 Right();

        /**
         * \brief Vector(0.f, -1.f)
         * \return Vector
         */
        static Vector2 Down();

        /**
         * \brief 두 백터를 선형 보간합니다.
         * \param a 첫번째 백터
         * \param b 두번째 백터
         * \param t 비율
         * \return Vector
         */
        static Vector2 Lerp(Vector2 a, Vector2 b, float t);

        /**
         * \brief 두 백터의 거리를 반환합니다.
         * \param a 첫번째 백터
         * \param b 두번째 백터
         * \return float
         */
        static float Distance(Vector2 a, Vector2 b);

        /**
         * \brief 두 백터의 내적을 반환합니다.
         * \param a 첫번째 백터
         * \param b 두번째 백터
         * \return float
         */
        static float Dot(Vector2 a, Vector2 b);

        /**
         * \brief 두 백터의 외적을 반환합니다.
         * \param a 첫번째 백터
         * \param b 두번째 백터
         * \return float
         */
        static float Cross(Vector2 a, Vector2 b);

        /**
         * \brief 두 백터의 가장 큰 값으로 이루어진 백터를 반환합니다.
         * \param a 첫번째 백터
         * \param b 두번째 백터
         * \return float
         */
        static Vector2 Max(Vector2 a, Vector2 b);

        /**
         * \brief 두 백터의 가장 작은 값으로 이루어진 백터를 반환합니다.
         * \param a 첫번째 백터
         * \param b 두번째 백터
         * \return float
         */
        static Vector2 Min(Vector2 a, Vector2 b);

        /**
         * 반사 벡터를 반환합니다.
         * @param direction 방향
         * @param normal 법선
         * @return Vector
         */
        static Vector2 Reflect(Vector2 direction, Vector2 normal);

        /**
         * 백터의 최대값을 반환합니다.
         * @return Vector
         */
        static Vector2 PositiveInfinity();

        /**
         * 백터의 최소값을 반환합니다.
         * @return Vector
         */
        static Vector2 NegativeInfinity();

        /**
         * 두 백터의 각도를 반환합니다.
         * @param from 시작 백터
         * @param to 끝 백터
         * @return float
         */
        static float Angle(Vector2 from, Vector2 to);

        /**
         * \brief 백터를 정규화된 값으로 반환합니다.
         * \return Vector
         */
        Vector2 Normalized() const;

        /**
         * \brief 백터의 길이를 반환합니다.
         * \return float
         */
        float Magnitude() const;

        /**
         * \brief 백터를 정규화를 합니다.
         */
        void Normalize();

        float x;
        float y;
    };

    Vector2 operator+(float val, const Vector2& kVector);
    Vector2 operator-(float val, const Vector2& kVector);
    Vector2 operator*(float val, const Vector2& kVector);
    Vector2 operator/(float val, const Vector2& kVector);
}
