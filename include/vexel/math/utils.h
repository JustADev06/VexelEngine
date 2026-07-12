#pragma once

#include "vexel/math/interval.h"
#include "vexel/math/matrix4.h"
#include "vexel/math/projection_matrix.h"
#include "vexel/math/rotor.h"
#include "vexel/math/vector3.h"
#include "vexel/math/vector4.h"

namespace vexel
{
    using Vector3 = impl::Vector3<float>;
    using Vector4 = impl::Vector4<float>;
    using Matrix4 = impl::Matrix4<float>;
    using Rotor = impl::Rotor<float>;
    using Interval = impl::Interval<float>;

    using Vector3d = impl::Vector3<double>;
    using Vector4d = impl::Vector4<double>;
    using Matrix4d = impl::Matrix4<double>;
    using Rotord = impl::Rotor<double>;
    using Intervald = impl::Interval<double>;
}