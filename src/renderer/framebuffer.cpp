#include "framebuffer.h"
#include "OpenGL_framebuffer.h"
#include "../core/base.h"

#include <memory>

std::shared_ptr<Framebuffer> Framebuffer::Create(
    const FramebufferSpecification &spec ) {
    return CreateRef<OpenGLFramebuffer>( spec );
}
