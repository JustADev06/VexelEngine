#pragma once

#include <cstdint>
#include <array>

#include "vexel/graphics/texture_view.h"

namespace vexel
{
    /** The maximum that OpenGL macros support, the color attachments to be exact :( */
    #define VEXEL_MAX_RENDER_PASS_ATTACHMENTS 32u

    /** Operation that has to be done, be it when loading, storing, etc. */
    enum class RenderPassAttachmentOp : std::uint8_t
    {
        CLEAR,
        DONT_CARE
    };

    enum class RenderPassAttachmentType : std::uint8_t
    {
        INPUT = 1, // Means that this color attachment is an input for the pass (typically input for the fragment shader)
        COLOR = 2, // Means that the color attachment output is there
        DEPTH = 4, // Means that the depth attachment output is there
        DEPTH_STENCIL = 8 // Means that the depth and stencil output is there
        /** Sadly we don't offer only stencil, but we could, ohh so bad me :( */ 
    };

    struct RenderPassAttachment
    {
        TextureView *attachment;
        TextureView *resolve_attachment; // = nullptr if not multisampled
        bool resolve; // If to actually resolve after the pass
        RenderPassAttachmentType type;
        RenderPassAttachmentOp load_op;
        ColorValue clear_color;
    };

    /** Vulkan-ish render pass ideology implementation */
    class RenderPass
    {
    public:
        RenderPass(std::uint32_t width, std::uint32_t height);
        RenderPass(const RenderPassAttachment &attachment);

        virtual ~RenderPass();

        VEXEL_DELETE_COPY_MOVE(RenderPass);

        void specify_attachment(const RenderPassAttachment &attachment, std::uint8_t index);

        void remove_attachment(std::uint8_t index);

        void remove_depth_stencil_attachment();

        const std::array<RenderPassAttachment, VEXEL_MAX_RENDER_PASS_ATTACHMENTS> &attachments() const;

        const RenderPassAttachment &depth_stencil_attachment() const;

        bool use_stencil() const;

        GLuint handle() const;

        std::uint32_t width() const;

        std::uint32_t height() const;

    private:
        GLuint handle_;
        std::uint32_t width_;
        std::uint32_t height_;

        std::array<RenderPassAttachment, VEXEL_MAX_RENDER_PASS_ATTACHMENTS> color_attachments_;
        RenderPassAttachment depth_stencil_attachment_;
        bool use_stencil_;
    };
}