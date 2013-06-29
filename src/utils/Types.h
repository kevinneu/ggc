#ifndef __TYPES_H_
#define __TYPES_H_

#include <stdint.h>

#include <string>

namespace dbdky
{
using std::string;

template<typename To, typename From>
inline To implicit_cast(From const &f)
{
    return f;
}

template<typename To, typename From>
inline To down_cast(From* f)
{
#if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
    assert((f == NULL) || (dynamic_cast<To>(f) != NULL));
#endif

    return static_cast<To>(f);
}

}
#endif
