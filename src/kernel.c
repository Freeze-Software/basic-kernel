#include <stdint.h>

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static void serial_init(void) {
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x80);
    outb(0x3F8 + 0, 0x03);
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x03);
    outb(0x3F8 + 2, 0xC7);
    outb(0x3F8 + 4, 0x0B);
}

static int serial_can_write(void) {
    return inb(0x3F8 + 5) & 0x20;
}

static void serial_write_char(char c) {
    while (!serial_can_write()) {
    }
    outb(0x3F8, (uint8_t)c);
}

static void serial_write_string(const char *str) {
    while (*str != '\0') {
        if (*str == '\n') {
            serial_write_char('\r');
        }
        serial_write_char(*str);
        str++;
    }
}

static volatile uint16_t *const VGA_MEMORY = (volatile uint16_t *)0xB8000;
static uint8_t vga_row = 0;
static uint8_t vga_col = 0;

static void vga_put_char_color(char c, uint8_t color) {
    if (c == '\n') {
        vga_col = 0;
        vga_row++;
        return;
    }

    const uint16_t index = (uint16_t)vga_row * 80 + vga_col;
    VGA_MEMORY[index] = (uint16_t)color << 8 | (uint8_t)c;

    vga_col++;
    if (vga_col >= 80) {
        vga_col = 0;
        vga_row++;
    }
}

static void vga_put_char(char c) {
    vga_put_char_color(c, 0x0F);
}

static void vga_write_string(const char *str) {
    while (*str != '\0') {
        vga_put_char(*str);
        str++;
    }
}

static void vga_write_string_color(const char *str, uint8_t color) {
    while (*str != '\0') {
        vga_put_char_color(*str, color);
        str++;
    }
}

void kmain(void) {
    serial_init();

    vga_write_string_color("Hello World\n", 0x01);
    vga_write_string("basic-kernel booted\n");
    vga_write_string("video output: OK\n");

    serial_write_string("basic-kernel booted\n");
    serial_write_string("serial output: OK\n");

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
