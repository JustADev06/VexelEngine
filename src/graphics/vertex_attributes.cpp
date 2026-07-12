#include "vexel/graphics/vertex_attributes.h"

namespace vexel
{
    inline static std::size_t sizeof_type(VertexAttributeType type)
    {
    
        switch(type)
        {
        case VertexAttributeType::FLOAT32:
        case VertexAttributeType::FLOAT32_2: 
        case VertexAttributeType::FLOAT32_3: 
        case VertexAttributeType::FLOAT32_4: return sizeof(float); break; 
        case VertexAttributeType::FLOAT64:
        case VertexAttributeType::FLOAT64_2: 
        case VertexAttributeType::FLOAT64_3: 
        case VertexAttributeType::FLOAT64_4: return sizeof(double); break; 
        case VertexAttributeType::UINT8:  
        case VertexAttributeType::UINT8_2:  
        case VertexAttributeType::UINT8_3:  
        case VertexAttributeType::UINT8_4: return sizeof(std::uint8_t); break; 
        case VertexAttributeType::INT8:  
        case VertexAttributeType::INT8_2:  
        case VertexAttributeType::INT8_3:  
        case VertexAttributeType::INT8_4: return sizeof(std::int8_t); break; 
        case VertexAttributeType::UINT16:  
        case VertexAttributeType::UINT16_2:  
        case VertexAttributeType::UINT16_3:  
        case VertexAttributeType::UINT16_4: return sizeof(std::uint16_t); break; 
        case VertexAttributeType::INT16: 
        case VertexAttributeType::INT16_2: 
        case VertexAttributeType::INT16_3: 
        case VertexAttributeType::INT16_4: return sizeof(std::int16_t); break; 
        case VertexAttributeType::UINT32: 
        case VertexAttributeType::UINT32_2: 
        case VertexAttributeType::UINT32_3: 
        case VertexAttributeType::UINT32_4: return sizeof(std::uint32_t); break; 
        case VertexAttributeType::INT32:  
        case VertexAttributeType::INT32_2:  
        case VertexAttributeType::INT32_3:  
        case VertexAttributeType::INT32_4: return sizeof(std::int32_t); break; 
        case VertexAttributeType::UINT8_NORMALIZED: 
        case VertexAttributeType::UINT8_2_NORMALIZED: 
        case VertexAttributeType::UINT8_3_NORMALIZED: 
        case VertexAttributeType::UINT8_4_NORMALIZED: return sizeof(std::uint8_t); break; 
        case VertexAttributeType::INT8_NORMALIZED: 
        case VertexAttributeType::INT8_2_NORMALIZED: 
        case VertexAttributeType::INT8_3_NORMALIZED: 
        case VertexAttributeType::INT8_4_NORMALIZED: return sizeof(std::int8_t); break; 
        case VertexAttributeType::UINT16_NORMALIZED: 
        case VertexAttributeType::UINT16_2_NORMALIZED: 
        case VertexAttributeType::UINT16_3_NORMALIZED: 
        case VertexAttributeType::UINT16_4_NORMALIZED: return sizeof(std::uint16_t); break; 
        case VertexAttributeType::INT16_NORMALIZED: 
        case VertexAttributeType::INT16_2_NORMALIZED: 
        case VertexAttributeType::INT16_3_NORMALIZED: 
        case VertexAttributeType::INT16_4_NORMALIZED: return sizeof(std::int16_t); break; 
        case VertexAttributeType::UINT32_NORMALIZED: 
        case VertexAttributeType::UINT32_2_NORMALIZED: 
        case VertexAttributeType::UINT32_3_NORMALIZED: 
        case VertexAttributeType::UINT32_4_NORMALIZED: return sizeof(std::uint32_t); break; 
        case VertexAttributeType::INT32_NORMALIZED: 
        case VertexAttributeType::INT32_2_NORMALIZED: 
        case VertexAttributeType::INT32_3_NORMALIZED: 
        case VertexAttributeType::INT32_4_NORMALIZED: return sizeof(std::int32_t); break; 
        default: return GL_NONE; break;
        }

        return GL_NONE;       
    }

