#pragma once

#include "vexel/math/rotor.h"
#include "vexel/math/vector4.h"

#include <array>
#include <ostream>

namespace vexel
{
    // 0, 1, 2, sgn = 1
    // 2, 0, 1, sgn = 1
    // 1, 2, 0, sgn = 1
    // 1, 0, 2, sgn = -1
    // 2, 1, 0, sgn = -1
    // 0, 2, 1, sgn = -1

    // (row, column) = column * 3 + row
    // m_00 m_11 m_22 = m0 * m4 * m8
    // m_02 m_10 m_21 = m6 * m1 * m5
    // m_01 m_12 m_20 = m3 * m7 * m2
    // -m_01 m_10 m_22 = -m3 * m1 * m8
    // -m_02 m_11 m_20 = -m6 * m4 * m2
    // -m_00 m_12 m_21 = -m0 * m7 * m5

                 /* Col 0 */  /* Col 1 */  /* Col 2 */
    #define DET33(m0, m1, m2, m3, m4, m5, m6, m7, m8) (m0 * m4 * m8 + m6 * m1 * m5 + m3 * m7 * m2 - m3 * m1 * m8 - m6 * m4 * m2 - m0 * m7 * m5)
    #define DOT_ROW_COLUMN44(row, column, elements1, elements2) (elements2[4 * column + 0] * elements1[0 + row] + elements2[4 * column + 1] * elements1[4 + row] + elements2[4 * column + 2] * elements1[8 + row] + elements2[4 * column + 3] * elements1[12 + row])

    namespace impl
    {
        
    template<IsFloat T>
    class Matrix4
    {
    public:
        constexpr Matrix4()
            : Matrix4(T(1))
        {
        }

        constexpr Matrix4(T d)
            : Matrix4(d, d, d, d)
        {
        }

        constexpr Matrix4(T d1, T d2, T d3, T d4)
            : elements_({d1, T(0), T(0), T(0), T(0), d2, T(0), T(0), T(0), T(0), d3, T(0), T(0), T(0), T(0), d4})
        {
        }

        constexpr Matrix4(const Vector3<T> &col0, const Vector3<T> &col1, const Vector3<T> &col2, const Vector3<T> &col3) /** Plus translation */
            : elements_({col0.x, col0.y, col0.z, T(0), col1.x, col1.y, col1.z, T(0), col2.x, col2.y, col2.z, T(0), col3.x, col3.y, col3.z, T(1)})
        {
        }

        constexpr Matrix4(const Vector3<T> &col0, const Vector3<T> &col1, const Vector3<T> &col2) /** Only 3D transform */
            : elements_({col0.x, col0.y, col0.z, T(0), col1.x, col1.y, col1.z, T(0), col2.x, col2.y, col2.z, T(0), T(0), T(0), T(0), T(1)})
        {
        }
        
        constexpr Matrix4(T m00, T m01, T m02, T m03, /** First index - row, Second index - column */
                          T m10, T m11, T m12, T m13,
                          T m20, T m21, T m22, T m23,
                          T m30, T m31, T m32, T m33)
            : elements_({m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32, m03, m13, m23, m33})
        {
        }

        constexpr T &operator[](std::size_t index)
        {
            return elements_[index];
        }

        constexpr T operator[](std::size_t index) const
        {
            return elements_[index];
        }

        constexpr Matrix4<T> &operator*=(T scale)
        {
            for(auto &element : elements_)
            {
                element *= scale;
            }

            return *this;
        }

        constexpr Matrix4<T> operator*(T scale)
        {
            return Matrix4<T>(*this) *= scale;
        }

        constexpr Matrix4<T> &operator*=(const Matrix4<T> &m)
        {
            Matrix4<T> other(T(0));

            other[0] = DOT_ROW_COLUMN44(0, 0, elements_, m);
            other[1] = DOT_ROW_COLUMN44(1, 0, elements_, m);
            other[2] = DOT_ROW_COLUMN44(2, 0, elements_, m);
            other[3] = DOT_ROW_COLUMN44(3, 0, elements_, m);

            other[4] = DOT_ROW_COLUMN44(0, 1, elements_, m);
            other[5] = DOT_ROW_COLUMN44(1, 1, elements_, m);
            other[6] = DOT_ROW_COLUMN44(2, 1, elements_, m);
            other[7] = DOT_ROW_COLUMN44(3, 1, elements_, m);

            other[8] = DOT_ROW_COLUMN44(0, 2, elements_, m);
            other[9] = DOT_ROW_COLUMN44(1, 2, elements_, m);
            other[10] = DOT_ROW_COLUMN44(2, 2, elements_, m);
            other[11] = DOT_ROW_COLUMN44(3, 2, elements_, m);

            other[12] = DOT_ROW_COLUMN44(0, 3, elements_, m);
            other[13] = DOT_ROW_COLUMN44(1, 3, elements_, m);
            other[14] = DOT_ROW_COLUMN44(2, 3, elements_, m);
            other[15] = DOT_ROW_COLUMN44(3, 3, elements_, m);

            return (*this = std::move(other));
        }

