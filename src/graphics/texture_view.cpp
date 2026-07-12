#include "vexel/graphics/texture_view.h"

#include <iostream>

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

    // Main constructors for the Texture class, other are just with some arguments zero or defaults

    TextureView::TextureView(Texture *texture, std::uint8_t mip_level, const ColorValue &border_color, Sampler *sampler)
        : width_(0)
        , height_(0)
        , format_(TextureFormat::R8_UNORM)
        , handle_(0)
        , bindless_handle_(0)
        , face_(CubemapFace::POSITIVE_X)
        , border_color_(border_color)
        , mip_level_(mip_level)
        , sampler_(nullptr) // Setup later
        , texture_(texture)
        , is_cubemap_(false)
    {
        if(!texture_)
        {
            // TODO: Throw an error here!
        }

        if(!texture_->uses_mipmaps() && (mip_level_ > 0))
        {
            // TODO: Throw an error here!
        }

        bool multisampled = texture_->multisampled();
        GLenum target = multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        if((mip_level_ > texture_->max_mipmap_level()) || ((mip_level_ != 0) && multisampled))
        {
            // TODO: Throw an error here!
        }

        width_ = std::max(1u, texture_->width() >> mip_level_);
        height_ = std::max(1u, texture_->height() >> mip_level_);
        format_ = texture_->format();

        // Yep it's actually a texture, just with a linked storage, lol
        //glCreateTextures(target, 1u, &handle_);           THE FUCK IT DOESN'T SUPPORT FUCKING DSA, LOOK AT: https://gitlab.freedesktop.org/mesa/mesa/-/work_items/15822
        glGenTextures(1u, &handle_); // Data reinterpretation is actually not used by us
        glTextureView(handle_, target, texture_->handle(), to_opengl(texture_->format()), mip_level_, 1, 0, 1);

        set_sampler(sampler);
    }

    TextureView::TextureView(Texture *texture, const ColorValue &border_color, Sampler *sampler)
        : width_(0)
        , height_(0)
        , format_(TextureFormat::R8_UNORM)
        , handle_(0)
        , bindless_handle_(0)
        , face_(CubemapFace::POSITIVE_X)
        , border_color_(border_color)
        , mip_level_(0)
        , sampler_(nullptr) // Setup later
        , texture_(texture)
        , is_cubemap_(false)
    {
        if(!texture_)
        {
            // TODO: Throw an error here!
        }

        width_ = texture_->width();
        height_ = texture_->height();
        format_ = texture_->format();

        bool multisampled = texture_->multisampled();
        GLenum target = multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

        // Yep it's actually a texture, just with a linked storage, lol
        //glCreateTextures(target, 1u, &handle_);          THE FUCK IT DOESN'T SUPPORT FUCKING DSA, LOOK AT: https://gitlab.freedesktop.org/mesa/mesa/-/work_items/15822
        glGenTextures(1u, &handle_); // Data reinterpretation is actually not used by us
        glTextureView(handle_, target, texture_->handle(), to_opengl(texture_->format()), mip_level_, texture_->uses_mipmaps() ? texture_->max_mipmap_level() + 1 : 1, 0, 1);

        set_sampler(sampler);
    }


    
    TextureView::TextureView(Texture *texture, std::uint8_t mip_level, Sampler *sampler)
        : TextureView(texture, mip_level, ColorValue{.rgba_f32 = {0.0f, 0.0f, 0.0f, 0.0f}}, sampler)
    {
    }
    
    TextureView::TextureView(Texture *texture, std::uint8_t mip_level)
        : TextureView(texture, mip_level, ColorValue{.rgba_f32 = {0.0f, 0.0f, 0.0f, 0.0f}}, nullptr)
    {
    }
    
    TextureView::TextureView(Texture *texture, Sampler *sampler)
        : TextureView(texture, ColorValue{.rgba_f32 = {0.0f, 0.0f, 0.0f, 0.0f}}, sampler)
    {
    }

    TextureView::TextureView(Texture *texture)
        : TextureView(texture, ColorValue{.rgba_f32 = {0.0f, 0.0f, 0.0f, 0.0f}}, nullptr)
    {
    }

    TextureView::TextureView(Cubemap *cubemap, std::uint8_t mip_level, const ColorValue &border_color, Sampler *sampler)
        : width_(0)
        , height_(0)
        , format_(TextureFormat::R8_UNORM)
        , handle_(0)
        , bindless_handle_(0)
        , face_(CubemapFace::POSITIVE_X)
        , border_color_(border_color)
        , mip_level_(mip_level)
        , sampler_(nullptr) // Setup later
        , cubemap_(cubemap)
        , is_cubemap_(true)
    {
        if(!cubemap_)
        {
            // TODO: Throw an error here!
        }

        if(!cubemap_->uses_mipmaps() && (mip_level_ > 0))
        {
            // TODO: Throw an error here!
        }

        if(mip_level_ > cubemap_->max_mipmap_level())
        {
            // TODO: Throw an error here!
        }

        width_ = std::max(1u, cubemap_->width() >> mip_level_);
        height_ = std::max(1u, cubemap_->height() >> mip_level_);
        format_ = texture_->format();

        // Yep it's actually a texture, just with a linked storage, lol
        //glCreateTextures(GL_TEXTURE_CUBE_MAP, 1u, &handle_);           
        glGenTextures(1u, &handle_); // Data reinterpretation is actually not used by us
        glTextureView(handle_, GL_TEXTURE_CUBE_MAP, cubemap_->handle(), to_opengl(cubemap_->format()), mip_level_, 1, 0, 6 /** Six faces */);

        set_sampler(sampler);
    }

    TextureView::TextureView(Cubemap *cubemap, std::uint8_t mip_level, CubemapFace face, const ColorValue &border_color, Sampler *sampler)
        : width_(0)
        , height_(0)
        , format_(TextureFormat::R8_UNORM)
        , handle_(0)
        , bindless_handle_(0)
        , face_(face)
        , border_color_(border_color)
        , mip_level_(mip_level)
        , sampler_(nullptr) // Setup later
        , cubemap_(cubemap)
        , is_cubemap_(true) // This is not exposed, so we can use it as we want, even tho we actually have the view being a particular face
    {
        if(!cubemap_)
        {
            // TODO: Throw an error here!
        }

        if(!cubemap_->uses_mipmaps() && (mip_level_ > 0))
        {
            // TODO: Throw an error here!
        }

        if(mip_level_ > cubemap_->max_mipmap_level())
        {
            // TODO: Throw an error here!
        }

        width_ = std::max(1u, cubemap_->width() >> mip_level_);
        height_ = std::max(1u, cubemap_->height() >> mip_level_);
        format_ = texture_->format();

        // Yep it's actually a texture, just with a linked storage, lol
        //glCreateTextures(GL_TEXTURE_2D, 1u, &handle_);
        glGenTextures(1u, &handle_); // Data reinterpretation is actually not used by us           
        glTextureView(handle_, GL_TEXTURE_2D, cubemap_->handle(), to_opengl(cubemap_->format()), mip_level_, 1, static_cast<GLuint>(face_), 1);

        set_sampler(sampler);
    }

    TextureView::TextureView(Cubemap *cubemap, const ColorValue &border_color, Sampler *sampler)
        : width_(0)
        , height_(0)
        , format_(TextureFormat::R8_UNORM)
        , handle_(0)
        , bindless_handle_(0)
        , face_(CubemapFace::POSITIVE_X)
        , border_color_(border_color)
        , mip_level_(0)
        , sampler_(nullptr) // Setup later
        , cubemap_(cubemap)
        , is_cubemap_(true)
    {
        if(!cubemap_)
        {
            // TODO: Throw an error here!
        }

        width_ = cubemap_->width();
        height_ = cubemap_->height();
        format_ = texture_->format();

        // Yep it's actually a texture, just with a linked storage, lol
        //glCreateTextures(GL_TEXTURE_CUBE_MAP, 1u, &handle_);           
        glGenTextures(1u, &handle_);
        glTextureView(handle_, GL_TEXTURE_CUBE_MAP, cubemap_->handle(), to_opengl(cubemap_->format()), 0, cubemap_->uses_mipmaps() ? cubemap_->max_mipmap_level() + 1 : 1, 0, 6 /** Six faces */);

        set_sampler(sampler);
    }

    TextureView::TextureView(Cubemap *cubemap, CubemapFace face, const ColorValue &border_color, Sampler *sampler)
        : width_(0)
        , height_(0)
        , format_(TextureFormat::R8_UNORM)
        , handle_(0)
        , bindless_handle_(0)
        , face_(face)
        , border_color_(border_color)
        , mip_level_(0)
        , sampler_(nullptr) // Setup later
        , cubemap_(cubemap)
        , is_cubemap_(true) // This is not exposed, so we can use it as we want, even tho we actually have the view being a particular face
    {
        if(!cubemap_)
        {
            // TODO: Throw an error here!
        }

        width_ = cubemap_->width();
        height_ = cubemap_->height();
        format_ = texture_->format();

        // Yep it's actually a texture, just with a linked storage, lol
        //glCreateTextures(GL_TEXTURE_2D, 1u, &handle_);           
        glGenTextures(1u, &handle_); // Data reinterpretation is actually not used by us
        glTextureView(handle_, GL_TEXTURE_2D, cubemap_->handle(), to_opengl(cubemap_->format()), 0, cubemap_->uses_mipmaps() ? cubemap_->max_mipmap_level() + 1 : 1, static_cast<GLuint>(face_), 1);

        set_sampler(sampler);
    }




    TextureView::TextureView(Cubemap *cubemap, std::uint8_t mip_level, Sampler *sampler)
        : TextureView(cubemap, mip_level, ColorValue{.rgba_f32 = {0.0f, 0.0f, 0.0f, 0.0f}}, sampler)
    {
    }

    TextureView::TextureView(Cubemap *cubemap, std::uint8_t mip_level)
        : TextureView(cubemap, mip_level, ColorValue{.rgba_f32 = {0.0f, 0.0f, 0.0f, 0.0f}}, nullptr)
    {
    }

    TextureView::TextureView(Cubemap *cubemap, std::uint8_t mip_level, CubemapFace face, Sampler *sampler)
        : TextureView(cubemap, mip_level, face, ColorValue{.rgba_f32 = {0.0f, 0.0f, 0.0f, 0.0f}}, sampler)
    {
    }

    TextureView::TextureView(Cubemap *cubemap, std::uint8_t mip_level, CubemapFace face)
        : TextureView(cubemap, mip_level, face, ColorValue{.rgba_f32 = {0.0f, 0.0f, 0.0f, 0.0f}}, nullptr)
    {
    }

    TextureView::TextureView(Cubemap *cubemap, Sampler *sampler)
        : TextureView(cubemap, ColorValue{.rgba_f32 = {0.0f, 0.0f, 0.0f, 0.0f}}, sampler)
    {
    }

    TextureView::TextureView(Cubemap *cubemap)
        : TextureView(cubemap, ColorValue{.rgba_f32 = {0.0f, 0.0f, 0.0f, 0.0f}}, nullptr)
    {
    }

    TextureView::TextureView(Cubemap *cubemap, CubemapFace face, Sampler *sampler)
        : TextureView(cubemap, face, ColorValue{.rgba_f32 = {0.0f, 0.0f, 0.0f, 0.0f}}, sampler)
    {
    }

    TextureView::TextureView(Cubemap *cubemap, CubemapFace face)
        : TextureView(cubemap, face, ColorValue{.rgba_f32 = {0.0f, 0.0f, 0.0f, 0.0f}}, nullptr)
    {
    }

    TextureView::~TextureView()
    {
        if(bindless_handle_)
        {
            glMakeTextureHandleNonResidentARB(bindless_handle_);
        }

        glDeleteTextures(1, &handle_);
    }

    std::uint32_t TextureView::width() const
    {
        return width_;
    }

    std::uint32_t TextureView::height() const
    {
        return height_;
    }

    void TextureView::set_sampler(Sampler *sampler)
    {
        if(sampler)
        {
            TextureFormat format;
            sampler_ = sampler;
            if(is_cubemap_)
            {
                if(sampler_->uses_mipmaps() && (!(cubemap_->uses_mipmaps())))
                {
                    // TODO: Throw an error here!
                }
                format = cubemap_->format();
            }
            else
            {
                if(sampler_->uses_mipmaps() && (!(texture_->uses_mipmaps())))
                {
                    // TODO: Throw an error here!
                }
                format = texture_->format();
            }

            if((sampler_->u_mode() == WrappingMode::CLAMP_TO_BORDER) ||
               (sampler_->v_mode() == WrappingMode::CLAMP_TO_BORDER) ||
               (sampler_->w_mode() == WrappingMode::CLAMP_TO_BORDER))
            {
                if(static_cast<std::uint8_t>(format) >= static_cast<std::uint8_t>(TextureFormat::SRGB888) /** We're at SRGB or bigger, here it is just case checking */)
                {
                    switch(format)
                    {
                    case TextureFormat::SRGB888:
                    case TextureFormat::SRGBA8888: glSamplerParameterfv(sampler_->handle(), GL_TEXTURE_BORDER_COLOR, border_color_.rgba_f32); break;
                    case TextureFormat::STENCIL8: break; // Yeah sadly there's no border color stencil textures, because you can and can't even sample them explicitly really, but why would you
                    case TextureFormat::DEPTH16:
                    case TextureFormat::DEPTH24:
                    case TextureFormat::DEPTH32F:
                    case TextureFormat::DEPTH24_STENCIL8:
                    case TextureFormat::DEPTH32F_STENCIL8:
                        GLfloat params[4] = {border_color_.depth, 0.0f, 0.0f, 0.0f};
                        glSamplerParameterfv(sampler_->handle(), GL_TEXTURE_BORDER_COLOR, params);
                        break;
                    }
                }
                else
                {
                    switch(static_cast<std::uint8_t>(format) % 10)
                    {
                    case 0:
                    case 1:
                    case 2:
                    case 3: glSamplerParameterfv(sampler_->handle(), GL_TEXTURE_BORDER_COLOR, border_color_.rgba_f32); break;
                    case 4: 
                    case 5:
                    case 6: glSamplerParameterIuiv(sampler_->handle(), GL_TEXTURE_BORDER_COLOR, border_color_.rgba_u32); break;
                    case 7: 
                    case 8: 
                    case 9: glSamplerParameterIiv(sampler_->handle(), GL_TEXTURE_BORDER_COLOR, border_color_.rgba_i32); break;
                    }
                }
            }

            if(bindless_handle_)
            {
                glMakeTextureHandleNonResidentARB(bindless_handle_);
            }
            bindless_handle_ = glGetTextureSamplerHandleARB(handle_, sampler_->handle());
            glMakeTextureHandleResidentARB(bindless_handle_);
        }
        else
        {
            // No sampler, no bindless handle
            if(bindless_handle_)
            {
                glMakeTextureHandleNonResidentARB(bindless_handle_);
            }

            bindless_handle_ = 0;
            sampler_ = nullptr;
        }
    }

    Sampler *TextureView::sampler() const
    {
        return sampler_;
    }

    GLuint64 TextureView::bindless_handle() const
    {
        return bindless_handle_;
    }
    
    GLuint TextureView::handle() const
    {
        return handle_;
    }

    std::uint8_t TextureView::mip_level() const
    {
        return mip_level_;
    }
        
    CubemapFace TextureView::face() const
    {
        return face_;
    }

    ColorValue TextureView::border_color() const
    {
        return border_color_;
    }

    void TextureView::set_border_color(const ColorValue &border_color)
    {
        border_color_ = border_color;
    }   

    bool TextureView::is_depth() const
    {
        return (is_cubemap_ ? cubemap_->is_depth() : texture_->is_depth());
    }   

    bool TextureView::is_depth_stencil() const
    {
        return (is_cubemap_ ? cubemap_->is_depth_stencil() : texture_->is_depth_stencil());
    }
        
    bool TextureView::is_stencil() const
    {
        return (is_cubemap_ ? cubemap_->is_stencil() : texture_->is_stencil());
    }

    bool TextureView::multisampled() const
    {
        return (!is_cubemap_) ? texture_->multisampled() : false;
    }

    TextureFormat TextureView::format() const
    {
        return format_;
    }
}