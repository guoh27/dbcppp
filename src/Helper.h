
#pragma once

#include <string>
#include <memory>

#include "Export.h"

#include "EndianConfig.h"

#ifdef _MSC_VER
#   include <stdlib.h>
#   define bswap_32(x) _byteswap_ulong(x)
#   define bswap_64(x) _byteswap_uint64(x)
#elif defined(__APPLE__)
    // Mac OS X / Darwin features
#   include <libkern/OSByteOrder.h>
#   define bswap_32(x) OSSwapInt32(x)
#   define bswap_64(x) OSSwapInt64(x)
#elif defined(__sun) || defined(sun)
#   include <sys/byteorder.h>
#   define bswap_32(x) BSWAP_32(x)
#   define bswap_64(x) BSWAP_64(x)
#elif defined(__FreeBSD__)
#   include <sys/endian.h>
#   define bswap_32(x) bswap32(x)
#   define bswap_64(x) bswap64(x)
#elif defined(__OpenBSD__)
#   include <sys/types.h>
#   define bswap_32(x) swap32(x)
#   define bswap_64(x) swap64(x)
#elif defined(__NetBSD__)
#   include <sys/types.h>
#   include <machine/bswap.h>
#   if defined(__BSWAP_RENAME) && !defined(__bswap_32)
#       define bswap_32(x) bswap32(x)
#       define bswap_64(x) bswap64(x)
#   endif
#else
#   include <byteswap.h>
#endif

namespace dbcppp
{
    class NodeImpl;
    struct DBCPPP_API SharedNodeCmp
    {
        bool operator()(const NodeImpl& lhs, const NodeImpl& rhs) const;
    };
    inline void native_to_big_inplace(uint64_t& value)
    {
        if constexpr (dbcppp::Endian::Native == dbcppp::Endian::Little)
        {
            value = bswap_64(value);
        }
    }
    inline void native_to_little_inplace(uint64_t& value)
    {
        if constexpr (dbcppp::Endian::Native == dbcppp::Endian::Big)
        {
            value = bswap_64(value);
        }
    }
    template<typename T>
    inline void compare_set(T& v1, T& v2)
    {
        if (v1 != v2) {
            v1 = std::move(v2);
        }
    }
    template<typename T>
    inline void unique_merge(std::vector<T>& v1, std::vector<T>& v2)
    {
        for (T& item : v2) {
            if (std::find(v1.begin(), v1.end(), item) == v1.end()) {
                v1.push_back(std::move(item));
            }
        }
    }
    template<typename T, typename R>
    inline void unique_merge_by_attr(std::vector<T>& v1, std::vector<T>& v2, R (T::*func)() const)
    {
        for (T& item2 : v2) {
            auto it = std::find_if(v1.begin(), v1.end(), [&item2, func](const T& item1) {
                return (item1.*func)() == (item2.*func)();
            });
            if (it != v1.end()) {
                // same name replace it
                *it = std::move(item2);
            } else {
                // insert new
                v1.push_back(std::move(item2));
            }
        }
    }
    template<typename T>
    inline void unique_merge_by_name(std::vector<T>& v1, std::vector<T>& v2)
    {
        unique_merge_by_attr(v1, v2, &T::Name);
    }
}
