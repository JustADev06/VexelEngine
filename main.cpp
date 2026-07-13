#include <iostream>
#include <sstream>
#include <cstring>
#include <SDL3/SDL.h>
#include "glad/gl.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "vexel/graphics/vertex_attributes.h"
#include "vexel/graphics/buffer.h"
#include "vexel/graphics/texture.h"
#include "vexel/graphics/cubemap.h"
#include "vexel/graphics/sampler.h"
#include "vexel/graphics/texture_view.h"
#include "vexel/graphics/gl_debugger.h"
#include "vexel/math/utils.h"
#include "vexel/graphics/device.h"
#include "vexel/graphics/gl_hooks.h"

// Learning stuff:
// sRGB and framebuffer stuff: https://chatgpt.com/share/6a1b475d-4a38-83eb-97ab-a50d77f1f11a
// Multisampling and screen space explanation stuff: https://chatgpt.com/share/6a1b476e-ff74-83eb-9f83-a9c5d158352f

// glMapBufferRange: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glMapBufferRange.xhtml

bool suppress_notes = true;

void my_callback(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *usr)
{
    std::stringstream strm{};

    strm << "OpenGL ";
    switch(severity)
    {
    case GL_DEBUG_SEVERITY_NOTIFICATION: if(suppress_notes) { return; } strm << "Note"; break;
    case GL_DEBUG_SEVERITY_LOW: strm << "Note"; break;
    case GL_DEBUG_SEVERITY_MEDIUM: strm << "Warning!"; break;
    case GL_DEBUG_SEVERITY_HIGH: strm << "Error!"; break;
    default: strm << "No severity"; break;
    }

    strm << "\n";

    switch(src)
    {
    case GL_DEBUG_SOURCE_API: strm << "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: strm << "Window system"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: strm << "Shader compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: strm << "Third party"; break;
    case GL_DEBUG_SOURCE_APPLICATION: strm << "Application"; break;
    case GL_DEBUG_SOURCE_OTHER: strm << "Unknown"; break;
    default: strm << "Unknown"; break;
    }

    strm << " " << id << " ";

    switch(type)
    {
    case GL_DEBUG_TYPE_ERROR: break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: strm << "(Deprecated behaviour)"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: strm << "(Undefined behaviour)"; break;
    case GL_DEBUG_TYPE_PORTABILITY: strm << "(Portability issue)"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: strm << "(Performance concern)"; break;
    case GL_DEBUG_TYPE_MARKER: /** What does this mean? */; break;
    case GL_DEBUG_TYPE_PUSH_GROUP: /** Neither do i know what this means */; break;
    case GL_DEBUG_TYPE_POP_GROUP: /** Nor this */; break;
    case GL_DEBUG_TYPE_OTHER: break;
    default: break;
    }

    strm << '\n' << msg;

    std::cout << strm.str() << '\n';
}

bool CheckShader(GLuint shader, const char* name)
{
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success)
    {
        return true;
    }


    GLint logLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

    std::vector<GLchar> log(logLength);
    glGetShaderInfoLog(shader, logLength, nullptr, log.data());

    std::cerr << "Shader compilation failed: " << name << '\n';
    std::cerr << log.data() << '\n';

    return false;
}

bool CheckProgram(GLuint program)
{
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success)
    {
        return true;
    }

    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if(success)
    {
        return true;
    }

    GLint logLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

    std::vector<GLchar> log(logLength);
    glGetProgramInfoLog(program, logLength, nullptr, log.data());

    std::cerr << "Program linking failed:\n";
    std::cerr << log.data() << '\n';

    return false;
}

void gl_create_textures_hook(GLenum target, GLsizei n, GLuint *textures)
{
    std::cout << "glCreateTextures was called!" << '\n';
}

void gl_create_textures_hook_end()
{
    std::cout << "glCreateTextures hook ended!" << '\n';
}

void gl_create_buffers_hook(GLsizei n, GLuint *buffers)
{
    std::cout << "glCreateBuffers was called!" << '\n';
}

