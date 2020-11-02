#ifndef ROS_KERN_BUS_IO_HPP
#define ROS_KERN_BUS_IO_HPP
#include "stdint.hpp"

inline auto read_byte_from_bus(uint16_t port) {
    uint8_t result;
    asm ("in %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}
inline auto read_word_from_bus(uint16_t port) {
    uint16_t result;
    asm ("in %%dx, %%ax" : "=a"(result) : "d"(port));
    return result;
}
inline auto read_dword_from_bus(uint16_t port) {
    uint32_t result;
    asm ("in %%dx, %%eax" : "=a"(result) : "d"(port));
    return result;
}

inline void write_byte_to_bus(uint16_t port, uint8_t data) {
    asm ("out %%al, %%dx": : "a"(data), "d"(port));
}
inline void write_word_to_bus(uint16_t port, uint16_t data) {
    asm ("out %%ax, %%dx": : "a"(data), "d"(port));
}
inline void write_dword_to_bus(uint16_t port, uint32_t data) {
    asm ("out %%eax, %%dx": : "a"(data), "d"(port));
}





#endif
