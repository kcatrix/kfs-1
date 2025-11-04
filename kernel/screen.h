#ifndef SCREEN_H
#define SCREEN_H

#include <stdarg.h>
#include <stdint.h>

#define VIDEO_ADDRESS 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80

// VGA colors (foreground | background << 4)
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHT_GRAY 7
#define DARK_GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define YELLOW 14
#define WHITE 15

#define WHITE_ON_BLACK (WHITE | (BLACK << 4))

// --------------------
// Basic screen functions
// --------------------
void clear_screen(void);
void print_char(char c, int row, int col, unsigned char attr);
void print_string(const char *str);
void print_string_color(const char *str, unsigned char color);
void set_cursor(int row, int col);
void scroll(void);
void enable_cursor(unsigned char start, unsigned char end);

// --------------------
// Bonus helper
// --------------------
void kprintf_color(unsigned char color, const char *fmt, ...);


#endif
