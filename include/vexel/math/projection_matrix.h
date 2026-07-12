#pragma once

#include "vexel/math/utils.h"

namespace vexel
{


    template<impl::IsFloat T>
    constexpr impl::Matrix4<T> frustum(T left, T right, T down, T up, T near, T far /** The first five, define the view plane, while far specifies how far the frustum extends */)
    {
        // Basically just interval linear interpolation, with the assumption of the Z divide
        auto [A_x, B_x] = impl::Interval<T>::interval_lerp_coefficients(impl::Interval<T>(left, right), minus_one_to_one<T>);
        auto [A_y, B_y] = impl::Interval<T>::interval_lerp_coefficients(impl::Interval<T>(down, up), minus_one_to_one<T>);
        auto [A_z, B_z] = impl::Interval<T>::interval_lerp_coefficients(impl::Interval<T>(near, far), impl::Interval<T>(-near, far));
        
        return impl::Matrix4<T>(A_x * near, T(0), B_x, T(0),
                                T(0), A_y * near, B_y, T(0),
                                T(0), T(0), A_z,  B_z, T(0),
                                T(0), T(0), T(1) /** Copy the Z into W, for divide, maybe invert because of handedness change? */, T(0));
        // Answer: NO, refer to https://www.gingerbill.org/article/2024/11/10/opengl-is-not-right-handed for more information    
        // so yeah NDC is "left-handed" for most, but the normal convention for us, in fact the convention that we use +X right, +Y up, +Z forward
    }

    template<impl::IsFloat T>
    constexpr impl::Matrix4<T> symmetric_frustum(T width, T height, T near, T far) /** Equivalent to frustum(-width, width, -height, height, near, far) */
    {
        auto [A_z, B_z] = impl::Interval<T>::interval_lerp_coefficients(impl::Interval<T>(near, far), impl::Interval<T>(-near, far));

        return impl::Matrix4<T>(near / width, T(0), T(0), T(0),
                                T(0), near / height, T(0), T(0),
                                T(0), T(0), A_z, B_z,
                                T(0), T(0), T(1), T(0));
    }

    /**
     * 
     *  Creates a frustum defined by vertical field of view, together with the near plane distance,
     *  and the view plane is adjusted to match the aspect ratio provided by the user, which is typically the window's
     *  aspect ratio
     * 
     */
    template<impl::IsFloat T>
    constexpr impl::Matrix4<T> fov_aspect_ratio_frustum(T horizontal_fov, T aspect_ratio, T near, T far)
    {
        auto [A_z, B_z] = impl::Interval<T>::interval_lerp_coefficients(impl::Interval<T>(near, far), impl::Interval<T>(-near, far));

        return impl::Matrix4<T>(T(1) / std::tan(horizontal_fov / T(2)), T(0), T(0), T(0),
                                T(0), (T(1) / (std::tan(horizontal_fov / T(2)) * aspect_ratio)), T(0), T(0),
                                T(0), T(0), A_z, B_z,
                                T(0), T(0), T(1), T(0));
    }
}