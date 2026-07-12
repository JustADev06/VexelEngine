#include "vexel/graphics/sampler.h"

#include <iostream>

namespace vexel
{
    inline static GLint to_opengl_mode(vexel::WrappingMode mode)
    {
        switch(mode)
        {
        case vexel::WrappingMode::REPEAT: return GL_REPEAT; break;
        case vexel::WrappingMode::CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER; break;
        case vexel::WrappingMode::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE; break;
        case vexel::WrappingMode::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT; break;
        default: return GL_NONE; break;
        }

        return GL_NONE;
    }

    inline static GLint to_opengl_filter(vexel::FilteringMode mode)
    {
        switch(mode)
        {
        case vexel::FilteringMode::NEAREST: return GL_NEAREST; break;
        case vexel::FilteringMode::BILINEAR: return GL_LINEAR; break;
        default: return GL_NONE; break;
        }

        return GL_NONE;
    }

    inline static GLint pick_min_filter(vexel::FilteringMode mip_mode, vexel::FilteringMode texel_mode)
    {
        switch(texel_mode)
        {
        case vexel::FilteringMode::NEAREST:
            switch(mip_mode)
            {
            case vexel::FilteringMode::NEAREST: return GL_NEAREST_MIPMAP_NEAREST; break;
            case vexel::FilteringMode::BILINEAR: return GL_NEAREST_MIPMAP_LINEAR; break;
            default: return GL_NONE; break;
            }
            break;
        case vexel::FilteringMode::BILINEAR:
            switch(mip_mode)
            {
            case vexel::FilteringMode::NEAREST: return GL_LINEAR_MIPMAP_NEAREST; break;
            case vexel::FilteringMode::BILINEAR: return GL_LINEAR_MIPMAP_LINEAR; break;
            default: return GL_NONE; break;
            }
            break;
        default: return GL_NONE; break;
        }

        return GL_NONE;
    }

    inline static vexel::WrappingMode to_engine_mode(GLint mode)
    {
        switch(mode)
        {
        case GL_REPEAT: return vexel::WrappingMode::REPEAT; break;
        case GL_CLAMP_TO_EDGE: return vexel::WrappingMode::CLAMP_TO_EDGE; break;
        case GL_CLAMP_TO_BORDER: return vexel::WrappingMode::CLAMP_TO_BORDER; break;
        case GL_MIRRORED_REPEAT: return vexel::WrappingMode::MIRRORED_REPEAT; break;
        }

        return vexel::WrappingMode::REPEAT; // No, non-existent wrapping enum
    }

    inline static vexel::FilteringMode to_engine_filter(GLint mode)
    {
        switch(mode)
        {
        case GL_NEAREST: return vexel::FilteringMode::NEAREST; break;
        case GL_LINEAR: return vexel::FilteringMode::BILINEAR; break;
        }

        return vexel::FilteringMode::NEAREST; // No, non-existent filtering enum
    }

    Sampler::Sampler(const SamplerDescriptor &desc)
        : handle_(0)
    {
        glCreateSamplers(1, &handle_);

        glSamplerParameteri(handle_, GL_TEXTURE_WRAP_S, to_opengl_mode(desc.u_mode));
        glSamplerParameteri(handle_, GL_TEXTURE_WRAP_T, to_opengl_mode(desc.v_mode));
        glSamplerParameteri(handle_, GL_TEXTURE_WRAP_R, to_opengl_mode(desc.w_mode));
        glSamplerParameteri(handle_, GL_TEXTURE_MAG_FILTER, to_opengl_filter(desc.magnification_filter));
        glSamplerParameteri(handle_, GL_TEXTURE_MIN_FILTER, desc.use_mipmaps ? pick_min_filter(desc.mipmap_filter, desc.minification_filter) : to_opengl_filter(desc.minification_filter));
    }

    Sampler::~Sampler()
    {
        glDeleteSamplers(1, &handle_);
    }

    GLuint Sampler::handle() const
    {
        return handle_;
    }

    // FIXME: Just the save the description and reuse, because this will get expensive
    WrappingMode Sampler::u_mode() const
    {
        GLint mode = 0;
        glGetSamplerParameteriv(handle_, GL_TEXTURE_WRAP_S, &mode);
        return to_engine_mode(mode);
    }
        
    WrappingMode Sampler::v_mode() const
    {
        GLint mode = 0;
        glGetSamplerParameteriv(handle_, GL_TEXTURE_WRAP_T, &mode);
        return to_engine_mode(mode);
    }
        
    WrappingMode Sampler::w_mode() const
    {
        GLint mode = 0;
        glGetSamplerParameteriv(handle_, GL_TEXTURE_WRAP_R, &mode);
        return to_engine_mode(mode);
    }
        
    FilteringMode Sampler::magnification_filter() const
    {
        GLint mode = 0;
        glGetSamplerParameteriv(handle_, GL_TEXTURE_MAG_FILTER, &mode);
        return to_engine_filter(mode);
    }
        
    FilteringMode Sampler::minification_filter() const
    {
        GLint mode = 0;
        glGetSamplerParameteriv(handle_, GL_TEXTURE_MIN_FILTER, &mode);
        switch(mode)
        {
        case GL_NEAREST_MIPMAP_NEAREST:
        case GL_NEAREST_MIPMAP_LINEAR: return vexel::FilteringMode::NEAREST; break;
        case GL_LINEAR_MIPMAP_NEAREST:
        case GL_LINEAR_MIPMAP_LINEAR: return vexel::FilteringMode::BILINEAR; break;
        case GL_NEAREST: return vexel::FilteringMode::NEAREST; break;
        case GL_LINEAR: return vexel::FilteringMode::BILINEAR; break;
        }

        return vexel::FilteringMode::NEAREST;
    }
        
    FilteringMode Sampler::mipmap_filter() const
    {
        GLint mode = 0;
        glGetSamplerParameteriv(handle_, GL_TEXTURE_MIN_FILTER, &mode);
        switch(mode)
        {
        case GL_NEAREST_MIPMAP_NEAREST:
        case GL_LINEAR_MIPMAP_NEAREST: return vexel::FilteringMode::NEAREST; break;
        case GL_NEAREST_MIPMAP_LINEAR: 
        case GL_LINEAR_MIPMAP_LINEAR: return vexel::FilteringMode::BILINEAR; break;
        case GL_LINEAR:
        case GL_NEAREST: break;
        }

        return vexel::FilteringMode::NEAREST;
    }

    // FIXME: This is used quite often in texture initialization, maybe reimplement to be independent of opengl's api, store whether they're used yourself
    bool Sampler::uses_mipmaps() const
    {
        GLint mode = 0;
        glGetSamplerParameteriv(handle_, GL_TEXTURE_MIN_FILTER, &mode);
        return !((mode == GL_NEAREST) || (mode == GL_LINEAR));
    }
}