#pragma once

#include "vexel/math/vector3.h" // IsFloat

namespace vexel
{
    namespace impl
    {
        
    template<IsFloat T>
    class Interval
    {
    public:
        constexpr Interval(T a, T b)
            : a_(a)
            , b_(b)
            , len_(b - a)
        {
        }

        constexpr Interval(T t)
            : a_(-t)
            , b_(t)
            , len_(T(2) * t)
        {
        }

        constexpr T a() const
        {
            return a_;
        }

        constexpr T b() const
        {
            return b_;
        }

        constexpr T diff() const
        {
            return len_;
        }

        constexpr T len() const
        {
            return std::abs(len_);
        }

        constexpr bool outside(T x) const
        {
            return ((x < a_) || (x > b_));
        }

        constexpr bool inside(T x) const
        {
            return !outside(x);
        }

        static constexpr bool outside(T x, const Interval &ab)
        {
            return ab.outside(x);
        }
        
        static constexpr bool inside(T x, const Interval &ab)
        {
            return ab.inside(x);
        }

        constexpr T lerp(T t) const
        {
            return a + t * len_;
        }

        static constexpr T lerp(T t, const Interval &ab)
        {
            return ab.lerp(t);
        }

        constexpr T interval_lerp(T x, const Interval &other) const
        {
            return (other.len_ / len_) * x + ((b_ * other.a_ - a_ * other.b_) / len_);
        }

        static constexpr T interval_lerp(T x, const Interval &ab, const Interval &cd)
        {
            return ab.interval_lerp(x, cd);
        }

        constexpr std::pair<T, T> interval_lerp_coefficients(const Interval &other) const
        {
            return std::make_pair(other.len_ / len_, (b_ * other.a_ - a_ * other.b_) / len_);
        }

        static constexpr std::pair<T, T> interval_lerp_coefficients(const Interval &ab, const Interval &cd)
        {
            return ab.interval_lerp_coefficients(cd);
        }

    private:
        T a_;
        T b_;
        T len_; // Oriented!
    };

    }

    template<impl::IsFloat T>
    constexpr impl::Interval minus_one_to_one = impl::Interval<T>(T(1));
}