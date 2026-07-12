#include <cstdint>
#include <functional>

#include "SDL3/SDL.h"
#include "vexel/graphics/gl_hooks.h"

namespace vexel
{
    static const std::unordered_map<std::string_view, __Proc> hooked_funcs =
    {
        {"glCreateTextures", reinterpret_cast<__Proc>(hook_glCreateTextures)},
        {"glCreateBuffers", reinterpret_cast<__Proc>(hook_glCreateBuffers)},
        {"glCreateSamplers", reinterpret_cast<__Proc>(hook_glCreateSamplers)},
        {"glDeleteSamplers", reinterpret_cast<__Proc>(hook_glDeleteSamplers)}
    };

    inline void hook_glCreateBuffers(GLsizei n, GLuint * buffers)
    {
        static PFNGLCREATEBUFFERSPROC sdl_func = reinterpret_cast<PFNGLCREATEBUFFERSPROC >(SDL_GL_GetProcAddress("glCreateBuffers"));
        std::function<void (GLsizei,GLuint *)> hook = reinterpret_cast<void (*)(GLsizei,GLuint *)>(gl_hooks_hooks.at("glCreateBuffers").first);
        std::function<void (GLsizei,GLuint *)> end_hook = reinterpret_cast<void (*)(GLsizei,GLuint *)>(gl_hooks_hooks.at("glCreateBuffers").second);

        hook(n,buffers);
        sdl_func(n,buffers);
        end_hook ? end_hook(n,buffers) : void(0);
    }

    inline void hook_glCreateSamplers(GLsizei n, GLuint * samplers)
    {
        static PFNGLCREATESAMPLERSPROC sdl_func = reinterpret_cast<PFNGLCREATESAMPLERSPROC >(SDL_GL_GetProcAddress("glCreateSamplers"));
        std::function<void (GLsizei,GLuint *)> hook = reinterpret_cast<void (*)(GLsizei,GLuint *)>(gl_hooks_hooks.at("glCreateSamplers").first);
        std::function<void (GLsizei,GLuint *)> end_hook = reinterpret_cast<void (*)(GLsizei,GLuint *)>(gl_hooks_hooks.at("glCreateSamplers").second);

        hook(n,samplers);
        sdl_func(n,samplers);
        end_hook ? end_hook(n,samplers) : void(0);
    }

    inline void hook_glCreateTextures(GLenum target, GLsizei n, GLuint * textures)
    {
        static PFNGLCREATETEXTURESPROC sdl_func = reinterpret_cast<PFNGLCREATETEXTURESPROC >(SDL_GL_GetProcAddress("glCreateTextures"));
        std::function<void (GLenum,GLsizei,GLuint *)> hook = reinterpret_cast<void (*)(GLenum,GLsizei,GLuint *)>(gl_hooks_hooks.at("glCreateTextures").first);
        std::function<void (GLenum,GLsizei,GLuint *)> end_hook = reinterpret_cast<void (*)(GLenum,GLsizei,GLuint *)>(gl_hooks_hooks.at("glCreateTextures").second);

        hook(target,n,textures);
        sdl_func(target,n,textures);
        end_hook ? end_hook(target,n,textures) : void(0);
    }

    inline void hook_glDeleteSamplers(GLsizei count, const GLuint * samplers)
    {
        static PFNGLDELETESAMPLERSPROC sdl_func = reinterpret_cast<PFNGLDELETESAMPLERSPROC >(SDL_GL_GetProcAddress("glDeleteSamplers"));
        std::function<void (GLsizei,const GLuint *)> hook = reinterpret_cast<void (*)(GLsizei,const GLuint *)>(gl_hooks_hooks.at("glDeleteSamplers").first);
        std::function<void (GLsizei,const GLuint *)> end_hook = reinterpret_cast<void (*)(GLsizei,const GLuint *)>(gl_hooks_hooks.at("glDeleteSamplers").second);

        hook(count,samplers);
        sdl_func(count,samplers);
        end_hook ? end_hook(count,samplers) : void(0);
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
