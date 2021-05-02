
#include <stdint.h>
#define ROS_KERN_STDINT_HPP

#ifndef ROS_KERN_STDINT_HPP
#define ROS_KERN_STDINT_HPP

using uint8_t = unsigned char;
using uint16_t = unsigned short;
using uint32_t = unsigned int;
using uint64_t = unsigned long long;
static_assert(sizeof(uint8_t) == 1);
static_assert(sizeof(uint16_t) == 2);
static_assert(sizeof(uint32_t) == 4);
static_assert(sizeof(uint64_t) == 8);

using sint8_t  = signed char;
using sint16_t = signed short;
using sint32_t = signed int;
using sint64_t = signed long long;
static_assert(sizeof(sint8_t) == 1);
static_assert(sizeof(sint16_t) == 2);
static_assert(sizeof(sint32_t) == 4);
static_assert(sizeof(sint64_t) == 8);

using size_t = uint32_t;

#endif
