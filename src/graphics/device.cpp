#include "vexel/graphics/device.h"

#include "glad/gl.h"

namespace vexel
{
    Device::Device()
        : properties_()
    {
        properties_.vendor_name = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
        properties_.renderer_name = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
        properties_.shading_language_version = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
        
        GLint minor_version = 0;
        GLint major_version = 0;
        glGetIntegerv(GL_MINOR_VERSION, &minor_version);
        glGetIntegerv(GL_MAJOR_VERSION, &major_version);

        properties_.api_version = static_cast<std::uint16_t>((static_cast<std::uint8_t>(major_version) << 8) | static_cast<std::uint8_t>(minor_version));

        // https://registry.khronos.org/OpenGL-Refpages/es3/html/glGet.xhtml
        glGetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, reinterpret_cast<GLint *>(&properties_.max_color_texture_samples));
        glGetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, reinterpret_cast<GLint *>(&properties_.max_depth_texture_samples));
        properties_.max_depth_stencil_texture_samples = properties_.max_depth_texture_samples; // https://docs.gl/gl4/glGet
        glGetIntegerv(GL_MAX_INTEGER_SAMPLES, reinterpret_cast<GLint *>(&properties_.max_integer_texture_samples));
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, reinterpret_cast<GLint *>(&properties_.max_texture_size));
        glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, reinterpret_cast<GLint *>(&properties_.max_cubemap_size));
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, reinterpret_cast<GLint *>(&properties_.max_vertex_attributes));
        glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, reinterpret_cast<GLint *>(&properties_.max_vertex_attribute_bindings));
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, reinterpret_cast<GLint *>(&properties_.max_vertex_uniforms_components));
        glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, reinterpret_cast<GLint *>(&properties_.max_fragment_uniforms_components));
        glGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, reinterpret_cast<GLint *>(&properties_.max_uniform_blocks));
        glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, reinterpret_cast<GLint *>(&properties_.max_uniform_block_size));
        glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, reinterpret_cast<GLint *>(&properties_.max_uniform_buffer_bindings));
        glGetIntegerv(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, reinterpret_cast<GLint *>(&properties_.max_shader_storage_blocks));
        glGetInteger64v(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, reinterpret_cast<GLint64 *>(&properties_.max_shader_storage_block_size));
        glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, reinterpret_cast<GLint *>(&properties_.max_shader_storage_buffer_bindings));
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, reinterpret_cast<GLint *>(&properties_.max_color_attachments));
        glGetIntegerv(GL_MAX_DRAW_BUFFERS, reinterpret_cast<GLint *>(&properties_.max_bindable_color_attachments));
        glGetIntegerv(GL_MAX_VIEWPORTS, reinterpret_cast<GLint *>(&properties_.max_viewports));
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, reinterpret_cast<GLint *>(properties_.max_viewport_dimensions));

        std::uint32_t extension_count = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, reinterpret_cast<GLint *>(&extension_count));

        for(auto i = 0u; i < extension_count; i++)
        {
            properties_.supported_extensions.push_back(reinterpret_cast<const char *>(glGetStringi(GL_EXTENSIONS, i)));
        }
    }

    Device::~Device()
    {
    }
    
    const DeviceProperties &Device::properties() const
    {
        return properties_;
    }

    std::uint16_t Device::api_version() const
    {
        return properties_.api_version;
    }

    std::string Device::vendor() const
    {
        return properties_.vendor_name;
    }

    std::string Device::renderer() const
    {
        return properties_.renderer_name;
    }

    std::string Device::shading_language_version() const
    {
        return properties_.shading_language_version;
    }
}