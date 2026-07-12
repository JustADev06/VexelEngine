#include "vexel/graphics/texture.h"

#include <cmath>

namespace vexel
{
    inline GLenum to_opengl(TextureFormat format)
    {
        switch(format)
        {
        case TextureFormat::R8_UNORM: return GL_R8; break;
        case TextureFormat::R8_SNORM: return GL_R8_SNORM; break;
        case TextureFormat::R_F16: return GL_R16F; break;
        case TextureFormat::R_F32: return GL_R32F; break;
        case TextureFormat::R_UINT8: return GL_R8UI; break;
        case TextureFormat::R_UINT16: return GL_R16UI; break;
        case TextureFormat::R_UINT32: return GL_R32UI; break;
        case TextureFormat::R_INT8: return GL_R8I; break;
        case TextureFormat::R_INT16: return GL_R16I; break;
        case TextureFormat::R_INT32: return GL_R32I; break;
        case TextureFormat::RG88_UNORM: return GL_RG8; break;
        case TextureFormat::RG88_SNORM: return GL_RG8_SNORM; break;
        case TextureFormat::RG_F16: return GL_RG16F; break;
        case TextureFormat::RG_F32: return GL_RG32F; break;
        case TextureFormat::RG_UINT8: return GL_RG8UI; break;
        case TextureFormat::RG_UINT16: return GL_RG16UI; break;
        case TextureFormat::RG_UINT32: return GL_RG32UI; break;
        case TextureFormat::RG_INT8: return GL_RG8I; break;
        case TextureFormat::RG_INT16: return GL_RG16I; break;
        case TextureFormat::RG_INT32: return GL_RG32I; break;
        case TextureFormat::RGB888_UNORM: return GL_RGB8; break;
        case TextureFormat::RGB888_SNORM: return GL_RGB8_SNORM; break;
        case TextureFormat::RGB_F16: return GL_RGB16F; break;
        case TextureFormat::RGB_F32: return GL_RGB32F; break;
        case TextureFormat::RGB_UINT8: return GL_RGB8UI; break;
        case TextureFormat::RGB_UINT16: return GL_RGB16UI; break;
        case TextureFormat::RGB_UINT32: return GL_RGB32UI; break;
        case TextureFormat::RGB_INT8: return GL_RGB8I; break;
        case TextureFormat::RGB_INT16: return GL_RGB16I; break;
        case TextureFormat::RGB_INT32: return GL_RGB32I; break;
        case TextureFormat::RGBA8888_UNORM: return GL_RGBA8; break;
        case TextureFormat::RGBA8888_SNORM: return GL_RGBA8_SNORM; break;
        case TextureFormat::RGBA_F16: return GL_RGBA16F; break;
        case TextureFormat::RGBA_F32: return GL_RGBA32F; break;
        case TextureFormat::RGBA_UINT8: return GL_RGBA8UI; break;
        case TextureFormat::RGBA_UINT16: return GL_RGBA16UI; break;
        case TextureFormat::RGBA_UINT32: return GL_RGBA32UI; break;
        case TextureFormat::RGBA_INT8: return GL_RGBA8I; break;
        case TextureFormat::RGBA_INT16: return GL_RGBA16I; break;
        case TextureFormat::RGBA_INT32: return GL_RGBA32I; break;
        case TextureFormat::SRGB888: return GL_SRGB8; break;
        case TextureFormat::SRGBA8888: return GL_SRGB8_ALPHA8; break;
        case TextureFormat::DEPTH16: return GL_DEPTH_COMPONENT16; break;
        case TextureFormat::DEPTH24: return GL_DEPTH_COMPONENT24; break;
        case TextureFormat::DEPTH32F: return GL_DEPTH_COMPONENT32F; break;
        case TextureFormat::STENCIL8: return GL_STENCIL_INDEX8; break;
        case TextureFormat::DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8; break;
        case TextureFormat::DEPTH32F_STENCIL8: return GL_DEPTH32F_STENCIL8; break;
        default: return GL_NONE; break;
        }

        return GL_NONE;
    }

