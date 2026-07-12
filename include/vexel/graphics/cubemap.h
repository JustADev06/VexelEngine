#pragma once

#include <cstddef>
#include <array>
#include <cmath>

#include "vexel/graphics/utils.h"
#include "vexel/graphics/texture.h"

#include "glad/gl.h"

namespace vexel
{
    enum class CubemapFace : std::uint8_t
    {
        POSITIVE_X = 0,
        NEGATIVE_X = 1,
        POSITIVE_Y = 2,
        NEGATIVE_Y = 3,
        POSITIVE_Z = 4,
        NEGATIVE_Z = 5
    };

    class Cubemap
    {
    public:
        Cubemap(std::uint32_t width, std::uint32_t height, TextureFormat format, bool uses_mipmaps);
        Cubemap(std::uint32_t width, std::uint32_t height, TextureFormat format, bool uses_mipmaps, const std::array<std::byte *, 6u> &data);
        virtual ~Cubemap();
    
        VEXEL_DELETE_COPY_MOVE(Cubemap);

        GLuint handle() const;

        std::uint32_t width() const;

        std::uint32_t height() const; // Same as width, because a it's a cube, duuh
        
        TextureFormat format() const;

        // Non-zero inclusive, if only 0-th level, then just returns 0, if 0 level and 1 level, then returns 1, and so on
        std::uint32_t max_mipmap_level() const;

        /** Whether the actual texture uses mipmaps, NOT the sampler */
        bool uses_mipmaps() const;

        void specify_image(CubemapFace face, std::byte *data, bool rebuild_cubemap_mipmaps = false /** Whether to rebuild all of them, for all faces */);

        void specify_cubemap(const std::array<std::byte *, 6u> &data);

        void specify_subimage(CubemapFace face, std::uint32_t xoffset, std::uint32_t yoffset, std::uint32_t width, std::uint32_t height, std::byte *data, bool rebuild_cubemap_mipmaps = false /** Whether to rebuild all of them, for all faces */);

        // TODO: Maybe add option here to not regenerate mipmaps and let then the user specify them himself via these below
        void specify_mipmap_level(CubemapFace face, std::uint32_t level /** in [0, max_mipmap_levels()] */, std::byte *data);

        void specify_cubemap_level(std::uint32_t level, const std::array<std::byte *, 6u> &data);

        void specify_mipmap_level_subimage(CubemapFace face, std::uint32_t level /** in [0, max_mipmap_levels()] */, std::uint32_t xoffset, std::uint32_t yoffset, std::byte *data);

        // For the use case, where you don't want to rebuild the whole cubemap mipmaps automatically because you just wrote a new image to one face,
        // so instead you do it manually
        void rebuild_mipmaps();
        
        bool is_depth() const;

        bool is_stencil() const;

        bool is_depth_stencil() const;

    private:
        GLuint handle_;
    
        std::uint32_t width_;

        std::uint32_t height_;

        TextureFormat format_;

        std::uint32_t max_mipmap_level_;

        GLint gl_internal_format_;

        GLenum gl_format_;

        GLenum gl_type_;

        bool uses_mipmaps_;
    };
}