        constexpr Matrix4<T> operator*(const Matrix4<T> &m) const
        {
            return Matrix4<T>(*this) *= m;
        }

        constexpr Vector3<T> operator*(const Vector3<T> &v) const
        {
            return Vector3<T>(elements_[0] * v.x + elements_[4] * v.y + elements_[8] * v.z + elements_[12],
                              elements_[1] * v.x + elements_[5] * v.y + elements_[9] * v.z + elements_[13],
                              elements_[2] * v.x + elements_[6] * v.y + elements_[10] * v.z + elements_[14]);
        }

        constexpr Vector4<T> operator*(const Vector4<T> &v) const
        {
            return Vector4<T>(elements_[0] * v.x + elements_[4] * v.y + elements_[8] * v.z + elements_[12] * v.w,
                              elements_[1] * v.x + elements_[5] * v.y + elements_[9] * v.z + elements_[13] * v.w,
                              elements_[2] * v.x + elements_[6] * v.y + elements_[10] * v.z + elements_[14] * v.w,
                              elements_[3] * v.x + elements_[7] * v.y + elements_[11] * v.z + elements_[15] * v.w);
        }

        constexpr Matrix4<T> &transpose()
        {
            /**
             *  m0, m4, m8,  m12
             *  m1, m5, m9,  m13
             *  m2, m6, m10, m14
             *  m3, m7, m11, m15
             */
            std::swap(elements_[1], elements_[4]);
            std::swap(elements_[2], elements_[8]);
            std::swap(elements_[3], elements_[12]);
            std::swap(elements_[6], elements_[9]);
            std::swap(elements_[7], elements_[13]);
            std::swap(elements_[11], elements_[14]);

            return *this;
        }

        constexpr Matrix4<T> transpose() const
        {
            return Matrix4<T>(*this).transpose();
        }

        static constexpr Matrix4<T> transpose(const Matrix4<T> &m)
        {
            return m.transpose();
        }

        static constexpr Matrix4<T> translate(const Vector3<T> &t)
        {
            return Matrix4<T>(T(1), T(0), T(0), t.x,
                              T(0), T(1), T(0), t.y,
                              T(0), T(0), T(1), t.z,
                              T(0), T(0), T(0), T(1));
        }

        static constexpr Matrix4<T> scale(const Vector3<T> &scale)
        {
            return Matrix4<T>(T(scale.x), T(0),       T(0),       T(0),
                              T(0),       T(scale.y), T(0),       T(0),
                              T(0),       T(0),       T(scale.z), T(0),
                              T(0),       T(0),       T(0),       T(1));
        }

        static constexpr Matrix4<T> rotate(const Rotor<T> &r)
        {
            return Matrix4<T>(r.rotate_x(), r.rotate_y(), r.rotate_z());
        }

        static constexpr Rotor<T> to_rotor(const Matrix4<T> &m)
        {
            T a = std::sqrt(m[0] + m[5] + m[10] + T(1)) / T(2); // The two solutions represent the two rotors R and -R giving same rotation, but we don't care and pick a > 0 by convention
            T xy = (m[4] - m[1]) / (a * T(4)); 
            T zx = (m[2] - m[8]) / (a * T(4));
            T yz = (m[9] - m[6]) / (a * T(4));

            return Rotor<T>(a, yz, zx, xy);
        }

                                   /* Translation, Scale, Rotation, they should be applied in the following order, which is the typical convention, translation * rotation * scale, read from right to left  */
        static constexpr std::tuple<Vector3<T>, Vector3<T>, Rotor<T>> decompose(const Matrix4<T> &m)
        {
            Vector3<T> translation;
            translation.x = m[12];
            translation.y = m[13];
            translation.z = m[14];

            Vector3<T> col0 = Vector3<T>(m[0], m[1], m[2]);
            Vector3<T> col1 = Vector3<T>(m[4], m[5], m[6]);
            Vector3<T> col2 = Vector3<T>(m[8], m[9], m[10]);

            Vector3<T> scale;
            scale.x = col0.magnitude(); // How big is the respective basis vector after applying the matrix
            scale.y = col1.magnitude();
            scale.z = col2.magnitude();

            col0 *= (T(1) / scale.x);
            col1 *= (T(1) / scale.y);
            col2 *= (T(1) / scale.z);

            T a = std::sqrt(col0.x + col1.y + col2.z + T(1)) / T(2);           
            T xy = (col1.x - col0.y) / (a * T(4)); 
            T zx = (col0.z - col2.x) / (a * T(4));
            T yz = (col2.y - col1.z) / (a * T(4));

            return std::make_tuple(translation, scale, Rotor<T>(a, yz, zx, xy));
        }

