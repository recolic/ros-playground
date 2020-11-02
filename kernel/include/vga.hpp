#ifndef ROS_KERN_VGA_HPP
#define ROS_KERN_VGA_HPP

inline char *vga_begin = (char *)0xb8000;
void push_char(char c, char color) {
    static int pos = 0;
    vga_begin[pos++] = c;
    vga_begin[pos++] = color;
}

void set_char(int x, int y, char c, char color) {
    auto pos = x + 80 * y;
    vga_begin[pos*2] = c;
    vga_begin[pos*2+1] = color;
}

#endif