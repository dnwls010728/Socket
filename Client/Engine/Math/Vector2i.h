#pragma once

namespace Math
{
    struct Vector2i
    {
        Vector2i();
        Vector2i(int32_t new_x, int32_t new_y);
        
        Vector2i& operator+=(const Vector2i& vector);
        Vector2i& operator-=(const Vector2i& vector);
        Vector2i& operator*=(const Vector2i& vector);
        Vector2i& operator=(const Vector2i& vector);

        Vector2i operator+(const Vector2i& vector);
        Vector2i operator-(const Vector2i& vector);
        Vector2i operator*(const Vector2i& vector);
     
        Vector2i operator+(const Vector2i& vector) const;
        Vector2i operator-(const Vector2i& vector) const;
        Vector2i operator*(const Vector2i& vector) const;
        
        Vector2i operator+(int32_t val);
        Vector2i operator-(int32_t val);
        Vector2i operator*(int32_t val);
        Vector2i operator/(int32_t val);

        Vector2i operator+(int32_t val) const;
        Vector2i operator-(int32_t val) const;
        Vector2i operator*(int32_t val) const;
        Vector2i operator/(int32_t val) const;
        
        bool operator==(const Vector2i& vector) const;
        bool operator!=(const Vector2i& vector) const;

        ~Vector2i() = default;
        
        /**
         * \brief Vector(0.f, 0.f)
         * \return Vector
         */
        static Vector2i Zero();

        /**
         * \brief Vector(1.f, 1.f)
         * \return Vector
         */
        static Vector2i One();

        /**
         * \brief Vector(-1.f, 0.f)
         * \return Vector
         */
        static Vector2i Left();

        /**
         * \brief Vector(0.f, 1.f)
         * \return Vector
         */
        static Vector2i Up();

        /**
         * \brief Vector(1.f, 0.f)
         * \return Vector
         */
        static Vector2i Right();

        /**
         * \brief Vector(0.f, -1.f)
         * \return Vector
         */
        static Vector2i Down();

        int32_t x;
        int32_t y;
    };
    
    Vector2i operator+(int32_t val, const Vector2i& vector);
    Vector2i operator-(int32_t val, const Vector2i& vector);
    Vector2i operator*(int32_t val, const Vector2i& vector);
    Vector2i operator/(int32_t val, const Vector2i& vector);
}
