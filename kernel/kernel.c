

char *vga_begin = (char *)0xb8000;
void push_char(char c, char color) {
    static int pos = 0;
    vga_begin[pos++] = c;
    vga_begin[pos++] = color;
}

void main() {
    int pos = 0;
    for(int cter = 0; cter < 256; ++cter) {
        push_char(cter % 10 + '0', cter);
        for(int i = 1; i < 10; ++i) {
            push_char(i + '0', cter);
        }
    }

}

