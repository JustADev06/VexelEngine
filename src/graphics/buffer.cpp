#include "vexel/graphics/buffer.h"

namespace vexel
{
    Buffer::Buffer(std::size_t size)
        : handle_(0)
        , size_(size)
        , buf_(nullptr)
    {
        glCreateBuffers(1, &handle_);
        glNamedBufferStorage(handle_, size_, nullptr, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT);
        buf_ = glMapNamedBufferRange(handle_, 0, size_, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT);
        
        std::memset(buf_, 0, size_); // Zero out
    }

    Buffer::~Buffer()
    {
        glUnmapNamedBuffer(handle_);
        glDeleteBuffers(1, &handle_);
    }

    GLuint Buffer::handle() const
    {
        return handle_;
    }
}