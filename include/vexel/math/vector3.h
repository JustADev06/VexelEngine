#pragma once

#include <type_traits>
#include <cmath>
#include <ostream>

namespace vexel
{
    namespace impl
    {
    
    template<class T>
    concept IsFloat = std::is_floating_point_v<T>;

    // We use X+ right, Y+ up, +Z forward convention 
    template<IsFloat T>
    class Vector3
    {
    public:
        constexpr Vector3(T x, T y, T z)
            : x(x)
            , y(y)
            , z(z)
        {
        }

        constexpr Vector3(T xyz)
            : Vector3(xyz, xyz, xyz)
        {
        }

        constexpr Vector3()
            : Vector3(T(0))
        {
        }

        constexpr Vector3<T> &operator+=(const Vector3<T> &other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            
            return *this;
        }

        constexpr Vector3<T> operator+(const Vector3<T> &other) const
        {
            return Vector3<T>(*this) += other;
        }

        constexpr Vector3<T> &operator-=(const Vector3<T> &other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            
            return *this;
        }

        constexpr Vector3<T> operator-(const Vector3<T> &other) const
        {
            return Vector3<T>(*this) -= other;
        }

        constexpr Vector3<T> operator-() const
        {
            return Vector3<T>(-x, -y, -z);
        }

        constexpr Vector3<T> &operator*=(T scale)
        {
            x *= scale;
            y *= scale;
            z *= scale;

            return *this;
        }

        constexpr Vector3<T> operator*(T scale) const
        {
            return Vector3<T>(*this) *= scale;
        }

        constexpr T dot(const Vector3<T> &other) const
        {
            return x * other.x + y * other.y + z * other.z;
        }

        static constexpr T dot(const Vector3<T> &v1, const Vector3<T> &v2)
        {
            return v1.dot(v2);
        }

        constexpr Vector3<T> cross(const Vector3<T> &other) const
        {
            return Vector3<T>(y * other.z - z * other.y,
                              z * other.x - x * other.z,
                              x * other.y - y * other.x);
        }

        static constexpr Vector3<T> cross(const Vector3<T> &v1, const Vector3<T> &v2)
        {
            return v1.cross(v2);
        }

        constexpr T magnitude2() const
        {
            return Vector3<T>::dot(*this, *this);
        }

        static constexpr T magnitude2(const Vector3<T> &other)
        {
            return other.magnitude2();
        }

        constexpr T magnitude() const
        {
            return std::sqrt(magnitude2());
        }

        static constexpr T magnitude(const Vector3<T> &other)
        {
            return other.magnitude();
        }

        constexpr Vector3<T> &normalize(T epsilon = Vector3<T>::epsilon)
        {
            auto len = magnitude();
            if(std::abs(len) < epsilon)
            {
                return *this;
            }

            auto inv_len = T(1) / len;
            return (*this *= inv_len);
        }

        constexpr Vector3<T> normalize(T epsilon = Vector3<T>::epsilon) const
        {
            return Vector3<T>(*this).normalize(epsilon);
        }

        static constexpr Vector3<T> normalize(const Vector3<T> &other, T epsilon = Vector3<T>::epsilon)
        {
            return Vector3<T>(other).normalize(epsilon);
        }

        constexpr bool operator!=(const Vector3<T> &other) const
        {
            return (std::abs(x - other.x) > Vector3<T>::epsilon) || (std::abs(y - other.y) > Vector3<T>::epsilon) || (std::abs(z - other.z) > Vector3<T>::epsilon);
        }

        constexpr bool operator==(const Vector3<T> &other) const
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
            };
            struct
            {
                T xyz[3];
            };
        };

        friend std::ostream &operator<<(std::ostream &os, const Vector3<T> &v)
        {
            os << "X: " << v.x << '\n'
               << "Y: " << v.y << '\n'
               << "Z: " << v.z << '\n';

            return os;
        }

    private:
        static constexpr T epsilon = T(0.00000001); 
    };

    }
}