#include "vexel/graphics/gl_debugger.h"

#include <algorithm>
#include <sstream>

namespace vexel
{
    static std::string helper_sampler_params_to_string(GLint param)
    {
        switch(param)
        {
        case GL_NEAREST: return "Nearest"; break;
        case GL_LINEAR: return "Bilinear"; break;
        case GL_NEAREST_MIPMAP_NEAREST: return "Nearest pixel and nearest mipmap"; break;
        case GL_NEAREST_MIPMAP_LINEAR: return "Nearest pixel and linear mipmap"; break;
        case GL_LINEAR_MIPMAP_NEAREST: return "Bilinear pixel and nearest mipmap"; break;
        case GL_LINEAR_MIPMAP_LINEAR: return "Bilinear pixel and linear mipmap"; break;
        case GL_REPEAT: return "Repeat"; break;
        case GL_MIRRORED_REPEAT: return "Mirrored repeat"; break;
        case GL_CLAMP_TO_EDGE: return "Clamp to edge"; break;
        case GL_CLAMP_TO_BORDER: return "Clamp to border"; break;
        case GL_COMPARE_REF_TO_TEXTURE: return "Compare this to the currently bound texture"; break;
        case GL_NONE: return "None"; break;
        case GL_LEQUAL: return "Less than or equal"; break;
        case GL_GEQUAL: return "Greater than or equal"; break;
        case GL_LESS: return "Less than"; break;
        case GL_GREATER: return "Greater than"; break;
        case GL_EQUAL: return "Equal"; break;
        case GL_NOTEQUAL: return "Not equal"; break;
        case GL_ALWAYS: return "Always pass"; break;
        case GL_NEVER: return "Never pass"; break;
        // Brochacho who even uses this
        //case GL_MIRROR_CLAMP_TO_EDG
        default: return ""; break;
        }

        return "";
    }

    static void helper_sampler_print_border_color(const GLSamplerState &state, std::stringstream &strm)
    {
        switch(state.border_color_type)
        {
        case vexel::GLSamplerState::BorderColorType::FLOAT:
            strm << "(" << state.border_color[0] << ','
                        << state.border_color[1] << ','
                        << state.border_color[2] << ','
                        << state.border_color[3] << ")\n";
            break;
        case vexel::GLSamplerState::BorderColorType::SIGNED_INT_NORMALIZED:
            strm << "(" << state.border_color[0] << ','
                        << state.border_color[1] << ','
                        << state.border_color[2] << ','
                        << state.border_color[3] << ")  (Before signed normalization, that is conversion to [-1, 1], because the format of the texture used is unknown)\n";
            break;
        case vexel::GLSamplerState::BorderColorType::INT:
            strm << "(" << state.border_color_int[0] << ','
                        << state.border_color_int[1] << ','
                        << state.border_color_int[2] << ','
                        << state.border_color_int[3] << ")\n";
            break;
        case vexel::GLSamplerState::BorderColorType::UINT:
            strm << "(" << state.border_color_uint[0] << ','
                        << state.border_color_uint[1] << ','
                        << state.border_color_uint[2] << ','
                        << state.border_color_uint[3] << ")\n";
            break;
        default: break;
        }
    }

    void gl_debugger_hook_gl_sampler_parameter_f(GLuint sampler, GLenum pname, GLfloat param)
    {
        GLDebugger &debugger = GLDebugger::gl_debugger();

        GLSamplerState &state = *std::find_if(debugger.gl_sampler_states_.begin(), debugger.gl_sampler_states_.end(), [&](auto &state){
            return state.sampler == sampler;
        });

        switch(pname)
        {
        case GL_TEXTURE_MIN_LOD: state.min_lod = param; !state.initialized[6] ? state.initialized[6] = true : bool(0); break;
        case GL_TEXTURE_MAX_LOD: state.max_lod = param; !state.initialized[7] ? state.initialized[7] = true : bool(0); break;
        case GL_TEXTURE_LOD_BIAS: state.lod_bias = param; !state.initialized[8] ? state.initialized[8] = true : bool(0); break;
        default: return;
        }
    }

