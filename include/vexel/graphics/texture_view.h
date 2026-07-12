#pragma once

#include "vexel/graphics/texture.h"
#include "vexel/graphics/cubemap.h"
#include "vexel/graphics/sampler.h"

namespace vexel
{
    class TextureView
    {
    public:
        TextureView(Texture *texture, std::uint8_t mip_level, const ColorValue &border_color, Sampler *sampler);
        TextureView(Texture *texture, std::uint8_t mip_level, Sampler *sampler);
        TextureView(Texture *texture, std::uint8_t mip_level);

        TextureView(Texture *texture, const ColorValue &border_color, Sampler *sampler);
        TextureView(Texture *texture, Sampler *sampler);
        TextureView(Texture *texture);

        TextureView(Cubemap *cubemap, std::uint8_t mip_level, const ColorValue &border_color, Sampler *sampler);
        TextureView(Cubemap *cubemap, std::uint8_t mip_level, Sampler *sampler);
        TextureView(Cubemap *cubemap, std::uint8_t mip_level);
        TextureView(Cubemap *cubemap, std::uint8_t mip_level, CubemapFace face, const ColorValue &border_color, Sampler *sampler);
        TextureView(Cubemap *cubemap, std::uint8_t mip_level, CubemapFace face, Sampler *sampler);
        TextureView(Cubemap *cubemap, std::uint8_t mip_level, CubemapFace face);

        TextureView(Cubemap *cubemap, const ColorValue &border_color, Sampler *sampler);
        TextureView(Cubemap *cubemap, Sampler *sampler);
        TextureView(Cubemap *cubemap);
        TextureView(Cubemap *cubemap, CubemapFace face, const ColorValue &border_color, Sampler *sampler);
        TextureView(Cubemap *cubemap, CubemapFace face, Sampler *sampler);
        TextureView(Cubemap *cubemap, CubemapFace face);

        virtual ~TextureView();

        std::uint32_t width() const;

        std::uint32_t height() const;

        void set_sampler(Sampler *sampler);

        Sampler *sampler() const;

        GLuint64 bindless_handle() const;
        
        GLuint handle() const;

        std::uint8_t mip_level() const; // Returns 0 if the whole texture, independent of whether all of the mipmaps are used or not
        
        CubemapFace face() const; // Returns POSITIVE_X if it's just a 2D texture or the whole cubemap

        ColorValue border_color() const;

        void set_border_color(const ColorValue &border_color);
        
        bool is_depth() const;
        
        bool is_depth_stencil() const;
        
        bool is_stencil() const;

        bool multisampled() const;

        TextureFormat format() const;

    private:
        std::uint32_t width_;

        std::uint32_t height_;

        TextureFormat format_;

        GLuint handle_;

        GLuint64 bindless_handle_;

        CubemapFace face_;

        ColorValue border_color_;

        std::uint8_t mip_level_;
        
        Sampler *sampler_;

        union
        {
            Texture *texture_;
            Cubemap *cubemap_;
        };

        bool is_cubemap_;
    };
}