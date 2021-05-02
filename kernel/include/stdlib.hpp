#ifndef ROS_KERN_STDLIB_HPP
#define ROS_KERN_STDLIB_HPP

#include "stdint.hpp"

#ifndef NULL
#define NULL 0
#endif

template <typename T>
inline void memcpy(T *dst, const T *src, uint32_t count) {
    for(auto i = 0; i < count; ++i) {
        dst[i] = src[i];
    }
}
template <typename T>
inline void memset(T *dst, const T &data_to_set, uint32_t count) {
    char *cdst = reinterpret_cast<char *>(dst);

    for(auto i = 0; i < count; ++i) {
        cdst[i] = data_to_set;
    }
}



#endif
