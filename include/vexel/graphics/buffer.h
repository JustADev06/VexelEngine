#pragma once

#include <vector>
#include <cstring>
#include <limits>

#include "vexel/utils.h"
#include "vexel/graphics/utils.h"
#include "glad/gl.h"

namespace vexel
{
    /** A constantly sized buffer, typically referred to as "static" */
    class Buffer
    {
    public:
        template<class T>
        Buffer(const std::vector<T> &data, std::size_t size = std::numeric_limits<std::size_t>::max())
            : Buffer(size == std::numeric_limits<std::size_t>::max() ? size * sizeof(T) : size)
        {
            write(data);
        }

        template<class T, std::size_t N>
        Buffer(const std::array<T, N> &data, std::size_t size = std::numeric_limits<std::size_t>::max())
            : Buffer(size == std::numeric_limits<std::size_t>::max() ? sizeof(T) * N : size)
        {
            write(data);
        }

        Buffer(std::size_t size);

        VEXEL_DELETE_COPY_MOVE(Buffer);

        virtual ~Buffer();

        template<class T>
        void write(const T *data, std::size_t size, std::size_t offset)
        {
            std::size_t size_bytes = size * sizeof(T);
            if(offset + size_bytes > size_)
            {   
                VEXEL_THROW(Exception, "The write will overflow, current size " + std::to_string(size_) + ", while data has size " + std::to_string(size_bytes) + ", and is written at the offset " + std::to_string(offset));
            }
        
            std::byte *buffer = reinterpret_cast<std::byte *>(buf_);
            buffer += offset;
            std::memcpy(buffer, data, size_bytes);
            buffer -= offset;
        }

        template<class T, std::size_t N>
        void write(const std::array<T, N> &data, std::size_t offset)
        {
            write(data.data(), N, offset);
        }

        template<class T>
        void write(const std::vector<T> &data, std::size_t offset)
        {
            write(data.data(), data.size(), offset);
        }

        template<class T>
        void write(const T &data, std::size_t offset)
        {
            write(std::addressof(data) /** What if operator& is overloaded? */, offset);
        }

        GLuint handle() const;

    private:
        GLuint handle_;

        std::size_t size_;

        void *buf_;
    };
}