int main()
{
    bool running = true;

    SDL_Window *window = SDL_CreateWindow("Hello, pizda!", 1024, 768, SDL_WINDOW_OPENGL);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);
    //vexel::GLDebugger gl_debugger{};
    vexel::gl_hooks_set_hook_callback("glCreateTextures", gl_create_textures_hook, gl_create_textures_hook_end);
    vexel::gl_hooks_set_hook_callback("glCreateBuffers", gl_create_buffers_hook, (void*)0);
    vexel::gl_hooks_set_hook_callback("glCreateSamplers", vexel::gl_debugger_hook_gl_create_samplers, vexel::gl_debugger_hook_gl_create_samplers_end);
    vexel::gl_hooks_set_hook_callback("glDeleteSamplers", vexel::gl_debugger_hook_gl_delete_samplers, (void*)0);
    vexel::gl_hooks_set_hook_callback("glSamplerParameterf", vexel::gl_debugger_hook_gl_sampler_parameter_f, (void*)0);
    vexel::gl_hooks_set_hook_callback("glSamplerParameteri", vexel::gl_debugger_hook_gl_sampler_parameter_i, (void*)0);
    vexel::gl_hooks_set_hook_callback("glSamplerParameterfv", vexel::gl_debugger_hook_gl_sampler_parameter_fv, (void*)0);
    vexel::gl_hooks_set_hook_callback("glSamplerParameteriv", vexel::gl_debugger_hook_gl_sampler_parameter_iv, (void*)0);
    vexel::gl_hooks_set_hook_callback("glSamplerParameterIiv", vexel::gl_debugger_hook_gl_sampler_parameter_Iiv, (void*)0);
    vexel::gl_hooks_set_hook_callback("glSamplerParameterIuiv", vexel::gl_debugger_hook_gl_sampler_parameter_Iuiv, (void*)0);
    auto version = gladLoadGL(vexel::gl_hooks_gl_hook_proc);
    if(!version)
    {
        running = false;
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);

    glDebugMessageCallback(my_callback, nullptr);

    struct Vertex
    {
        float x;
        float y;
        float z;

        float r;
        float g;
        float b;

        float u;
        float v;
    };

    glEnable(GL_DEPTH_TEST); // This was disabled by default, the fuck?!

    Vertex vertices[] = {
        Vertex{-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
        Vertex{0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
        Vertex{0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
        Vertex{-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
        Vertex{-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
        Vertex{0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
        Vertex{0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
        Vertex{-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f}
    };

    uint32_t indices[] = {
        0, 1, 2, 2, 3, 0, 4, 6, 5, 6, 4, 7, 0, 3, 7, 7, 4, 0, 1, 5, 6, 6, 2, 1, 0, 4, 5, 5, 1, 0, 3, 2, 6, 6, 7, 3
    };

    vexel::Device dev{};

    vexel::Buffer vbo(sizeof(vertices));
    vexel::Buffer ebo(sizeof(indices));

    vbo.write(vertices, VEXEL_SIZEOF_C_ARRAY(vertices), 0);
    ebo.write(indices, VEXEL_SIZEOF_C_ARRAY(indices), 0);

    vexel::SingleStreamVertexAttributes vao{vexel::VertexAttributeType::FLOAT32_3, vexel::VertexAttributeType::FLOAT32_3, vexel::VertexAttributeType::FLOAT32_2};
    vao.bind_vertex_stream(vbo.handle(), 0);
    vao.bind_element_buffer(ebo.handle());

    ::stbi_set_flip_vertically_on_load(true);

    std::int32_t width_tex = 0;
    std::int32_t height_tex = 0;
    std::uint8_t *data = ::stbi_load("./../grass.jpg", &width_tex, &height_tex, nullptr, 3);

    if(!data || (width_tex == 0) || (height_tex == 0))
    {
        std::cout << "STB error: " << stbi_failure_reason() << '\n';
    }

    vexel::SamplerDescriptor sampler_desc = vexel::SamplerDescriptor{.u_mode = vexel::WrappingMode::REPEAT,
                             .v_mode = vexel::WrappingMode::REPEAT,
                             .w_mode = vexel::WrappingMode::REPEAT,
                             .magnification_filter = vexel::FilteringMode::BILINEAR,
                             .minification_filter = vexel::FilteringMode::BILINEAR,
                             .mipmap_filter = vexel::FilteringMode::BILINEAR,
                             .use_mipmaps = true};
    vexel::Sampler my_sampler(sampler_desc);
    vexel::Texture my_texture(static_cast<std::uint32_t>(width_tex), static_cast<std::uint32_t>(height_tex), vexel::TextureFormat::RGB888_UNORM, true);
    vexel::TextureView my_texture_whole_view(&my_texture, &my_sampler);
    my_texture.specify_image(reinterpret_cast<std::byte *>(data));

    std::int32_t width_skybox = 0;
    std::int32_t height_skybox = 0;

    ::stbi_set_flip_vertically_on_load(false);

    std::uint8_t *data0 = ::stbi_load("./../skybox/1.jpg", &width_skybox, &height_skybox, nullptr, 3);
    std::uint8_t *data1 = ::stbi_load("./../skybox/2.jpg", &width_skybox, &height_skybox, nullptr, 3);
    std::uint8_t *data2 = ::stbi_load("./../skybox/3.jpg", &width_skybox, &height_skybox, nullptr, 3);
    std::uint8_t *data3 = ::stbi_load("./../skybox/4.jpg", &width_skybox, &height_skybox, nullptr, 3);
    std::uint8_t *data4 = ::stbi_load("./../skybox/5.jpg", &width_skybox, &height_skybox, nullptr, 3);
    std::uint8_t *data5 = ::stbi_load("./../skybox/6.jpg", &width_skybox, &height_skybox, nullptr, 3);


    std::array<std::byte *, 6u> data_skybox = {reinterpret_cast<std::byte *>(data0),
                                               reinterpret_cast<std::byte *>(data1),
                                               reinterpret_cast<std::byte *>(data2),
                                               reinterpret_cast<std::byte *>(data3),
                                               reinterpret_cast<std::byte *>(data4),
                                               reinterpret_cast<std::byte *>(data5)};

    vexel::SamplerDescriptor skybox_sampler_desc = vexel::SamplerDescriptor{.u_mode = vexel::WrappingMode::CLAMP_TO_EDGE,
                             .v_mode = vexel::WrappingMode::CLAMP_TO_EDGE,
                             .w_mode = vexel::WrappingMode::CLAMP_TO_EDGE,
                             .magnification_filter = vexel::FilteringMode::BILINEAR,
                             .minification_filter = vexel::FilteringMode::BILINEAR,
                             .mipmap_filter = vexel::FilteringMode::BILINEAR,
                             .use_mipmaps = false};
    vexel::Sampler skybox_sampler(skybox_sampler_desc);
    vexel::Cubemap my_skybox(static_cast<std::uint32_t>(width_skybox), static_cast<std::uint32_t>(height_skybox), vexel::TextureFormat::RGB888_UNORM, false);
    vexel::TextureView my_skybox_view(&my_skybox, &skybox_sampler);
    my_skybox.specify_image(vexel::CubemapFace::POSITIVE_X, data_skybox[0]);
    my_skybox.specify_image(vexel::CubemapFace::NEGATIVE_X, data_skybox[1]);
    my_skybox.specify_image(vexel::CubemapFace::POSITIVE_Y, data_skybox[2]);
    my_skybox.specify_image(vexel::CubemapFace::NEGATIVE_Y, data_skybox[3]);
    my_skybox.specify_image(vexel::CubemapFace::POSITIVE_Z, data_skybox[4]);
    my_skybox.specify_image(vexel::CubemapFace::NEGATIVE_Z, data_skybox[5]);

    float skybox_vertices[] = {
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f
    };

    std::uint32_t skybox_indices[] = {

        // Front face (+Z)
        0, 1, 2,
        2, 1, 3,

        // Back face (-Z)
        5, 4, 7,
        7, 4, 6,

        // Left face (-X)
        4, 0, 6,
        6, 0, 2,

        // Right face (+X)
        1, 5, 3,
        3, 5, 7,

        // Top face (+Y)
        2, 3, 6,
        6, 3, 7,

        // Bottom face (-Y)
        4, 5, 0,
        0, 5, 1
    };

    vexel::Buffer skybox_vbo(sizeof(skybox_vertices));
    vexel::Buffer skybox_ebo(sizeof(skybox_indices));

    skybox_vbo.write(skybox_vertices, VEXEL_SIZEOF_C_ARRAY(skybox_vertices), 0);
    skybox_ebo.write(skybox_indices, VEXEL_SIZEOF_C_ARRAY(skybox_indices), 0);

    vexel::SingleStreamVertexAttributes skybox_vao{vexel::VertexAttributeType::FLOAT32_3};
    skybox_vao.bind_vertex_stream(skybox_vbo.handle(), 0);
    skybox_vao.bind_element_buffer(skybox_ebo.handle());

    const char *skybox_vs_src = R"(
        #version 460 core
        #extension GL_ARB_bindless_texture : require
        layout (location = 0) in vec3 pos;

        out vec3 world_pos;
        uniform mat4 proj;
        uniform mat4 view;

        void main()
        {
            world_pos = pos;
            vec4 clip_pos = proj * mat4(mat3(view)) * vec4(pos * 10.0, 1.0);
            gl_Position = clip_pos.xyww; // on top of everything
        }

    )";

    const char *skybox_fs_src = R"(
        #version 460 core
        #extension GL_ARB_bindless_texture : require

        in vec3 world_pos;
        layout(bindless_sampler) uniform samplerCube skybox;
        out vec4 fragColor;

        void main()
        {
            vec3 dir = normalize(world_pos);
            fragColor = textureLod(skybox, dir, 0.0);
        }
    )";


    GLuint skybox_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint skybox_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint skybox_program = glCreateProgram();

    glShaderSource(skybox_vertex_shader, 1, &skybox_vs_src, nullptr);
    glShaderSource(skybox_fragment_shader, 1, &skybox_fs_src, nullptr);

    glCompileShader(skybox_vertex_shader);
    CheckShader(skybox_vertex_shader, "vs");
    glCompileShader(skybox_fragment_shader);
    CheckShader(skybox_fragment_shader, "fs");

    glAttachShader(skybox_program, skybox_vertex_shader);
    glAttachShader(skybox_program, skybox_fragment_shader);

    glLinkProgram(skybox_program);
    glValidateProgram(skybox_program);
    CheckProgram(skybox_program);

    GLint skybox_view_mat_loc = glGetUniformLocation(skybox_program, "view");
    GLint skybox_proj_mat_loc = glGetUniformLocation(skybox_program, "proj");
    GLint skybox_tex_loc = glGetUniformLocation(skybox_program, "skybox");

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint program = glCreateProgram();

    const char *vs_src = R"(
        #version 460 core
        #extension GL_ARB_bindless_texture : require
        layout (location = 0) in vec3 pos;
        layout (location = 1) in vec3 color;
        layout (location = 2) in vec2 uv;

        out vec3 col;
        out vec2 uv_out;
        uniform mat4 proj;
        uniform mat4 view;
        uniform mat4 model;

        void main()
        {
            gl_Position = proj * view * model * vec4(pos, 1.0);
            col = color;
            uv_out = uv;
        }

    )";

    const char *fs_src = R"(
        #version 460 core
        #extension GL_ARB_bindless_texture : require

        in vec3 col;
        in vec2 uv_out;
        layout(bindless_sampler) uniform sampler2D tex;
        out vec4 fragColor;

        void main()
        {
            fragColor = texture(tex, uv_out);
        }
    )";

    glShaderSource(vertex_shader, 1, &vs_src, nullptr);
    glShaderSource(fragment_shader, 1, &fs_src, nullptr);

    glCompileShader(vertex_shader);
    CheckShader(vertex_shader, "vs");
    glCompileShader(fragment_shader);
    CheckShader(fragment_shader, "fs");

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);
    glValidateProgram(program);
    CheckProgram(program);

    GLint view_mat_loc = glGetUniformLocation(program, "view");
    GLint proj_mat_loc = glGetUniformLocation(program, "proj");
    GLint model_mat_loc = glGetUniformLocation(program, "model");
    GLint tex_loc = glGetUniformLocation(program, "tex");

    vexel::Matrix4 m(1.0f, 0.0f, 2.0f, -1.0f,
                     3.0f, 0.0f, 0.0f, 5.0f,
                     2.0f, 1.0f, 4.0f, -3.0f,
                     1.0f, 0.0f, 5.0f, 0.0f);

    std::cout << "The determinant: " << m.determinant() << '\n';
    std::cout << "The adjoint:\n" << m.adjoint() << '\n';
    std::cout << "Inverse:\n" << m.inverse() << '\n';
    std::cout << "Out: " << m * (m.inverse());

    vexel::Matrix4 projection = vexel::fov_aspect_ratio_frustum(static_cast<float>(M_PI_2), 768.0f / 1024.0f, 0.1f, 100.0f);
    vexel::Matrix4 view = vexel::Matrix4();

    vexel::Rotor my_rot(vexel::Vector3(0.0f, 1.0f, 0.0f), M_PI_4);

    vexel::Matrix4 my_mat(1.732f, -0.750f, 0.0f, 5.0f,
                                 1.0f, 1.299f, 0.0f, 3.0f,
                                0.0f, 0.0f, 1.0f, -2.0f,
                                0.0f, 0.0f, 0.0f, 1.0f);

    vexel::Matrix4 other_mat(0.9063f, 0.2717f, 0.3237f, 0.0f,
                                    0.0f, 0.7660f, -0.6428f, 0.0f,
                                    -0.4226f, 0.5826f, 0.6943f, 0.0f,
                                     0.0f, 0.0f, 0.0f, 1.0f);

    vexel::Matrix4 change_of_basis = (0.0f, 1.0f, 0.0f, 0.0f,
                                             1.0f, 0.0f, 0.0f, 0.0f,
                                             0.0f, 0.0f, 1.0f, 0.0f,
                                             0.0f, 0.0f, 0.0f, 1.0f);

    vexel::Matrix4 pizda_other_mat = vexel::Matrix4::transpose(change_of_basis) * other_mat * change_of_basis;

    vexel::Rotor rot_30_z = vexel::Rotor(vexel::Vector3(0.0f, 0.0f, 1.0f), M_PI_2 / 3.0f);
    vexel::Rotor rot_other = vexel::Rotor(0.6981317008f, 0.4363323130f, 0.0f, vexel::EulerAnglesOrder::ZYX);

    auto [translation, scale, rotation] = vexel::Matrix4::decompose(my_mat);
    std::cout << "Translation: " << translation << '\n';
    std::cout << "Scale: " << scale << '\n';
    std::cout << "Rotation: " << rotation << '\n';
    std::cout << "Really rot: " << rot_30_z << '\n';

    auto [translation_other, scale_other, rotation_other] = vexel::Matrix4::decompose(other_mat);
    auto [_, __, pizda_rotatata] = vexel::Matrix4::decompose(pizda_other_mat);
    std::cout << "Translation: " << translation_other << '\n';
    std::cout << "Scale: " << scale_other << '\n';
    std::cout << "Rotation: " << rotation_other.normalize() << '\n';
    std::cout << "Really rot: " << rot_other.normalize() << '\n';
    std::cout << "So really really rot: " << pizda_rotatata << '\n';


    float yaw = 0.0f;
    float pitch = 0.0f;
    vexel::Rotor orientation;
    vexel::Rotor cube_rot1(vexel::Vector3(1.0f, 0.0f, 0.0f), M_PI_2); // cube_rot1 and 2 are actually the same rotation when conjugating, because (-1)v(-1) = (-1)^2 v = v = 1 * v * 1
    vexel::Rotor cube_rot2(vexel::Vector3(0.0f, 1.0f, 0.0f), M_PI_2);
    vexel::Rotor cube_rot;
    float yaw_of_cube = 0.0f;
    vexel::Vector3 view_vector = vexel::Vector3(0.0f, 0.0f, 1.0f);
    vexel::Vector3 camera_up = vexel::Vector3(0.0f, 1.0f, 0.0f);
    vexel::Vector3 camera_right = vexel::Vector3(1.0f, 0.0f, 0.0f);
    vexel::Vector3 pos = vexel::Vector3(0.0f, 0.0f, 0.0f);

    while(running)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_EVENT_QUIT)
            {
                running = false;
                break;
            }
            else if(e.type == SDL_EVENT_KEY_DOWN)
            {
                if(e.key.key == SDLK_ESCAPE)
                {
                    running = false;
                    break;
                }
                else if(e.key.key == SDLK_W)
                {
                    pos += view_vector;
                }
                else if(e.key.key == SDLK_S)
                {
                    pos -= view_vector;
                }
                else if(e.key.key == SDLK_A)
                {
                    pos -= camera_right;
                }
                else if(e.key.key == SDLK_D)
                {
                    pos += camera_right;
                }
                else if(e.key.key == SDLK_SPACE)
                {
                    pos += camera_up;
                }
                else if(e.key.mod & SDL_KMOD_SHIFT)
                {
                    pos -= camera_up;
                }
            }
            else if(e.type == SDL_EVENT_MOUSE_MOTION)
            {
                static const float to_rads = M_PI / 180.0f;
                static const float sensetivity = 0.5f;
                float xrel = -e.motion.xrel; // Because the default is turning counterclockwise, if you subtract from it you're going to end up moving clockwise, which is not what we want, so flip it
                float yrel = -e.motion.yrel; // SDL has flipped convention

                yaw += xrel * sensetivity * to_rads;
                pitch += yrel * sensetivity * to_rads;
            }
        }

            orientation = vexel::Rotor(yaw, pitch, 0.0f, vexel::EulerAnglesOrder::XYZ);
            cube_rot = vexel::Rotor::slerp(cube_rot1, cube_rot2, yaw_of_cube);
            vexel::Matrix4 cube_model = vexel::Matrix4::translate(vexel::Vector3(0.0f, 0.0f, 4.0f)) * vexel::Matrix4::rotate(cube_rot);
            view_vector = orientation.rotate(vexel::Vector3(0.0f, 0.0f, 1.0f));
            camera_up = orientation.rotate(vexel::Vector3(0.0f, 1.0f, 0.0f));
            camera_right = orientation.rotate(vexel::Vector3(1.0f, 0.0f, 0.0f));

            view = vexel::Matrix4(camera_right, camera_up, view_vector);
            view = view.transpose(); // An orthonormal change of basis, has it's inverse just being it's transpose
            view *= view.translate(-pos); // The rightmost entries are not just -pos.x, -pos.y, -pos.z, they're infact the dor products with -pos and the view basis vectors, do
                                          // this multiplication yourself, remember always, that the order of composition for a our transformations, should be translate * rotate * scale
                                          // read from right to left

            yaw_of_cube += 0.01f;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao.handle());
        glUseProgram(program);
        glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, view.data());
        glUniformMatrix4fv(proj_mat_loc, 1, GL_FALSE, projection.data());
        glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, cube_model.data());
        glUniformHandleui64ARB(tex_loc, my_texture_whole_view.bindless_handle());
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

        glDepthFunc(GL_LEQUAL);
        glBindVertexArray(skybox_vao.handle());
        glUseProgram(skybox_program);
        glUniformMatrix4fv(skybox_view_mat_loc, 1, GL_FALSE, view.data());
        glUniformMatrix4fv(skybox_proj_mat_loc, 1, GL_FALSE, projection.data());
        glUniformHandleui64ARB(skybox_tex_loc, my_skybox_view.bindless_handle());
        glDrawElements(GL_TRIANGLES, sizeof(skybox_indices) / sizeof(skybox_indices[0]), GL_UNSIGNED_INT, 0);
        glDepthFunc(GL_LESS);

        SDL_GL_SwapWindow(window);
    }

    // Just to test, yes it works!
    my_sampler.~Sampler();
    skybox_sampler.~Sampler();

    // This won't capture the deletes because the samplers go out of scope later, down below, when the scope of int main() ends to be exact, but i'm sure they would
    // if we were to delete them beforehand
    auto &gl_debugger = vexel::GLDebugger::gl_debugger();
    std::cout << '\n' << gl_debugger.log_strm().str() << '\n';

    return 0;
}
