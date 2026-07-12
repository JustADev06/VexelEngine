#pragma once

#include <cstdint>
#include <vector>

#include "utils.h"

namespace vexel
{
    enum class VertexAttributeType : std::uint8_t
    {
        FLOAT32 = 0,
        FLOAT32_2 = 1,
        FLOAT32_3 = 2,
        FLOAT32_4 = 3,
        FLOAT64 = 4,
        FLOAT64_2 = 5,
        FLOAT64_3 = 6,
        FLOAT64_4 = 7,
        UINT8 = 8,
        UINT8_2 = 9,
        UINT8_3 = 10,
        UINT8_4 = 11,
        INT8 = 12,
        INT8_2 = 13,
        INT8_3 = 14,
        INT8_4 = 15,
        UINT16 = 16,
        UINT16_2 = 17,
        UINT16_3 = 18,
        UINT16_4 = 19,
        INT16 = 20,
        INT16_2 = 21,
        INT16_3 = 22,
        INT16_4 = 23,
        UINT32 = 24,
        UINT32_2 = 25,
        UINT32_3 = 26,
        UINT32_4 = 27,
        INT32 = 28,
        INT32_2 = 29,
        INT32_3 = 30,
        INT32_4 = 31,

        // Trick, the last byte says whether normalized type of the underlying integer type
        UINT8_NORMALIZED = UINT8 | 0x80,
        UINT8_2_NORMALIZED = UINT8_2 | 0x80,
        UINT8_3_NORMALIZED = UINT8_3 | 0x80,
        UINT8_4_NORMALIZED = UINT8_4 | 0x80,
        INT8_NORMALIZED = INT8 | 0x80,
        INT8_2_NORMALIZED = INT8_2 | 0x80,
        INT8_3_NORMALIZED = INT8_3 | 0x80,
        INT8_4_NORMALIZED = INT8_4 | 0x80,
        UINT16_NORMALIZED = UINT16 | 0x80,
        UINT16_2_NORMALIZED = UINT16_2 | 0x80,
        UINT16_3_NORMALIZED = UINT16_3 | 0x80,
        UINT16_4_NORMALIZED = UINT16_4 | 0x80,
        INT16_NORMALIZED = INT16 | 0x80,
        INT16_2_NORMALIZED = INT16_2 | 0x80,
        INT16_3_NORMALIZED = INT16_3 | 0x80,
        INT16_4_NORMALIZED = INT16_4 | 0x80,
        UINT32_NORMALIZED = UINT32 | 0x80,
        UINT32_2_NORMALIZED = UINT32_2 | 0x80,
        UINT32_3_NORMALIZED = UINT32_3 | 0x80,
        UINT32_4_NORMALIZED = UINT32_4 | 0x80,
        INT32_NORMALIZED = INT32 | 0x80,
        INT32_2_NORMALIZED = INT32_2 | 0x80,
        INT32_3_NORMALIZED = INT32_3 | 0x80,
        INT32_4_NORMALIZED = INT32_4 | 0x80
    };

    class VertexAttributes
    {
    protected: // No non-derived instantiation
        VertexAttributes(); // Being protected means that only the derived class can run this, which is really done by default when constructing the derived class

    public:
        virtual void bind_vertex_stream(GLuint handle, GLuint offset, GLuint binding_point) = 0; // This prevents instantiation of this class anyway

        void bind_element_buffer(GLuint handle);

        GLuint handle() const;

        virtual ~VertexAttributes();
    
    protected:
        GLuint vao_;
    };

    class SingleStreamVertexAttributes : public VertexAttributes
    {
    public:
        SingleStreamVertexAttributes(const std::initializer_list<VertexAttributeType> &attributes);
        ~SingleStreamVertexAttributes() override = default;

        VEXEL_DELETE_COPY_MOVE(SingleStreamVertexAttributes);

        void bind_vertex_stream(GLuint handle, GLuint offset, GLuint binding_point = 0 /** There's only one stream */) override; // When overriding functions, then the default value in the override will be used

    private:
        std::size_t size_;
    };

    /** TODO: Add streaming vertices I guess, if necessary that is */
}