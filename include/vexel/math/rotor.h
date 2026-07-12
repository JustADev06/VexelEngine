#pragma once

#include "vexel/math/vector3.h"

#include <ostream>

namespace vexel
{
    enum class EulerAnglesOrder : std::uint8_t
    {
        // Important! Read from right to left, like as composition order
        XYZ,
        ZXY,
        YZX,
        YXZ,
        ZYX,
        XZY
    };

    namespace impl
    {

    template<IsFloat T>
    class Rotor
    {
    public:
        constexpr Rotor()
            : a(T(1))
            , xy(T(0))
            , yz(T(0))
            , zx(T(0))
        {
        }

        constexpr Rotor(T a, T yz, T zx, T xy)
            : a(a)
            , xy(xy)
            , yz(yz)
            , zx(zx)
        {
            //normalize();
        }

        constexpr Rotor(const Vector3<T> &v1, const Vector3<T> &v2)
            : a(Vector3<T>::dot(v1, v2))
            , xy(v1.x * v2.y - v1.y * v2.x)
            , yz(v1.y * v2.z - v1.z * v2.y)
            , zx(v1.z * v2.x - v1.x * v2.z)
        {
            normalize();
        }

        constexpr Rotor(const Vector3<T> &normal, T angle)
            : a(std::cos(angle / T(2)))
            , xy(std::sin(angle / T(2)) * normal.z)
            , yz(std::sin(angle / T(2)) * normal.x)
            , zx(std::sin(angle / T(2)) * normal.y)
        {
            normalize();
        }

        constexpr Rotor(T yaw /** Y axis */, T pitch /** X axis */, T roll /** Z axis */, EulerAnglesOrder order)
            : Rotor()
        {
            Rotor<T> x_axis(Vector3<T>(T(1), T(0), T(0)), pitch);
            Rotor<T> y_axis(Vector3<T>(T(0), T(1), T(0)), yaw);
            Rotor<T> z_axis(Vector3<T>(T(0), T(0), T(1)), roll);

            switch(order)
            {
            case EulerAnglesOrder::XYZ: *this *= z_axis; *this *= y_axis; *this *= x_axis; break;
            case EulerAnglesOrder::ZXY: *this *= y_axis; *this *= x_axis; *this *= z_axis; break;
            case EulerAnglesOrder::YZX: *this *= x_axis; *this *= z_axis; *this *= y_axis; break;
            case EulerAnglesOrder::YXZ: *this *= z_axis; *this *= x_axis; *this *= y_axis; break;
            case EulerAnglesOrder::ZYX: *this *= x_axis; *this *= y_axis; *this *= z_axis; break;
            case EulerAnglesOrder::XZY: *this *= y_axis; *this *= z_axis; *this *= x_axis; break;
            }
        }

        // *this = *this * other
        constexpr Rotor<T> &operator*=(const Rotor<T> &other)
        {
            Rotor<T> temp;
            temp.a = a * other.a - xy * other.xy - yz * other.yz - zx * other.zx;
            temp.yz = yz * other.a + a * other.yz + xy * other.zx - zx * other.xy;
            temp.zx = a * other.zx + other.a * zx + yz * other.xy - xy * other.yz;
            temp.xy = xy * other.a + zx * other.yz - yz * other.zx + a * other.xy;

            return (*this = std::move(temp));
        }

        constexpr Rotor<T> operator*(const Rotor<T> &other) const
        {
            return Rotor<T>(*this) *= other;
        }

        constexpr Rotor<T> &operator*=(T scale)
        {
            a *= scale;
            xy *= scale;
            yz *= scale;
            zx *= scale;

            return *this;
        }

        constexpr Rotor<T> operator*(T scale) const
        {
            return Rotor<T>(*this) *= scale;
        }

        constexpr T magnitude2() const
        {
            // The reverse and R itself multiplied
            return a * a + xy * xy + yz * yz + zx * zx;
        }

        static constexpr T magnitude2(const Rotor<T> &r)
        {
            return r.magnitude2();
        }

        constexpr T magnitude() const
        {
            return std::sqrt(magnitude2());
        }

        static constexpr T magnitude(const Rotor<T> &r)
        {
            return r.magnitude();
        }

        constexpr Rotor<T> &normalize(T epsilon = Rotor<T>::epsilon)
        {
            T len = magnitude();
            if(std::abs(len) < epsilon)
            {
                return *this;
            }

            return (*this *= (T(1) / len));            
        }

        constexpr Rotor<T> normalize(T epsilon = Rotor<T>::epsilon) const
        {
            return Rotor<T>(*this).normalize();
        }

        static constexpr Rotor<T> normalize(const Rotor<T> &r, T epsilon = Rotor<T>::epsilon)
        {
            return r.normalize();
        }
   
