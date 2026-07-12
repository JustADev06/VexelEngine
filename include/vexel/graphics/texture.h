#pragma once

#include <cstddef>

#include "vexel/graphics/utils.h"

#include "glad/gl.h"

namespace vexel
{
    // Refer to required formats, to have a nice idea of which are supported universally
    // Required formats: https://wikis.khronos.org/opengl/Image_Format
    enum class TextureFormat : std::uint8_t
    {
        /** Similar trick as with the vertex attributes, we just work mod 10 to get the input variable type */
        R8_UNORM = 0,
        R8_SNORM = 1,
        R_F16 = 2,
        R_F32 = 3,
        R_UINT8 = 4,
        R_UINT16 = 5,
        R_UINT32 = 6,
        R_INT8 = 7,
        R_INT16 = 8,
        R_INT32 = 9,
        RG88_UNORM = 10,
        RG88_SNORM = 11,
        RG_F16 = 12,
        RG_F32 = 13,
        RG_UINT8 = 14,
        RG_UINT16 = 15,
        RG_UINT32 = 16,
        RG_INT8 = 17,
        RG_INT16 = 18,
        RG_INT32 = 19,
        RGB888_UNORM = 20,
        RGB888_SNORM = 21,
        RGB_F16 = 22,
        RGB_F32 = 23,
        RGB_UINT8 = 24,
        RGB_UINT16 = 25,
        RGB_UINT32 = 26,
        RGB_INT8 = 27,
        RGB_INT16 = 28,
        RGB_INT32 = 29,
        RGBA8888_UNORM = 30,
        RGBA8888_SNORM = 31,
        RGBA_F16 = 32,
        RGBA_F32 = 33,
        RGBA_UINT8 = 34,
        RGBA_UINT16 = 35,
        RGBA_UINT32 = 36,
        RGBA_INT8 = 37,
        RGBA_INT16 = 38,
        RGBA_INT32 = 39,
        SRGB888 = 40,
        SRGBA8888 = 41,
        DEPTH16 = 42,
        DEPTH24 = 43,
        DEPTH32F = 44,
        STENCIL8 = 45,
        DEPTH24_STENCIL8 = 46,
        DEPTH32F_STENCIL8 = 47
    };

    class Texture
    {
    public:
        Texture(std::uint32_t width, std::uint32_t height, TextureFormat format, bool uses_mipmaps, std::uint8_t samples = 1u);
        Texture(std::uint32_t width, std::uint32_t height, TextureFormat format, bool uses_mipmaps, std::byte *data);
        virtual ~Texture();
    
        VEXEL_DELETE_COPY_MOVE(Texture);

        GLuint handle() const;

        std::uint32_t width() const;

        std::uint32_t height() const;
        
        TextureFormat format() const;

        std::uint8_t samples() const;

        bool multisampled() const;

        // Non-zero inclusive, if only 0-th level, then just returns 0, if 0 level and 1 level, then returns 1, and so on
        std::uint32_t max_mipmap_level() const;

        /** Whether the actual texture uses mipmaps, NOT the sampler */
        bool uses_mipmaps() const;

        void specify_image(std::byte *data);

        void specify_subimage(std::uint32_t xoffset, std::uint32_t yoffset, std::uint32_t width, std::uint32_t height, std::byte *data);

        // TODO: Maybe add option here to not regenerate mipmaps and let then the user specify them himself via these below
        void specify_mipmap_level(std::uint32_t level /** in [0, max_mipmap_levels()] */, std::byte *data);

        void specify_mipmap_level_subimage(std::uint32_t level /** in [0, max_mipmap_levels()] */, std::uint32_t xoffset, std::uint32_t yoffset, std::byte *data);

        void rebuild_mipmaps();

        bool is_depth() const;

        bool is_stencil() const;

        bool is_depth_stencil() const;

    private:
        GLuint handle_;

        std::uint32_t width_;

        std::uint32_t height_;

        TextureFormat format_;

        std::uint8_t samples_;

        std::uint32_t max_mipmap_level_;

        GLint gl_internal_format_;

        GLenum gl_format_;

        GLenum gl_type_;

        bool uses_mipmaps_;
    };
}