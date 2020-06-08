
void main();
void __entry() {
    main();
}

void print_char(char to_print) {
    asm inline (
            "mov $0x0e, %%ah\n\t"
            "mov %0, %%al\n\t"
            "int $0x10"
            : 
            : "Ir" (to_print)
            : "ax"
            );
}

void println(char *s) {
    while(*s != 0) {
        print_char(*s);
    }
}
    


void main() {
    asm (
            "mov $0x0e, %ah\n\t"
            "mov $0x55, %al\n\t"
            "int $0x10"
            );
    //print_char('X');
    //println("Hello world!");
    //println("Recolic Booting OS HERE...");

    asm (
    "jmp 0x-2\n\t"
    );
}


