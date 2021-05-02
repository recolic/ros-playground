#ifndef ROS_KERN_VGA_HPP
#define ROS_KERN_VGA_HPP

#include "stdint.hpp"
#include "stdlib.hpp"

#define VGA_BEGIN_ADDR ((uint16_t *)0xb8000)
constexpr uint16_t VGA_WIDTH = 80;
constexpr uint16_t VGA_HEIGHT = 25;
constexpr uint16_t VGA_MAKE_CHAR(char c, uint8_t color) {
    return (color << 8) + uint8_t(c);
}
constexpr uint8_t default_color = 0x0f;

inline void trigger_scroll(uint16_t *pos) {
    for(uint16_t row = 1; row < VGA_HEIGHT; ++row) {
        memcpy(VGA_BEGIN_ADDR + (row-1)*VGA_WIDTH, VGA_BEGIN_ADDR + row*VGA_WIDTH, VGA_WIDTH);
    }
    memset(VGA_BEGIN_ADDR + (VGA_HEIGHT-1)*VGA_WIDTH, VGA_MAKE_CHAR(0, 0), VGA_WIDTH);
    (*pos) -= VGA_WIDTH;
}

inline void set_char(uint16_t x, uint16_t y, char c, char color) {
    VGA_BEGIN_ADDR[y*VGA_WIDTH + x] = VGA_MAKE_CHAR(c, color);
}

inline void print_char(char c, uint8_t color = default_color) {
    static uint16_t pos = 0;
    if(pos >= VGA_WIDTH * VGA_HEIGHT)
        trigger_scroll(&pos);

    switch(c) {
    case '\n':
        pos += VGA_WIDTH;
        [[fallthrough]]; // unix \n implies \r.
    case '\r':
        pos -= pos % VGA_WIDTH;
        break;
    default:
        VGA_BEGIN_ADDR[pos++] = VGA_MAKE_CHAR(c, color);
    }
}

inline void print(const char *cstr, uint8_t color = default_color) {
    while(*cstr != '\0') {
        print_char(*(cstr++), color);
    }
}

inline void clear_screen() {
    for(auto y = 0; y < 25; ++y) {
        for(auto x = 0; x < 80; ++x) {
            char c = x%10 + '0';
            char color = x + y*80;
            set_char(x, y, ' ', 0x0f);
        }
    }   
}

#endif