    inline static GLenum to_opengl_type(VertexAttributeType type)
    {
        switch(type)
        {
        case VertexAttributeType::FLOAT32:
        case VertexAttributeType::FLOAT32_2: 
        case VertexAttributeType::FLOAT32_3: 
        case VertexAttributeType::FLOAT32_4: return GL_FLOAT; break; 
        case VertexAttributeType::FLOAT64:
        case VertexAttributeType::FLOAT64_2: 
        case VertexAttributeType::FLOAT64_3: 
        case VertexAttributeType::FLOAT64_4: return GL_DOUBLE; break; 
        case VertexAttributeType::UINT8:  
        case VertexAttributeType::UINT8_2:  
        case VertexAttributeType::UINT8_3:  
        case VertexAttributeType::UINT8_4: return GL_UNSIGNED_BYTE; break; 
        case VertexAttributeType::INT8:  
        case VertexAttributeType::INT8_2:  
        case VertexAttributeType::INT8_3:  
        case VertexAttributeType::INT8_4: return GL_BYTE; break; 
        case VertexAttributeType::UINT16:  
        case VertexAttributeType::UINT16_2:  
        case VertexAttributeType::UINT16_3:  
        case VertexAttributeType::UINT16_4: return GL_UNSIGNED_SHORT; break; 
        case VertexAttributeType::INT16: 
        case VertexAttributeType::INT16_2: 
        case VertexAttributeType::INT16_3: 
        case VertexAttributeType::INT16_4: return GL_SHORT; break; 
        case VertexAttributeType::UINT32: 
        case VertexAttributeType::UINT32_2: 
        case VertexAttributeType::UINT32_3: 
        case VertexAttributeType::UINT32_4: return GL_UNSIGNED_INT; break; 
        case VertexAttributeType::INT32:  
        case VertexAttributeType::INT32_2:  
        case VertexAttributeType::INT32_3:  
        case VertexAttributeType::INT32_4: return GL_INT; break; 
        case VertexAttributeType::UINT8_NORMALIZED: 
        case VertexAttributeType::UINT8_2_NORMALIZED: 
        case VertexAttributeType::UINT8_3_NORMALIZED: 
        case VertexAttributeType::UINT8_4_NORMALIZED: return GL_UNSIGNED_BYTE; break; 
        case VertexAttributeType::INT8_NORMALIZED: 
        case VertexAttributeType::INT8_2_NORMALIZED: 
        case VertexAttributeType::INT8_3_NORMALIZED: 
        case VertexAttributeType::INT8_4_NORMALIZED: return GL_BYTE; break; 
        case VertexAttributeType::UINT16_NORMALIZED: 
        case VertexAttributeType::UINT16_2_NORMALIZED: 
        case VertexAttributeType::UINT16_3_NORMALIZED: 
        case VertexAttributeType::UINT16_4_NORMALIZED: return GL_UNSIGNED_SHORT; break; 
        case VertexAttributeType::INT16_NORMALIZED: 
        case VertexAttributeType::INT16_2_NORMALIZED: 
        case VertexAttributeType::INT16_3_NORMALIZED: 
        case VertexAttributeType::INT16_4_NORMALIZED: return GL_SHORT; break; 
        case VertexAttributeType::UINT32_NORMALIZED: 
        case VertexAttributeType::UINT32_2_NORMALIZED: 
        case VertexAttributeType::UINT32_3_NORMALIZED: 
        case VertexAttributeType::UINT32_4_NORMALIZED: return GL_UNSIGNED_INT; break; 
        case VertexAttributeType::INT32_NORMALIZED: 
        case VertexAttributeType::INT32_2_NORMALIZED: 
        case VertexAttributeType::INT32_3_NORMALIZED: 
        case VertexAttributeType::INT32_4_NORMALIZED: return GL_INT; break; 
        default: return GL_NONE; break;
        }

        return GL_NONE;
    }

    inline static GLint to_opengl_size(VertexAttributeType type)
    {
        return (static_cast<GLint>(type) % 4) + 1; // Look at how the types are numbered, each tell you how many components they have, just mod 4 and - 1
    }

    inline static GLboolean to_opengl_normalized(VertexAttributeType type)
    {
        return (static_cast<GLboolean>(type) & 0x80); // The last bit tells you whether normalized
    }

    VertexAttributes::VertexAttributes()
        : vao_(0)
    {
        glCreateVertexArrays(1, &vao_);
    }

    VertexAttributes::~VertexAttributes()
    {
        glDeleteVertexArrays(1, &vao_);
    }

    void VertexAttributes::bind_element_buffer(GLuint handle)
    {
        glVertexArrayElementBuffer(vao_, handle);
    }

    GLuint VertexAttributes::handle() const
    {
        return vao_;
    }



    SingleStreamVertexAttributes::SingleStreamVertexAttributes(const std::initializer_list<VertexAttributeType> &attributes)
        : VertexAttributes()
        , size_(0)
    {
        auto index = 0u;
        std::size_t size = 0u;
        for(const auto &type : attributes)
        {
            glVertexArrayAttribFormat(vao_, index, to_opengl_size(type), to_opengl_type(type), to_opengl_normalized(type), size /** Accumulating, we get the offset */);
            glEnableVertexArrayAttrib(vao_, index);
            glVertexArrayAttribBinding(vao_, index, 0);
            size += sizeof_type(type) * to_opengl_size(type);
            index++;
        }

        size_ = size;
    }

    void SingleStreamVertexAttributes::bind_vertex_stream(GLuint handle, GLuint offset, GLuint binding_point /** Unused */)
    {
        glVertexArrayVertexBuffer(vao_, 0, handle, offset, size_);
    }
}