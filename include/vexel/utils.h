#pragma once

#include <stdexcept>
#include <string>
#include <source_location>

namespace vexel
{
    class Exception : public std::runtime_error
    {
    public:
        Exception(const std::string &what, const std::source_location &loc)
            : std::runtime_error(what)
            , loc_(loc)
        {
        }

    protected:
        std::source_location loc_;
    };

    #define VEXEL_THROW(type, str) throw type(str, std::source_location::current())

    #define VEXEL_SIZEOF_C_ARRAY(x) (sizeof(x) / sizeof(x[0]))
}