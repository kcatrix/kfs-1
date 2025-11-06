#ifndef SCREEN_H
#define SCREEN_H

typedef __builtin_va_list va_list;
#define va_start(ap, param) __builtin_va_start(ap, param)
#define va_end(ap)         __builtin_va_end(ap)
#define va_arg(ap, type)   __builtin_va_arg(ap, type)

#define VIDEO_ADDRESS 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define SCREEN_SIZE (MAX_ROWS * MAX_COLS * 2)

// Nombre d'écrans virtuels
#define NUM_SCREENS 4

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

// Structure pour gérer un écran
typedef struct {
    unsigned char buffer[SCREEN_SIZE];  // Buffer pour cet écran
    int cursor_row;
    int cursor_col;
} screen_t;

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
// Multi-screen functions
// --------------------
void init_screens(void);
void switch_screen(int screen_num);
int get_current_screen(void);

// --------------------
// Bonus helper
// --------------------
void kprintf_color(unsigned char color, const char *fmt, ...);

#endif