        constexpr T determinant() const
        {
            /**
             *  m0, m4, m8,  m12
             *  m1, m5, m9,  m13
             *  m2, m6, m10, m14
             *  m3, m7, m11, m15
             */

            return elements_[0] * DET33(elements_[5], elements_[6], elements_[7], elements_[9], elements_[10], elements_[11], elements_[13], elements_[14], elements_[15]) +
                   (T(-1)) * elements_[4] * DET33(elements_[1], elements_[2], elements_[3], elements_[9], elements_[10], elements_[11], elements_[13], elements_[14], elements_[15]) +
                   elements_[8] * DET33(elements_[1], elements_[2], elements_[3], elements_[5], elements_[6], elements_[7], elements_[13], elements_[14], elements_[15]) +
                   (T(-1)) * elements_[12] * DET33(elements_[1], elements_[2], elements_[3], elements_[5], elements_[6], elements_[7], elements_[9], elements_[10], elements_[11]);
        }

        static constexpr T determinant(const Matrix4<T> &m)
        {
            return m.determinant();
        }

        constexpr T trace() const
        {
            return elements_[0] + elements_[5] + elements_[10] + elements_[15];
        }

        static constexpr T trace(const Matrix4<T> &m)
        {
            return m.trace();
        }

        constexpr Matrix4<T> adjoint() const
        {
            /**
             *  m0, m4, m8,  m12
             *  m1, m5, m9,  m13
             *  m2, m6, m10, m14
             *  m3, m7, m11, m15
             */
            T a0 = DET33(elements_[5], elements_[6], elements_[7], elements_[9], elements_[10], elements_[11], elements_[13], elements_[14], elements_[15]);
            T a1 = -DET33(elements_[4], elements_[6], elements_[7], elements_[8], elements_[10], elements_[11], elements_[12], elements_[14], elements_[15]);
            T a2 = DET33(elements_[4], elements_[5], elements_[7], elements_[8], elements_[9], elements_[11], elements_[12], elements_[13], elements_[15]);
            T a3 = -DET33(elements_[4], elements_[5], elements_[6], elements_[8], elements_[9], elements_[10], elements_[12], elements_[13], elements_[14]);

            T a4 = -DET33(elements_[1], elements_[2], elements_[3], elements_[9], elements_[10], elements_[11], elements_[13], elements_[14], elements_[15]);
            T a5 = DET33(elements_[0], elements_[2], elements_[3], elements_[8], elements_[10], elements_[11], elements_[12], elements_[14], elements_[15]);
            T a6 = -DET33(elements_[0], elements_[1], elements_[3], elements_[8], elements_[9], elements_[11], elements_[12], elements_[13], elements_[15]);
            T a7 = DET33(elements_[0], elements_[1], elements_[2], elements_[8], elements_[9], elements_[10], elements_[12], elements_[13], elements_[14]);

            T a8 = DET33(elements_[1], elements_[2], elements_[3], elements_[5], elements_[6], elements_[7], elements_[13], elements_[14], elements_[15]);
            T a9 = -DET33(elements_[0], elements_[2], elements_[3], elements_[4], elements_[6], elements_[7], elements_[12], elements_[14], elements_[15]);
            T a10 = DET33(elements_[0], elements_[1], elements_[3], elements_[4], elements_[5], elements_[7], elements_[12], elements_[13], elements_[15]);
            T a11 = -DET33(elements_[0], elements_[1], elements_[2], elements_[4], elements_[5], elements_[6], elements_[12], elements_[13], elements_[14]);

            T a12 = -DET33(elements_[1], elements_[2], elements_[3], elements_[5], elements_[6], elements_[7], elements_[9], elements_[10], elements_[11]);
            T a13 = DET33(elements_[0], elements_[2], elements_[3], elements_[4], elements_[6], elements_[7], elements_[8], elements_[10], elements_[11]);
            T a14 = -DET33(elements_[0], elements_[1], elements_[3], elements_[4], elements_[5], elements_[7], elements_[8], elements_[9], elements_[11]);
            T a15 = DET33(elements_[0], elements_[1], elements_[2], elements_[4], elements_[5], elements_[6], elements_[8], elements_[9], elements_[10]);

            return Matrix4<T>(a0, a1, a2, a3,
                              a4, a5, a6, a7,
                              a8, a9, a10, a11,
                              a12, a13, a14, a15);
        }

