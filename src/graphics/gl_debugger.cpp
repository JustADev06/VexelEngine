#include "vexel/graphics/gl_debugger.h"

namespace vexel
{
    void gl_debugger_hook_gl_sampler_parameter_f(GLuint sampler, GLenum pname, GLfloat param)
    {
        
    }

    void gl_debugger_hook_gl_sampler_parameter_i(GLuint sampler, GLenum pname, GLint param)
    {

    }
    
    void gl_debugger_hook_gl_sampler_parameter_fv(GLuint sampler, GLenum pname, const GLfloat *params)
    {

    }

    void gl_debugger_hook_gl_sampler_parameter_iv(GLuint sampler, GLenum pname, const GLint *params)
    {

    }
    
    void gl_debugger_hook_gl_sampler_parameter_Iiv(GLuint sampler, GLenum pname, const GLint *params)
    {

    }

    void gl_debugger_hook_gl_sampler_parameter_Iuiv(GLuint sampler, GLenum pname, const GLuint *params)
    {

    }
    
    void gl_debugger_hook_gl_create_samplers(GLsizei, GLuint *)
    {
    }
        
    void gl_debugger_hook_gl_create_samplers_end(GLsizei n, GLuint *samplers)
    {
        auto &debugger = GLDebugger::gl_debugger();
        debugger.log_strm_ << "Created " << static_cast<std::uint32_t>(n) << " samplers\n";
        for(auto i = 0u; i < n; i++)
        {
            debugger.log_strm_ << "Sampler" << static_cast<std::uint32_t>(i) << ": " << samplers[i] << '\n';
            auto pair = std::make_pair(samplers[i], GLObjectType::SAMPLER);
            debugger.gl_objects_["Sampler" + std::to_string(static_cast<std::uint32_t>(i))] = pair;
            debugger.gl_objects_reverse_[pair] = "Sampler" + std::to_string(static_cast<std::uint32_t>(i));
        }
        
        debugger.log_strm_ << '\n';

        debugger.gl_sampler_states_.reserve(n);
    }
    
    void gl_debugger_hook_gl_delete_samplers(GLsizei n, const GLuint *samplers)
    {
        auto &debugger = GLDebugger::gl_debugger();
        debugger.log_strm_ << "Deleted " << n << " samplers\n";
        for(auto i = 0u; i < n; i++)
        {
            debugger.log_strm_ << static_cast<std::uint32_t>(i) << ": " << debugger.gl_objects_reverse_[std::make_pair(samplers[i], GLObjectType::SAMPLER)] << '\n';
        }

        debugger.log_strm_ << '\n';
    }

}