    void gl_debugger_hook_gl_sampler_parameter_i(GLuint sampler, GLenum pname, GLint param)
    {
        GLDebugger &debugger = GLDebugger::gl_debugger();

        GLSamplerState &state = *std::find_if(debugger.gl_sampler_states_.begin(), debugger.gl_sampler_states_.end(), [&](auto &state){
            return state.sampler == sampler;
        });

        switch(pname)
        {
        case GL_TEXTURE_WRAP_S: state.u_mode = param; !state.initialized[0] ? state.initialized[0] = true : bool(0); break;
        case GL_TEXTURE_WRAP_T: state.v_mode = param; !state.initialized[1] ? state.initialized[1] = true : bool(0); break;
        case GL_TEXTURE_WRAP_R: state.w_mode = param; !state.initialized[2] ? state.initialized[2] = true : bool(0); break;
        case GL_TEXTURE_MIN_FILTER: state.minification_filter = param; !state.initialized[3] ? state.initialized[3] = true : bool(0); break;
        case GL_TEXTURE_MAG_FILTER: state.magnification_filter = param; !state.initialized[4] ? state.initialized[4] = true : bool(0); break;
        case GL_TEXTURE_COMPARE_FUNC: state.compare_func = param; !state.initialized[9] ? state.initialized[9] = true : bool(0); break;
        case GL_TEXTURE_COMPARE_MODE: state.compare_mode = param; !state.initialized[10] ? state.initialized[10] = true : bool(0); break;
        default: return;
        }
    }

    void gl_debugger_hook_gl_sampler_parameter_fv(GLuint sampler, GLenum pname, const GLfloat *params)
    {
        if(pname != GL_TEXTURE_BORDER_COLOR)
        {
            // TODO: Throw an error here!
        }

        GLDebugger &debugger = GLDebugger::gl_debugger();

        GLSamplerState &state = *std::find_if(debugger.gl_sampler_states_.begin(), debugger.gl_sampler_states_.end(), [&](auto &state){
            return state.sampler == sampler;
        });

        state.border_color_type = GLSamplerState::BorderColorType::FLOAT;
        state.border_color[0] = params[0];
        state.border_color[1] = params[1];
        state.border_color[2] = params[2];
        state.border_color[3] = params[3];
        !state.initialized[5] ? state.initialized[5] = true : bool(0);
    }

    void gl_debugger_hook_gl_sampler_parameter_iv(GLuint sampler, GLenum pname, const GLint *params)
    // The fuck is this function even, like for what bro?
    // I guess bro: https://registry.khronos.org/OpenGL/specs/gl/glspec46.core.pdf#page=208
    {
        if(pname != GL_TEXTURE_BORDER_COLOR)
        {
            // TODO: Throw an error here!
        }

        GLDebugger &debugger = GLDebugger::gl_debugger();

        GLSamplerState &state = *std::find_if(debugger.gl_sampler_states_.begin(), debugger.gl_sampler_states_.end(), [&](auto &state){
            return state.sampler == sampler;
        });

        state.border_color_type = GLSamplerState::BorderColorType::SIGNED_INT_NORMALIZED;
        state.border_color_int[0] = params[0];
        state.border_color_int[1] = params[1];
        state.border_color_int[2] = params[2];
        state.border_color_int[3] = params[3];
        !state.initialized[5] ? state.initialized[5] = true : bool(0);
    }

    void gl_debugger_hook_gl_sampler_parameter_Iiv(GLuint sampler, GLenum pname, const GLint *params)
    {
        if(pname != GL_TEXTURE_BORDER_COLOR)
        {
            // TODO: Throw an error here!
        }

        GLDebugger &debugger = GLDebugger::gl_debugger();

        GLSamplerState &state = *std::find_if(debugger.gl_sampler_states_.begin(), debugger.gl_sampler_states_.end(), [&](auto &state){
            return state.sampler == sampler;
        });

        state.border_color_type = GLSamplerState::BorderColorType::INT;
        state.border_color_int[0] = params[0];
        state.border_color_int[1] = params[1];
        state.border_color_int[2] = params[2];
        state.border_color_int[3] = params[3];
        !state.initialized[5] ? state.initialized[5] = true : bool(0);
    }

    void gl_debugger_hook_gl_sampler_parameter_Iuiv(GLuint sampler, GLenum pname, const GLuint *params)
    {
        if(pname != GL_TEXTURE_BORDER_COLOR)
        {
            // TODO: Throw an error here!
        }

        GLDebugger &debugger = GLDebugger::gl_debugger();

        GLSamplerState &state = *std::find_if(debugger.gl_sampler_states_.begin(), debugger.gl_sampler_states_.end(), [&](auto &state){
            return state.sampler == sampler;
        });

        state.border_color_type = GLSamplerState::BorderColorType::UINT;
        state.border_color_uint[0] = params[0];
        state.border_color_uint[1] = params[1];
        state.border_color_uint[2] = params[2];
        state.border_color_uint[3] = params[3];
        !state.initialized[5] ? state.initialized[5] = true : bool(0);
    }

