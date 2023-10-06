#pragma once

#include <stdint.h>
#include <memory>

struct FramebufferSpecification {
    uint32_t width, height;
    uint32_t samples = 1;
    bool SwapChainTarget = false;  // glBindFramebuffer(0)
};

class Framebuffer {
   public:
    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual uint32_t GetColorAttachmentRendererID() const = 0;

    virtual const FramebufferSpecification &GetSpecification() const = 0;

    static std::shared_ptr<Framebuffer> Create(
        const FramebufferSpecification &spec );
};