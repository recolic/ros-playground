

#include "include/vga.hpp"
#include "include/bus_io.hpp"

void test_dummy_function() {
    auto tmp = read_byte_from_bus(0x3f2);
    tmp |= 0x08;
    write_byte_to_bus(0x3f2, tmp);
}

void main() {
    for(auto y = 0; y < 25; ++y) {
        for(auto x = 0; x < 80; ++x) {
            char c = x%10 + '0';
            char color = x + y*80;
            set_char(x, y, 'F', 0x0f);
        }
    }
    print("Hello world!\n");
    print("Hello world!\n", 0xb1);
    print("Hello world!\n", 0xae);
    print("Hello world!\n", 0x5c);
    print("Hello world! This is recolic's test message, \n");
}

