#pragma once

#include <cstdint>

#include "vexel/graphics/render_pass.h"

namespace vexel
{
    enum class CommandType : std::uint8_t
    {
        BEGIN_RENDER_PASS,
        END_RENDER_PASS
    };

    // You call different functions provided by CommandBuffer class, which itself records them, and then by CommandBuffer::execute you can execute them repeatedly
    class CommandBuffer
    {
    public:
        CommandBuffer();

        void begin_render_pass(RenderPass *pass);

        void end_render_pass();

    private:
        RenderPass *current_pass_;

        GLuint scratch_framebuffer_handle_;
    };
}