        constexpr Matrix4<T> inverse(T epsilon = Matrix4<T>::epsilon) const
        {
            /**
             *  m0, m4, m8,  m12
             *  m1, m5, m9,  m13
             *  m2, m6, m10, m14
             *  m3, m7, m11, m15
             */
            T a0 = DET33(elements_[5], elements_[6], elements_[7], elements_[9], elements_[10], elements_[11], elements_[13], elements_[14], elements_[15]);
            T a1 = -DET33(elements_[4], elements_[6], elements_[7], elements_[8], elements_[10], elements_[11], elements_[12], elements_[14], elements_[15]);
            T a2 = DET33(elements_[4], elements_[5], elements_[7], elements_[8], elements_[9], elements_[11], elements_[12], elements_[13], elements_[15]);
            T a3 = -DET33(elements_[4], elements_[5], elements_[6], elements_[8], elements_[9], elements_[10], elements_[12], elements_[13], elements_[14]);

            T a4 = -DET33(elements_[1], elements_[2], elements_[3], elements_[9], elements_[10], elements_[11], elements_[13], elements_[14], elements_[15]);
            T a5 = DET33(elements_[0], elements_[2], elements_[3], elements_[8], elements_[10], elements_[11], elements_[12], elements_[14], elements_[15]);
            T a6 = -DET33(elements_[0], elements_[1], elements_[3], elements_[8], elements_[9], elements_[11], elements_[12], elements_[13], elements_[15]);
            T a7 = DET33(elements_[0], elements_[1], elements_[2], elements_[8], elements_[9], elements_[10], elements_[12], elements_[13], elements_[14]);

            T a8 = DET33(elements_[1], elements_[2], elements_[3], elements_[5], elements_[6], elements_[7], elements_[13], elements_[14], elements_[15]);
            T a9 = -DET33(elements_[0], elements_[2], elements_[3], elements_[4], elements_[6], elements_[7], elements_[12], elements_[14], elements_[15]);
            T a10 = DET33(elements_[0], elements_[1], elements_[3], elements_[4], elements_[5], elements_[7], elements_[12], elements_[13], elements_[15]);
            T a11 = -DET33(elements_[0], elements_[1], elements_[2], elements_[4], elements_[5], elements_[6], elements_[12], elements_[13], elements_[14]);

            T a12 = -DET33(elements_[1], elements_[2], elements_[3], elements_[5], elements_[6], elements_[7], elements_[9], elements_[10], elements_[11]);
            T a13 = DET33(elements_[0], elements_[2], elements_[3], elements_[4], elements_[6], elements_[7], elements_[8], elements_[10], elements_[11]);
            T a14 = -DET33(elements_[0], elements_[1], elements_[3], elements_[4], elements_[5], elements_[7], elements_[8], elements_[9], elements_[11]);
            T a15 = DET33(elements_[0], elements_[1], elements_[2], elements_[4], elements_[5], elements_[6], elements_[8], elements_[9], elements_[10]);

            T det = elements_[0] * a0 + elements_[4] * a4 + elements_[8] * a8 + elements_[12] * a12;

            if(std::abs(det) < epsilon)
            {
                return Matrix4<T>(T(0));
            }

            return (Matrix4<T>(a0, a1, a2, a3,
                               a4, a5, a6, a7,
                               a8, a9, a10, a11,
                               a12, a13, a14, a15) * (T(1) / det));
        }

        static constexpr Matrix4<T> inverse(const Matrix4<T> &m, T epsilon = Matrix4<T>::epsilon)
        {
            return m.inverse();
        }

        static constexpr Matrix4<T> adjoint(const Matrix4<T> &m)
        {
            return m.adjoint();
        }

        constexpr const T *data() const
        {
            return elements_.data();
        }

        static constexpr const T *data(const Matrix4<T> &m)
        {
            return m.data();
        }

        friend std::ostream &operator<<(std::ostream &os, const Matrix4<T> &m)
        {
            os << m[0] << ", " << m[4] << ", " << m[8] << ", " << m[12] << '\n'
               << m[1] << ", " << m[5] << ", " << m[9] << ", " << m[13] << '\n'
               << m[2] << ", " << m[6] << ", " << m[10] << ", " << m[14] << '\n'
               << m[3] << ", " << m[7] << ", " << m[11] << ", " << m[15] << '\n';

            return os;
        }
        
    private:
        std::array<T, 16u> elements_; // Column major order

        static constexpr T epsilon = T(0.00000001);
    };

    }

    #undef DET33

}