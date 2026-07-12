#pragma once

#include <string>
#include <unordered_map>

#include "glad/gl.h"

namespace vexel
{
    using __Proc = void (*)();

    inline std::unordered_map<std::string_view, std::pair<__Proc, __Proc>> gl_hooks_hooks;

    template<class Func1, class Func2>
    inline void gl_hooks_set_hook_callback(std::string_view name, Func1 *hook_start, Func2 *hook_end)
    {
        gl_hooks_hooks[name] = std::make_pair(reinterpret_cast<__Proc>(hook_start), reinterpret_cast<__Proc>(hook_end));
    }

    inline void hook_glCreateBuffers(GLsizei n, GLuint * buffers);
    inline void hook_glCreateSamplers(GLsizei n, GLuint * samplers);
    inline void hook_glCreateTextures(GLenum target, GLsizei n, GLuint * textures);
    inline void hook_glDeleteSamplers(GLsizei count, const GLuint * samplers);

    __Proc gl_hooks_gl_hook_proc(const char *name);
}
