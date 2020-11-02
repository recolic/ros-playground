#ifndef ROS_KERN_STDLIB_HPP
#define ROS_KERN_STDLIB_HPP

#include "stdint.hpp"

template <typename T>
inline void memcpy(T *dst, const T *src, size_t count) {
    for(auto i = 0; i < count; ++i) {
        dst[i] = src[i];
    }
}
template <typename T>
inline void memset(T *dst, const T &data_to_set, size_t count) {
    char *cdst = reinterpret_cast<char *>(dst);

    for(auto i = 0; i < count; ++i) {
        cdst[i] = data_to_set;
    }
}


#endif
