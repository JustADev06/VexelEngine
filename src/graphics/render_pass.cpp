#include "vexel/graphics/render_pass.h"

namespace vexel
{
    RenderPass::RenderPass(std::uint32_t width, std::uint32_t height)
        : handle_(0)
        , width_(width)
        , height_(height)
        , color_attachments_()
        , depth_stencil_attachment_()
        , use_stencil_(false)
    {
        glCreateFramebuffers(1, &handle_);   
    }

    RenderPass::~RenderPass()
    {
        glDeleteFramebuffers(1, &handle_);
    }

    RenderPass::RenderPass(const RenderPassAttachment &attachment)
        : RenderPass(attachment.attachment->width(), attachment.attachment->height())
    {
        if(attachment.type == RenderPassAttachmentType::INPUT) // We just expect a target texture provided here, so we have anything to render to
        {
            // TODO: Throw an error
        }

        bool depth = attachment.attachment->is_depth();
        if(attachment.resolve_attachment)
        {
            if(attachment.resolve_attachment->multisampled())
            {
                // TODO: Throw an error here
            }

            if((attachment.resolve_attachment->width() != attachment.attachment->width()) ||
               (attachment.resolve_attachment->height() != attachment.attachment->height()))
            {
                // TODO: Throw an error here
            }

            if(attachment.resolve)
            {
                if(!attachment.attachment->multisampled())
                {
                    // TODO: Throw an error here
                }
                else
                {
                    if(depth) // You cannot resolve depth (or stencil) textures
                    {
                        // TODO: Throw an error here
                    }
                }
            }
        }

        if(depth)
        {
            if((attachment.type != RenderPassAttachmentType::DEPTH) || (attachment.type != RenderPassAttachmentType::DEPTH_STENCIL))
            {
                // TODO: Throw an error here
            }

            depth_stencil_attachment_ = attachment; 
            use_stencil_ = attachment.attachment->is_depth_stencil();
            glNamedFramebufferTexture(handle_, use_stencil_ ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT, depth_stencil_attachment_.attachment->handle(), 0); /** Mip level is already specified in the view */
        }
        else
        {
            if(attachment.type != RenderPassAttachmentType::COLOR)
            {
                // TODO: Throw an error here
            }
            
            color_attachments_[0] = attachment;
            glNamedFramebufferTexture(handle_, GL_COLOR_ATTACHMENT0, color_attachments_[0].attachment->handle(), 0); /** Mip level is already specified in the view */
        }
    }

    void RenderPass::specify_attachment(const RenderPassAttachment &attachment, std::uint8_t index)
    {
        if(index >= VEXEL_MAX_RENDER_PASS_ATTACHMENTS)
        {
            // TODO: Throw an error here!
        }

        if((attachment.attachment->width() != width_) ||
           (attachment.attachment->height() != height_))
        {
            // TODO: Throw an error here!
        }

        bool depth = attachment.attachment->is_depth();
        if(attachment.resolve_attachment)
        {
            if(attachment.resolve_attachment->multisampled())
            {
                // TODO: Throw an error here
            }

            if(attachment.resolve)
            {
                if(!attachment.attachment->multisampled())
                {
                    // TODO: Throw an error here
                }
                else
                {
                    if(depth) // You cannot resolve depth (or stencil) textures
                    {
                        // TODO: Throw an error here
                    }
                }
            }
        }

        if(depth)
        {
            use_stencil_ = attachment.attachment->is_depth_stencil();
            depth_stencil_attachment_ = attachment;
            glNamedFramebufferTexture(handle_, use_stencil_ ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT, attachment.attachment->handle(), 0);
        }
        else
        {
            color_attachments_[index] = attachment;
            glNamedFramebufferTexture(handle_, GL_COLOR_ATTACHMENT0 + index, attachment.attachment->handle(), 0);
        }
    }

    void RenderPass::remove_attachment(std::uint8_t index)
    {
        if(index >= VEXEL_MAX_RENDER_PASS_ATTACHMENTS)
        {
            // TODO: Throw an error here!
        }

        color_attachments_[index].attachment = nullptr;
        color_attachments_[index].load_op = RenderPassAttachmentOp::DONT_CARE;
        color_attachments_[index].resolve_attachment = nullptr;
        color_attachments_[index].resolve = false;
        color_attachments_[index].type = RenderPassAttachmentType::COLOR;
    
        glNamedFramebufferTexture(handle_, GL_COLOR_ATTACHMENT0 + index, 0, 0);
    }

    void RenderPass::remove_depth_stencil_attachment()
    {
        depth_stencil_attachment_.attachment = nullptr;
        depth_stencil_attachment_.load_op = RenderPassAttachmentOp::DONT_CARE;
        depth_stencil_attachment_.resolve_attachment = nullptr;
        depth_stencil_attachment_.resolve = false;
        depth_stencil_attachment_.type = RenderPassAttachmentType::DEPTH;

        glNamedFramebufferTexture(handle_, use_stencil_ ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT, 0, 0);
        use_stencil_ = false;
    }

    const std::array<RenderPassAttachment, VEXEL_MAX_RENDER_PASS_ATTACHMENTS> &RenderPass::attachments() const
    {
        return color_attachments_;
    }

    const RenderPassAttachment &RenderPass::depth_stencil_attachment() const
    {
        return depth_stencil_attachment_;
    }

    bool RenderPass::use_stencil() const
    {
        return use_stencil_;
    }

    GLuint RenderPass::handle() const
    {
        return handle_;
    }

    std::uint32_t RenderPass::width() const
    {
        return width_;
    }

    std::uint32_t RenderPass::height() const
    {
        return height_;
    }
}