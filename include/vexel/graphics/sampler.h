#pragma once

#include "vexel/graphics/utils.h"
#include "vexel/math/utils.h"

namespace vexel
{
    enum class WrappingMode : std::uint8_t
    {
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER,
        MIRRORED_REPEAT,
        REPEAT
    };

    enum class FilteringMode : std::uint8_t
    {
        NEAREST,
        BILINEAR  
    };

    struct SamplerDescriptor
    {
        WrappingMode u_mode;
        WrappingMode v_mode;
        WrappingMode w_mode;
        FilteringMode magnification_filter;
        FilteringMode minification_filter;
        FilteringMode mipmap_filter;
        bool use_mipmaps;
    };

    class Sampler
    {
    public:
        Sampler(const SamplerDescriptor &desc);
        virtual ~Sampler();

        VEXEL_DELETE_COPY_MOVE(Sampler);

        GLuint handle() const;

        // Mostly for debugging purposes than getting info!
        WrappingMode u_mode() const;
        
        WrappingMode v_mode() const;
        
        WrappingMode w_mode() const;
        
        FilteringMode magnification_filter() const;
        
        FilteringMode minification_filter() const;
        
        FilteringMode mipmap_filter() const;
        
        bool uses_mipmaps() const;

    private:
        GLuint handle_;
    };
}