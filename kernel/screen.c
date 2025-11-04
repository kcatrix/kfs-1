#include "screen.h"
#include "ports.h"

static int cursor_row = 0;
static int cursor_col = 0;
static unsigned char *video_memory = (unsigned char *)VIDEO_ADDRESS;

static inline int valid_row(int r) { return r >= 0 && r < MAX_ROWS; }
static inline int valid_col(int c) { return c >= 0 && c < MAX_COLS; }

// --------------------
// Cursor control
// --------------------
void enable_cursor(unsigned char start, unsigned char end) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | start);
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | end);
}

void set_cursor(int row, int col) {
    if (!valid_row(row)) row = MAX_ROWS - 1;
    if (!valid_col(col)) col = 0;
    cursor_row = row;
    cursor_col = col;

    unsigned short pos = (cursor_row * MAX_COLS + cursor_col);
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}

// --------------------
// Scrolling
// --------------------
void scroll(void) {
    for (int row = 1; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {
            int src = (row * MAX_COLS + col) * 2;
            int dst = ((row - 1) * MAX_COLS + col) * 2;
            video_memory[dst] = video_memory[src];
            video_memory[dst + 1] = video_memory[src + 1];
        }
    }
    // clear last row
    for (int col = 0; col < MAX_COLS; col++) {
        int off = ((MAX_ROWS - 1) * MAX_COLS + col) * 2;
        video_memory[off] = ' ';
        video_memory[off + 1] = WHITE_ON_BLACK;
    }
    cursor_row = MAX_ROWS - 1;
    cursor_col = 0;
    set_cursor(cursor_row, cursor_col);
}

// --------------------
// Printing characters
// --------------------
void print_char(char c, int row, int col, unsigned char attr) {
    if (attr == 0) attr = WHITE_ON_BLACK;

    if (row >= 0 && col >= 0) {
        if (!valid_row(row)) row = MAX_ROWS - 1;
        if (!valid_col(col)) col = 0;
        cursor_row = row;
        cursor_col = col;
    }

    if (c == '\r') {
        cursor_col = 0;
    } else if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else {
        int offset = (cursor_row * MAX_COLS + cursor_col) * 2;
        video_memory[offset] = (unsigned char)c;
        video_memory[offset + 1] = attr;
        cursor_col++;
        if (cursor_col >= MAX_COLS) {
            cursor_col = 0;
            cursor_row++;
        }
    }

    if (cursor_row >= MAX_ROWS) {
        scroll();
    } else {
        set_cursor(cursor_row, cursor_col);
    }
}

void print_string(const char *str) {
    for (int i = 0; str[i]; i++)
        print_char(str[i], -1, -1, WHITE_ON_BLACK);
}

void print_string_color(const char *str, unsigned char color) {
    for (int i = 0; str[i]; i++)
        print_char(str[i], -1, -1, color);
}

// --------------------
// Clear screen
// --------------------
void clear_screen(void) {
    for (int i = 0; i < MAX_ROWS * MAX_COLS; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = WHITE_ON_BLACK;
    }
    cursor_row = 0;
    cursor_col = 0;
    set_cursor(0, 0);
}

// --------------------
// Simple kprintf
// Supports %s and %d only
// --------------------
static void itoa(int value, char* str) {
    char buffer[12];
    int i = 0, j, sign = 0;

    if (value < 0) { sign = 1; value = -value; }
    do { buffer[i++] = (value % 10) + '0'; } while ((value /= 10) > 0);
    if (sign) buffer[i++] = '-';
    for (j = 0; j < i; j++) str[j] = buffer[i - j - 1];
    str[i] = '\0';
}

void kprintf_color(unsigned char color, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buf[32];

    for (int i = 0; fmt[i]; i++) {
        if (fmt[i] == '%' && fmt[i+1]) {
            i++;
            if (fmt[i] == 'd') {
                itoa(va_arg(args, int), buf);
                print_string_color(buf, color);
            } else if (fmt[i] == 's') {
                print_string_color(va_arg(args, char *), color);
            } else {
                print_char(fmt[i], -1, -1, color);
            }
        } else {
            print_char(fmt[i], -1, -1, color);
        }
    }
    va_end(args);
}
