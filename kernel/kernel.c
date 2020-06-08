
void main() {
    char *vga_begin = (char *)0xb8000;
    *vga_begin = 'X';
}

