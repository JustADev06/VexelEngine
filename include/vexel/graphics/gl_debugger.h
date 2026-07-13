#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>
#include <sstream>
#include <vector>

#include "glad/gl.h"

namespace vexel
{
    enum class GLObjectType : std::uint8_t
    {
        TEXTURE, // May be a cubemap
        TEXTURE_VIEW,
        SAMPLER,
        FRAMEBUFFER,
        BUFFER,
        VERTEX_ARRAY
    };

    struct GLSamplerState
    {
        enum class BorderColorType : std::uint8_t
        {
            INT,
            SIGNED_INT_NORMALIZED, // Should be passed to the [-1, 1] when the format is known
            UINT,
            FLOAT
        };

        GLuint sampler;
        GLint u_mode; // GL_TEXTURE_WRAP_S
        GLint v_mode; // GL_TEXTURE_WRAP_T
        GLint w_mode; // GL_TEXTURE_WRAP_R
        GLint minification_filter; // GL_TEXTURE_MIN_FILTER
        GLint magnification_filter; // GL_TEXTURE_MAGNIFICATION_FILTER
        union
        {
            GLfloat border_color[4]; // GL_TEXTURE_BORDER_COLOR default: (0.0, 0.0, 0.0, 0.0)
            GLint border_color_int[4];
            GLuint border_color_uint[4];
        };
        BorderColorType border_color_type;
        GLfloat min_lod; // GL_TEXTURE_MIN_LOD default: -1000
        GLfloat max_lod; // GL_TEXTURE_MAX_LOD default: 1000
        GLfloat lod_bias; // GL_TEXTURE_LOD_BIAS default: 0
        GLint compare_func; // GL_TEXTURE_COMPARE_FUNC default: GL_LESS (just as a guess)
        GLint compare_mode; // GL_TEXTURE_COMPARE_MODE default: GL_NONE = 0 (just as a guess)
        bool initialized[11]; // Marks each part of the state whether was set or no
    };

    void gl_debugger_hook_gl_sampler_parameter_f(GLuint sampler, GLenum pname, GLfloat param);
    void gl_debugger_hook_gl_sampler_parameter_i(GLuint sampler, GLenum pname, GLint param);
    void gl_debugger_hook_gl_sampler_parameter_fv(GLuint sampler, GLenum pname, const GLfloat *params);
    void gl_debugger_hook_gl_sampler_parameter_iv(GLuint sampler, GLenum pname, const GLint *params);
    void gl_debugger_hook_gl_sampler_parameter_Iiv(GLuint sampler, GLenum pname, const GLint *params);
    void gl_debugger_hook_gl_sampler_parameter_Iuiv(GLuint sampler, GLenum pname, const GLuint *params);
    void gl_debugger_hook_gl_create_samplers(GLsizei, GLuint *);
    void gl_debugger_hook_gl_create_samplers_end(GLsizei n, GLuint *samplers); // Runs after the actual glCreateSamplers, we need that to capture the create handles
    void gl_debugger_hook_gl_delete_samplers(GLsizei n, const GLuint *samplers);

    struct __PairHash
    {
        template<class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2> &pair) const
        {
            std::size_t hash1 = std::hash<T1>{}(pair.first);
            std::size_t hash2 = std::hash<T2>{}(pair.second);
            return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2)); // Boost like hasher made up on the stop, yeeeah on the spot
        }
    };

    class GLDebugger // A singleton for now
    {
    public:
        static GLDebugger &gl_debugger()
        {
            static GLDebugger debugger{};
            return debugger;
        }

        const std::stringstream &log_strm() const
        {
            return log_strm_;
        }

    private:
        GLDebugger()
            : gl_current_vao_(0)
            , gl_current_program_(0)
            , gl_current_framebuffer_(0)
            , sampler_count_(0)
            , gl_objects_()
            , gl_objects_reverse_()
            , gl_sampler_states_()
            , log_strm_()
        {
        }

        friend void gl_debugger_hook_gl_sampler_parameter_f(GLuint sampler, GLenum pname, GLfloat param);
        friend void gl_debugger_hook_gl_sampler_parameter_i(GLuint sampler, GLenum pname, GLint param);
        friend void gl_debugger_hook_gl_sampler_parameter_fv(GLuint sampler, GLenum pname, const GLfloat *params);
        friend void gl_debugger_hook_gl_sampler_parameter_iv(GLuint sampler, GLenum pname, const GLint *params);
        friend void gl_debugger_hook_gl_sampler_parameter_Iiv(GLuint sampler, GLenum pname, const GLint *params);
        friend void gl_debugger_hook_gl_sampler_parameter_Iuiv(GLuint sampler, GLenum pname, const GLuint *params);
        friend void gl_debugger_hook_gl_create_samplers(GLsizei, GLuint *);
        friend void gl_debugger_hook_gl_create_samplers_end(GLsizei n, GLuint *samplers); // Runs after the actual glCreateSamplers, we need that to capture the create handles
        friend void gl_debugger_hook_gl_delete_samplers(GLsizei n, const GLuint *samplers);

        GLuint gl_current_vao_;
        GLuint gl_current_program_;
        GLuint gl_current_framebuffer_;


        std::unordered_map<std::string, std::pair<GLuint, GLObjectType>> gl_objects_;
        std::unordered_map<std::pair<GLuint, GLObjectType>, std::string, __PairHash> gl_objects_reverse_;

        std::vector<GLSamplerState> gl_sampler_states_;
        std::size_t sampler_count_;

        std::stringstream log_strm_;
    };
}
