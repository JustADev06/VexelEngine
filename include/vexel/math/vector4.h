#pragma once

#include "vexel/math/vector3.h"

namespace vexel
{
    namespace impl
    {

    template<IsFloat T>
    class Vector4
    {
    public:
        constexpr Vector4(T x, T y, T z, T w)
            : x(x)
            , y(y)
            , z(z)
            , w(w)
        {
        }

        constexpr Vector4(const Vector3<T> &v, T w = T(1))
            : Vector4(v.x, v.y, v.z, w)
        {

        }

        constexpr Vector4(T xyzw)
            : Vector4(xyzw, xyzw, xyzw, xyzw)
        {
        }

        constexpr Vector4()
            : Vector4(T(0))
        {
        }

        constexpr Vector4<T> &operator+=(const Vector4<T> &other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            
            return *this;
        }

        constexpr Vector4<T> operator+(const Vector4<T> &other) const
        {
            return Vector4<T>(*this) += other;
        }

        constexpr Vector4<T> &operator-=(const Vector4<T> &other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            
            return *this;
        }

        constexpr Vector4<T> operator-(const Vector4<T> &other) const
        {
            return Vector4<T>(*this) -= other;
        }

        constexpr Vector4<T> operator-() const
        {
            return Vector4<T>(-x, -y, -z, -w);
        }

        constexpr Vector4<T> &operator*=(T scale)
        {
            x *= scale;
            y *= scale;
            z *= scale;
            w *= scale;

            return *this;
        }

        constexpr Vector4<T> operator*(T scale) const
        {
            return Vector4<T>(*this) *= scale;
        }

        constexpr T dot(const Vector4<T> &other) const
        {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        static constexpr T dot(const Vector4<T> &v1, const Vector4<T> &v2)
        {
            return v1.dot(v2);
        }

        constexpr T magnitude2() const
        {
            return Vector4<T>::dot(*this, *this);
        }

        static constexpr T magnitude2(const Vector4<T> &other)
        {
            return other.magnitude2();
        }

        constexpr T magnitude() const
        {
            return std::sqrt(magnitude2());
        }

        static constexpr T magnitude(const Vector4<T> &other)
        {
            return other.magnitude();
        }

        constexpr Vector4<T> &normalize(T epsilon = Vector4<T>::epsilon)
        {
            auto len = magnitude();
            if(std::abs(len) < epsilon)
            {
                return *this;
            }

            auto inv_len = T(1) / len;
            return (*this *= inv_len);
        }


        constexpr Vector4<T> normalize(T epsilon = Vector4<T>::epsilon) const
        {
            return Vector4<T>(*this).normalize(epsilon);
        }

        static constexpr Vector4<T> normalize(const Vector4<T> &other, T epsilon = Vector4<T>::epsilon)
        {
            return Vector4<T>(other).normalize(epsilon);
        }

        constexpr bool operator!=(const Vector4<T> &other) const
        {
            return (std::abs(x - other.x) > Vector4<T>::epsilon) || (std::abs(y - other.y) > Vector4<T>::epsilon) || (std::abs(z - other.z) > Vector4<T>::epsilon) || (std::abs(w - other.w) > Vector4<T>::epsilon);
        }

        constexpr bool operator==(const Vector4<T> &other) const
        {
            return !(*this != other);
        }

        union
        {
            struct
            {
                T x;
                T y;
                T z;
                T w;
            };
            struct
            {
                T xyzw[4];
            };
        };
        
        friend std::ostream &operator<<(std::ostream &os, const Vector4<T> &v)
        {
            os << "X: " << v.x << '\n'
               << "Y: " << v.y << '\n'
               << "Z: " << v.z << '\n'
               << "W: " << v.w << '\n';

            return os;
        }

    private:
        static constexpr T epsilon = T(0.00000001); 
    };

    }
}