    inline GLenum to_opengl_input_format(TextureFormat format)
    {
        switch(format)
        {
        case TextureFormat::R8_UNORM: 
        case TextureFormat::R8_SNORM: 
        case TextureFormat::R_F16:
        case TextureFormat::R_F32:
        case TextureFormat::R_UINT8:
        case TextureFormat::R_UINT16:
        case TextureFormat::R_UINT32:
        case TextureFormat::R_INT8:
        case TextureFormat::R_INT16:
        case TextureFormat::R_INT32: return GL_RED; break;
        case TextureFormat::RG88_UNORM:
        case TextureFormat::RG88_SNORM:
        case TextureFormat::RG_F16:
        case TextureFormat::RG_F32:
        case TextureFormat::RG_UINT8:
        case TextureFormat::RG_UINT16:
        case TextureFormat::RG_UINT32:
        case TextureFormat::RG_INT8:
        case TextureFormat::RG_INT16:
        case TextureFormat::RG_INT32: return GL_RG; break;
        case TextureFormat::RGB888_UNORM:
        case TextureFormat::RGB888_SNORM:
        case TextureFormat::RGB_F16:
        case TextureFormat::RGB_F32:
        case TextureFormat::RGB_UINT8:
        case TextureFormat::RGB_UINT16:
        case TextureFormat::RGB_UINT32:
        case TextureFormat::RGB_INT8:
        case TextureFormat::RGB_INT16:
        case TextureFormat::RGB_INT32: return GL_RGB; break;
        case TextureFormat::RGBA8888_UNORM:
        case TextureFormat::RGBA8888_SNORM:
        case TextureFormat::RGBA_F16:
        case TextureFormat::RGBA_F32:
        case TextureFormat::RGBA_UINT8:
        case TextureFormat::RGBA_UINT16:
        case TextureFormat::RGBA_UINT32:
        case TextureFormat::RGBA_INT8:
        case TextureFormat::RGBA_INT16:
        case TextureFormat::RGBA_INT32: return GL_RGBA; break;
        case TextureFormat::SRGB888: return GL_RGB; break;
        case TextureFormat::SRGBA8888: return GL_RGBA; break;
        case TextureFormat::DEPTH16: return GL_DEPTH_COMPONENT; break;
        case TextureFormat::DEPTH24: return GL_DEPTH_COMPONENT; break;
        case TextureFormat::DEPTH32F: return GL_DEPTH_COMPONENT; break;
        case TextureFormat::STENCIL8: return GL_STENCIL_INDEX; break;
        case TextureFormat::DEPTH24_STENCIL8: return GL_DEPTH_STENCIL; break;
        case TextureFormat::DEPTH32F_STENCIL8: return GL_DEPTH_STENCIL; break;
        default: return GL_NONE; break;
        }

        return GL_NONE;
    }

    inline GLenum to_opengl_input_type(TextureFormat format)
    {
        if(static_cast<std::uint8_t>(format) >= static_cast<std::uint8_t>(TextureFormat::SRGB888) /** We're at SRGB or bigger, here it is just case checking */)
        {
            switch(format)
            {
            case TextureFormat::SRGB888: return GL_UNSIGNED_BYTE; break;
            case TextureFormat::SRGBA8888: return GL_UNSIGNED_BYTE; break;
            case TextureFormat::DEPTH16:
            case TextureFormat::DEPTH24:
            case TextureFormat::DEPTH32F:
            case TextureFormat::STENCIL8:
            case TextureFormat::DEPTH24_STENCIL8:
            case TextureFormat::DEPTH32F_STENCIL8: return GL_NONE; /** You don't provide input */ break;
            default: return GL_NONE; break;
            }

            return GL_NONE;
        }
        
        switch(static_cast<std::uint8_t>(format) % 10)
        {
        case 0: return GL_UNSIGNED_BYTE; break;
        case 1: return GL_BYTE; break;
        case 2: return GL_FLOAT /** There's no half float, only conversion from a default float */; break;
        case 3: return GL_FLOAT; break;
        case 4: return GL_UNSIGNED_BYTE; break;
        case 5: return GL_UNSIGNED_SHORT; break;
        case 6: return GL_UNSIGNED_INT; break;
        case 7: return GL_BYTE; break;
        case 8: return GL_SHORT; break;
        case 9: return GL_INT; break;
        default: return GL_NONE; break;
        }

        return GL_NONE;
    }

    Texture::Texture(std::uint32_t width, std::uint32_t height, TextureFormat format, bool uses_mipmaps, std::uint8_t samples)
        : handle_(0)
        , width_(width)
        , height_(height)
        , format_(format)
        , samples_(samples)
        , max_mipmap_level_(static_cast<std::uint32_t>(std::floor(std::log2(std::max(width_, height_)))))
        , gl_internal_format_(to_opengl(format))
        , gl_format_(to_opengl_input_format(format))
        , gl_type_(to_opengl_input_type(format))
        , uses_mipmaps_(samples_ > 1u ? false : uses_mipmaps)
    {
        GLenum target = samples_ > 1u ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        glCreateTextures(target, 1, &handle_);
        
        if(samples_ == 1u)
        {
            glTextureStorage2D(handle_, uses_mipmaps_ ? max_mipmap_level_ + 1 : 1, gl_internal_format_, width_, height_);
            if(uses_mipmaps_) // Just construct them from garbage data so they "exist", as in registered 
            {
                glGenerateTextureMipmap(handle_);
            }
        }
        else
        {
            glTextureStorage2DMultisample(handle_, samples_, gl_internal_format_, width_, height_, GL_TRUE);
        }
    }

