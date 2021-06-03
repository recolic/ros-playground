

#include "include/vga.hpp"
#include "include/bus_io.hpp"
#include "include/kutils.hpp"

#include <string>

void test_dummy_function() {
    auto tmp = read_byte_from_bus(0x3f2);
    tmp |= 0x08;
    write_byte_to_bus(0x3f2, tmp);
}

void kernel_init() {
    impl::kalloc_init_pool();
}

void main() {
    kernel_init();

    clear_screen();
    print("Hello world!\n");
    print("Hello world!\n", 0xb1);
    print("Hello world!\n", 0xae);
    print("Hello world!\n", 0x5c);
    print("Hello world! This is recolic's test message, \n");

    // std::array<uint32_t, 32> test_buf;
    // uint32_t * test_buf = (uint32_t *)kalloc_impl(sizeof(uint32_t) * 32);
    // for(auto i = 0; i < 32; ++i) {
    //     test_buf[i] = i == 0 ? 1 : test_buf[i-1]*2+1;
    //     print_char('0' + test_buf[i], default_color);
    // }
    // std::string s = "cxxhello world";
    // std::basic_string<char, std::char_traits<char>, kallocator<char>> s = "cxx hello world";
    // print(s.c_str());
}

