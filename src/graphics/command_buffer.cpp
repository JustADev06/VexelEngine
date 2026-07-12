#include "vexel/graphics/command_buffer.h"

namespace vexel
{
    void CommandBuffer::begin_render_pass(RenderPass *pass)
    {
        if(current_pass_)
        {
            // TODO: Throw an error that didn't have the last pass finished before this one!
        }

        current_pass_ = pass;
        glBindFramebuffer(GL_FRAMEBUFFER, current_pass_->handle());
        glViewport(0, 0, current_pass_->width(), current_pass_->height()); // Setup the viewport
        
        auto color_attachments = current_pass_->attachments();

        std::array<GLenum, VEXEL_MAX_RENDER_PASS_ATTACHMENTS> bufs{};

        for(auto i = 0u; i < VEXEL_MAX_RENDER_PASS_ATTACHMENTS; i++)
        {
            auto &color_attachment = color_attachments[i];

            switch(color_attachment.type)
            {
            case RenderPassAttachmentType::COLOR: bufs[i] = GL_COLOR_ATTACHMENT0 + i; break;
            // These don't appear in color attachments
            //case RenderPassAttachmentType::DEPTH: 
            //case RenderPassAttachmentType::DEPTH_STENCIL:
            case RenderPassAttachmentType::INPUT: bufs[i] = GL_NONE; break;
            }

            TextureFormat format = color_attachment.attachment->format();
            switch(color_attachment.load_op)
            {
            case RenderPassAttachmentOp::CLEAR:
                if(static_cast<std::uint8_t>(format) >= static_cast<std::uint8_t>(TextureFormat::SRGB888) /** We're at SRGB or bigger, here it is just case checking */)
                {
                    switch(format)
                    {
                    case TextureFormat::SRGB888:
                    case TextureFormat::SRGBA8888:
                        glEnable(GL_FRAMEBUFFER_SRGB); // Enable the linear -> sRGB space conversion
                        glClearNamedFramebufferfv(current_pass_->handle(), GL_COLOR, i, color_attachment.clear_color.rgba_f32);
                        glDisable(GL_FRAMEBUFFER_SRGB); // Disable it
                        break;
                    // Here we loop over only the color attachments
                    //case TextureFormat::STENCIL8:
                    //case TextureFormat::DEPTH16:
                    //case TextureFormat::DEPTH24:
                    //case TextureFormat::DEPTH32F:
                    //case TextureFormat::DEPTH24_STENCIL8:
                    //case TextureFormat::DEPTH32F_STENCIL8:
                    }
                }
                else
                {
                    switch(static_cast<std::uint8_t>(format) % 10)
                    {
                    case 0:
                    case 1:
                    case 2:
                    case 3: glClearNamedFramebufferfv(current_pass_->handle(), GL_COLOR, i, color_attachment.clear_color.rgba_f32); break;
                    case 4: 
                    case 5:
                    case 6: glClearNamedFramebufferuiv(current_pass_->handle(), GL_COLOR, i, color_attachment.clear_color.rgba_u32); break;
                    case 7: 
                    case 8: 
                    case 9: glClearNamedFramebufferiv(current_pass_->handle(), GL_COLOR, i, color_attachment.clear_color.rgba_i32); break;
                    }
                }
                break;

            case RenderPassAttachmentOp::DONT_CARE: break;
            }
        }

        const auto &depth_stencil_attachment = current_pass_->depth_stencil_attachment();
        if(current_pass_->use_stencil())
        {
            glClearNamedFramebufferfi(current_pass_->handle(), GL_DEPTH_STENCIL, 0, depth_stencil_attachment.clear_color.depth_stencil.depth,
                                                                                    depth_stencil_attachment.clear_color.depth_stencil.stencil);
        }
        else
        {
            glClearNamedFramebufferfv(current_pass_->handle(), GL_DEPTH, 0, &depth_stencil_attachment.clear_color.depth);
        }

        glNamedFramebufferDrawBuffers(current_pass_->handle(), VEXEL_MAX_RENDER_PASS_ATTACHMENTS, bufs.data());
    }

    void CommandBuffer::end_render_pass()
    {
        auto &color_attachments = current_pass_->attachments();
        for(auto i = 0u; i < VEXEL_MAX_RENDER_PASS_ATTACHMENTS; i++)
        {
            if(color_attachments[i].resolve && color_attachments[i].resolve_attachment && color_attachments[i].type == RenderPassAttachmentType::COLOR /** Only accept MSAA render targets, not just arbitrary textures */)
            {
                if((color_attachments[i].attachment->width() != color_attachments[i].resolve_attachment->width()) ||
                   (color_attachments[i].attachment->height() != color_attachments[i].resolve_attachment->height())) // This should've been done before
                {
                    // TODO: Throw an error here!
                }

                GLenum draw_buffer = GL_COLOR_ATTACHMENT0;
                glNamedFramebufferTexture(scratch_framebuffer_handle_, draw_buffer, color_attachments[i].resolve_attachment->handle(), 0);
                glNamedFramebufferDrawBuffers(scratch_framebuffer_handle_, 1, &draw_buffer); // glBlitNamedFramebuffer uses the first entry in the draw buffers array

                glNamedFramebufferReadBuffer(current_pass_->handle(), GL_COLOR_ATTACHMENT0 + i);
                glBlitNamedFramebuffer(current_pass_->handle(), scratch_framebuffer_handle_,
                                       0, 0, color_attachments[i].attachment->width(), color_attachments[i].attachment->height(),
                                       0, 0, color_attachments[i].resolve_attachment->width(), color_attachments[i].resolve_attachment->height(),
                                       GL_COLOR_BUFFER_BIT, GL_NEAREST);
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        current_pass_ = nullptr;
    }
}