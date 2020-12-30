

#include "include/vga.hpp"
#include "include/bus_io.hpp"
#include "include/kutils.hpp"

void test_dummy_function() {
    auto tmp = read_byte_from_bus(0x3f2);
    tmp |= 0x08;
    write_byte_to_bus(0x3f2, tmp);
}

void kernel_init() {
    impl::kalloc_init_pool();
}

void main() {
    clear_screen();
    print("Hello world!\n");
    print("Hello world!\n", 0xb1);
    print("Hello world!\n", 0xae);
    print("Hello world!\n", 0x5c);
    print("Hello world! This is recolic's test message, \n");
}