    void gl_debugger_hook_gl_create_samplers(GLsizei, GLuint *)
    {
    }

    void gl_debugger_hook_gl_create_samplers_end(GLsizei n, GLuint *samplers)
    {
        GLDebugger &debugger = GLDebugger::gl_debugger();

        debugger.log_strm_ << "Created " << static_cast<std::uint32_t>(n) << " samplers\n";
        for(auto i = 0u; i < n; i++)
        {
            debugger.log_strm_ << "Sampler" << static_cast<std::uint32_t>(i + debugger.sampler_count_) << ": " << samplers[i] << '\n';
            auto pair = std::make_pair(samplers[i], GLObjectType::SAMPLER);
            debugger.gl_objects_["Sampler" + std::to_string(static_cast<std::uint32_t>(i + debugger.sampler_count_))] = pair;
            debugger.gl_objects_reverse_[pair] = "Sampler" + std::to_string(static_cast<std::uint32_t>(i + debugger.sampler_count_));
            debugger.gl_sampler_states_.emplace_back(GLSamplerState{.sampler = samplers[i]});
            debugger.sampler_count_++;
        }

        debugger.log_strm_ << '\n';

        debugger.gl_sampler_states_.reserve(n);
    }

    void gl_debugger_hook_gl_delete_samplers(GLsizei n, const GLuint *samplers)
    {
        GLDebugger &debugger = GLDebugger::gl_debugger();
        debugger.log_strm_ << "Deleted " << n << " samplers\n";
        for(auto i = 0u; i < n; i++)
        {
            auto name = debugger.gl_objects_reverse_[std::make_pair(samplers[i], GLObjectType::SAMPLER)];
            const auto &state = std::find_if(debugger.gl_sampler_states_.cbegin(), debugger.gl_sampler_states_.cend(), [&](const auto &state){
                return state.sampler == samplers[i];
            });
            debugger.log_strm_ << static_cast<std::uint32_t>(i) << ": " << name << '\n';
            debugger.log_strm_ << name << " state:\n"
                               << "U mode: " << (state->initialized[0] ? helper_sampler_params_to_string(state->u_mode) : "(Uninitialized)") << '\n'
                               << "V mode: " << (state->initialized[1] ? helper_sampler_params_to_string(state->v_mode) : "(Uninitialized)") << '\n'
                               << "W mode: " << (state->initialized[2] ? helper_sampler_params_to_string(state->w_mode) : "(Uninitialized)") << '\n'
                               << "Minification filter: " << (state->initialized[3] ? helper_sampler_params_to_string(state->minification_filter) : "(Uninitialized)") << '\n'
                               << "Magnification filter: " << (state->initialized[4] ? helper_sampler_params_to_string(state->magnification_filter) : "(Uninitialized)") << '\n'
                               << "Border color: ";
                               (state->initialized[5] ? helper_sampler_print_border_color(*state, debugger.log_strm_)  : (void)(debugger.log_strm_ << "(0.0, 0.0, 0.0, 0.0)  (Uninitialized)\n")) /** Careful with compiler optimizations */;
                               if(state->initialized[6]) { debugger.log_strm_ << "Min LOD: " << state->min_lod << '\n'; } else { debugger.log_strm_ << "Min LOD: (Uninitialized)\n"; }
                               if(state->initialized[7]) { debugger.log_strm_ << "Max LOD: " << state->max_lod << '\n'; } else { debugger.log_strm_ << "Max LOD: (Uninitialized)\n"; }
                               if(state->initialized[8]) { debugger.log_strm_ << "LOD bias: " << state->lod_bias << '\n'; } else { debugger.log_strm_ << "LOD bias: (Uninitialized)\n"; }
            debugger.log_strm_ << "Comparison mode for depth texture format: " << (state->initialized[10] ? helper_sampler_params_to_string(state->compare_mode) : "(Uninitialized)") << '\n'
                               << "Comparison test: " << (state->initialized[9] ? helper_sampler_params_to_string(state->compare_func) : "(Uninitialized)") << "\n\n";
        }

        debugger.sampler_count_ -= n;

        debugger.log_strm_ << '\n';
    }

}