    Texture::Texture(std::uint32_t width, std::uint32_t height, TextureFormat format, bool uses_mipmaps, std::byte *data)
        : handle_(0)
        , width_(width)
        , height_(height)
        , format_(format)
        , samples_(1u)
        , max_mipmap_level_(static_cast<std::uint32_t>(std::floor(std::log2(std::max(width_, height_)))))
        , gl_internal_format_(to_opengl(format))
        , gl_format_(to_opengl_input_format(format))
        , gl_type_(to_opengl_input_type(format))
        , uses_mipmaps_(uses_mipmaps)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &handle_);
        glTextureStorage2D(handle_, uses_mipmaps_ ? max_mipmap_level_ + 1 : 1, gl_internal_format_, width_, height_);
        glTextureSubImage2D(handle_, 0, 0, 0, width_, height_, gl_format_, gl_type_, data);

        if(uses_mipmaps_)
        {
            glGenerateTextureMipmap(handle_);
        }
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &handle_);
    }

    GLuint Texture::handle() const
    {
        return handle_;
    }

    std::uint32_t Texture::width() const
    {
        return width_;
    }

    std::uint32_t Texture::height() const
    {
        return height_;
    }
        
    TextureFormat Texture::format() const
    {
        return format_;
    }

    std::uint8_t Texture::samples() const
    {
        return samples_;
    }

    bool Texture::multisampled() const
    {
        return samples_ > 1;
    }

    std::uint32_t Texture::max_mipmap_level() const
    {
        return max_mipmap_level_;
    }

    bool Texture::uses_mipmaps() const
    {
        return uses_mipmaps_;
    }

    void Texture::specify_image(std::byte *data)
    {
        specify_subimage(0, 0, width_, height_, data);
    }

    void Texture::specify_subimage(std::uint32_t xoffset, std::uint32_t yoffset, std::uint32_t width, std::uint32_t height, std::byte *data)
    {
        if(multisampled())
        {
            // TODO: Call code red, but seriously, cause an error/error message       
        }

        glTextureSubImage2D(handle_, 0, xoffset, yoffset, width, height, gl_format_, gl_type_, data);
        if(uses_mipmaps_)
        {
            glGenerateTextureMipmap(handle_);
        }
    }

    void Texture::specify_mipmap_level(std::uint32_t level /** in [0, max_mipmap_levels()] */, std::byte *data)
    {
        specify_mipmap_level_subimage(level, 0, 0, data);
    }

    void Texture::specify_mipmap_level_subimage(std::uint32_t level /** in [0, max_mipmap_levels()] */, std::uint32_t xoffset, std::uint32_t yoffset, std::byte *data)
    {
        if((level > max_mipmap_level_) || multisampled())
        {
            // TODO: Call code red, but seriously, cause an error/error message
        }

        glTextureSubImage2D(handle_, level, xoffset, yoffset, std::max(1u, width_ >> level), std::max(1u, height_ >> level), gl_format_, gl_type_, data);
    }   

    void Texture::rebuild_mipmaps()
    {
        if(uses_mipmaps_)
        {
            glGenerateTextureMipmap(handle_);
        }
    }

    bool Texture::is_depth() const
    {
        switch(format_)
        {
        case TextureFormat::DEPTH16:
        case TextureFormat::DEPTH24:
        case TextureFormat::DEPTH24_STENCIL8:
        case TextureFormat::DEPTH32F:
        case TextureFormat::DEPTH32F_STENCIL8: return true; break;
        default: return false; break;
        }

        return false;
    }

    bool Texture::is_stencil() const
    {
        switch(format_)
        {
        case TextureFormat::STENCIL8:
        case TextureFormat::DEPTH24_STENCIL8:
        case TextureFormat::DEPTH32F_STENCIL8: return true; break;
        default: return false; break;
        }

        return false;
    }

    bool Texture::is_depth_stencil() const
    {
        switch(format_)
        {
        case TextureFormat::DEPTH24_STENCIL8:
        case TextureFormat::DEPTH32F_STENCIL8: return true; break;
        default: return false; break;
        }

        return false;
    }
}