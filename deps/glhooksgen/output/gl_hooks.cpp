#include <cstdint>
#include <functional>

#include "SDL3/SDL.h"
#include "vexel/graphics/gl_hooks.h"

namespace vexel
{
    static const std::unordered_map<std::string_view, __Proc> hooked_funcs =
    {
        {"glCreateTextures", reinterpret_cast<__Proc>(hook_glCreateTextures)},
        {"glCreateBuffers", reinterpret_cast<__Proc>(hook_glCreateBuffers)}
    };

    inline void hook_glCreateBuffers(GLsizei n, GLuint * buffers)
    {
        static PFNGLCREATEBUFFERSPROC sdl_func = reinterpret_cast<PFNGLCREATEBUFFERSPROC >(SDL_GL_GetProcAddress("glCreateBuffers"));
        std::function<void (GLsizei,GLuint *)> hook = reinterpret_cast<void (*)(GLsizei,GLuint *)>(gl_hooks_hooks.at("glCreateBuffers").first);

        hook(n,buffers);
        sdl_func(n,buffers);
        auto end_hook = gl_hooks_hooks.at("glCreateBuffers").second;
        end_hook ? end_hook() : void(0);
    }

    inline void hook_glCreateTextures(GLenum target, GLsizei n, GLuint * textures)
    {
        static PFNGLCREATETEXTURESPROC sdl_func = reinterpret_cast<PFNGLCREATETEXTURESPROC >(SDL_GL_GetProcAddress("glCreateTextures"));
        std::function<void (GLenum,GLsizei,GLuint *)> hook = reinterpret_cast<void (*)(GLenum,GLsizei,GLuint *)>(gl_hooks_hooks.at("glCreateTextures").first);

        hook(target,n,textures);
        sdl_func(target,n,textures);
        auto end_hook = gl_hooks_hooks.at("glCreateTextures").second;
        end_hook ? end_hook() : void(0);
    }

    __Proc gl_hooks_gl_hook_proc(const char *name)
    {
        if(!gl_hooks_hooks.contains(name))
        {
            return SDL_GL_GetProcAddress(name);
        }
        else
        {
            return hooked_funcs.at(name);
        }
    }
}
