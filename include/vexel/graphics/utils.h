#pragma once

#include <cstdint>
#include <algorithm>

#include "glad/gl.h"

namespace vexel
{
    #define VEXEL_DELETE_COPY_MOVE(name) \
        name (const name &) = delete; \
        name (name &&) noexcept = delete; \
        name  &operator=(const name &) = delete; \
        name  &operator=(name &&) noexcept = delete; \

    union ColorValue
    {
        float rgba_f32[4]; // Used for normalized [0, 1], [-1, 1] and F16 and F32 floating-point formats, this is always in linear space, even for sRGB
        std::uint32_t rgba_u32[4]; // Used for the raw unsigned integer formats
        std::int32_t rgba_i32[4]; // // Used for the raw signed integer formats
        float depth;
        std::uint8_t stencil;
        struct DepthStencil
        {
            float depth;
            std::uint8_t stencil;
        };
        DepthStencil depth_stencil;
    };

    struct Color
    {
        constexpr Color(std::uint8_t r,
                        std::uint8_t g,
                        std::uint8_t b,
                        std::uint8_t a)
            : r(static_cast<float>(r) / 255.0f)
            , g(static_cast<float>(g) / 255.0f)
            , b(static_cast<float>(b) / 255.0f)
            , a(static_cast<float>(a) / 255.0f)
        {
        }

        constexpr Color(std::int8_t r,
                        std::int8_t g,
                        std::int8_t b,
                        std::int8_t a)
            : r(std::clamp(static_cast<float>(r) / 127.0f, -1.0f, 1.0f))
            , g(std::clamp(static_cast<float>(g) / 127.0f, -1.0f, 1.0f))
            , b(std::clamp(static_cast<float>(b) / 127.0f, -1.0f, 1.0f))
            , a(std::clamp(static_cast<float>(a) / 127.0f, -1.0f, 1.0f))
        {
        }

        constexpr Color(float r,
                        float g,
                        float b,
                        float a)
            : r(r)
            , g(g)
            , b(b)
            , a(a)
        {
        }

        constexpr explicit operator ColorValue() const
        {
            ColorValue value;
            value.rgba_f32[0] = r;
            value.rgba_f32[1] = g;
            value.rgba_f32[2] = b;
            value.rgba_f32[3] = a;
            return value;
        }

        union
        {
            struct
            {
                float r;
                float g;
                float b;
                float a;
            };

            struct
            {
                float rgb[3];
                float alpha;
            };

            float rgba[4];
        };
    };
}