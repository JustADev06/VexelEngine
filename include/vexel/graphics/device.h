#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <SDL3/SDL.h>

namespace vexel
{
    /**
     *  A vulkan like model, the device represents here both the physical device and the logical device, that the opengl context uses
     *  it's a very rough model as you can't pick devices, but you can query the data about the one that you already use, so it works,
     *  partially
    */

    struct DeviceProperties
    {
        std::uint16_t api_version; // First byte: Minor version, Second byte: Major version
        std::string vendor_name;
        std::string renderer_name; // glGetString(GL_RENDERER); This is the vendor's renderer
        std::string shading_language_version; // glGetString(GL_SHADING_LANGUAGE_VERSION);

        std::uint32_t max_color_texture_samples;
        std::uint32_t max_depth_texture_samples;
        std::uint32_t max_depth_stencil_texture_samples; // Same as max_depth_texture_samples
        std::uint32_t max_integer_texture_samples;
        
        std::uint32_t max_texture_size; 
        std::uint32_t max_cubemap_size;
        
        std::uint32_t max_vertex_attributes;
        std::uint32_t max_vertex_attribute_bindings;
        // A component is just a scalar value, say float, uint, int, bool, are all considered as 1 in the component count, thus vectors like vecN takes up N components, etc.
        // Refer to: https://docs.gl/gl4/glUniform, for more info
        
        std::uint32_t max_vertex_uniforms_components; 
        std::uint32_t max_fragment_uniforms_components;
        std::uint32_t max_vertex_output_components;
        std::uint32_t max_fragment_input_components;

        std::uint32_t max_uniform_blocks; // Maximum number of uniform blocks per program
        std::uint32_t max_uniform_block_size; // The GLSL block
        std::uint32_t max_uniform_buffer_bindings;
        std::uint32_t max_shader_storage_blocks;
        std::uint64_t max_shader_storage_block_size; // The GLSL block
        std::uint32_t max_shader_storage_buffer_bindings;
        
        std::uint32_t max_color_attachments; // How many color attachments can be bound to a framebuffer
        std::uint32_t max_bindable_color_attachments; // How many color attachments can be bound for drawing simultaneously
        
        std::uint32_t max_viewports;
        std::uint32_t max_viewport_dimensions[2];
        
        std::vector<std::string> supported_extensions;
    };

    class Device
    {
    public:
        Device();
        virtual ~Device();
        
        const DeviceProperties &properties() const;

        std::uint16_t api_version() const;

        std::string vendor() const;

        std::string renderer() const;

        std::string shading_language_version() const;
    private:
        DeviceProperties properties_;
    };
}