        // The inverse if the rotor is normalized, because (a + b e_1 e_2 + c e_2 e_3 + d e_3 e_1)(a - b e_1 e_2 - c e_2 e_3 - d e_3 e_1) = a^2 + b^2 + c^2 + d^2 = 1
        constexpr Rotor<T> reverse() const
        {
            return Rotor<T>(a, -yz, -zx, -xy);
        }

        static constexpr Rotor<T> reverse(const Rotor<T> &r)
        {
            return r.reverse();
        }

        constexpr Vector3<T> rotate_x() const
        {
            return Vector3<T>(T(1) - T(2) * (xy * xy + zx * zx), T(2) * (zx * yz - a * xy), T(2) * (a * zx + yz * xy));
        }

        constexpr Vector3<T> rotate_y() const
        {
            return Vector3<T>(T(2) * (xy * a + zx * yz), T(1) - T(2) * (xy * xy + yz * yz), T(2) * (xy * zx - a * yz));
        }

        constexpr Vector3<T> rotate_z() const
        {
            return Vector3<T>(T(2) * (xy * yz - a * zx), T(2) * (a * yz + zx * xy), T(1) - T(2) * (yz * yz + zx * zx));
        }

        static constexpr Vector3<T> rotate_x(const Rotor<T> &r)
        {
            return r.rotate_x();
        }

        static constexpr Vector3<T> rotate_y(const Rotor<T> &r)
        {
            return r.rotate_y();
        }

        static constexpr Vector3<T> rotate_z(const Rotor<T> &r)
        {
            return r.rotate_z();
        }

        constexpr Vector3<T> rotate(const Vector3<T> &v)
        {
            return rotate_x() * v.x  + rotate_y() * v.y + rotate_z() * v.z;
        }

        static constexpr Vector3<T> rotate(const Rotor<T> &r, const Vector3<T> &v)
        {
            return r.rotate(v);
        }

        // The default form of a rotation rotor, is R = e^(-theta/2) * B, where B^2 = -1, defines the plane of rotation and theta is the angle of rotation,
        // and for a general rotor, we have e^R = e^(a + be_2 e_3 + c e_3 e_1 + d e_1 e_2) = e^a * e^(|B| * B), where B^2 = -1 which can be now deduced
        constexpr Rotor<T> &exp(T epsilon = Rotor<T>::epsilon)
        {
            T scale = std::exp(a);
            T minus_half_angle = std::sqrt(xy * xy + yz * yz + zx * zx);
            if(std::abs(minus_half_angle) > epsilon)
            {
                T inv_minus_half_angle = T(1) / minus_half_angle;

                xy *= inv_minus_half_angle;
                yz *= inv_minus_half_angle;
                zx *= inv_minus_half_angle;       
            }

            a = std::cos(minus_half_angle);
            xy *= std::sin(minus_half_angle);
            yz *= std::sin(minus_half_angle);
            zx *= std::sin(minus_half_angle);

            return *this *= scale;
        }

        constexpr Rotor<T> &log(T epsilon = Rotor<T>::epsilon)
        {
            T norm = std::sqrt(a * a + xy * xy + yz * yz + zx * zx); // e^a is always positive
            if(std::abs(norm) > epsilon)
            {
                T inv_norm = T(1) / norm;

                // Now having removed the e^a scaling part, we're only left with e^|B|B = cos(|B|) + B sin(|B|), so the scalar part is the cosine of the |B|, or the -theta/2
                *this *= inv_norm; 
            }

            T angle = std::acos(a); // Or you can say |B| (up to sign) 
            impl::Vector3<T> bivector(yz, zx, xy);
            bivector.normalize(); // We don't care about the signs, the bivector already encodes the orientation of the rotation
            bivector *= angle;
            
            a = std::log(norm); 
            yz = bivector.x;
            zx = bivector.y;
            xy = bivector.z;

            return *this;
        }

        static constexpr Rotor<T> exp(const Rotor<T> &r)
        {
            return Rotor<T>(r).exp();
        }

        static constexpr Rotor<T> log(const Rotor<T> &r)
        {
            return Rotor<T>(r).log();
        }

        constexpr Rotor<T> slerp(const Rotor<T> &b, T t) const
        {
            return Rotor<T>::exp(Rotor<T>::log(b * reverse()) * t);
        }

        static constexpr Rotor<T> slerp(const Rotor<T> &a, const Rotor<T> &b, T t)
        {
            return a.slerp(b, t);
        }

        friend std::ostream &operator<<(std::ostream &os, const Rotor<T> &r)
        {
            os << "A: " << r.a << '\n'
               << "YZ: " << r.yz << '\n'
               << "ZX: " << r.zx << '\n'
               << "XY: " << r.xy << '\n';

            return os;
        }
    private:
        T a;
        T xy;
        T yz;
        T zx;

        static constexpr T epsilon = T(0.00000001);
    };

    }
}