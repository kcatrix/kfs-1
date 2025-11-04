#ifndef PORTS_H
#define PORTS_H

#define VIDEO_ADDRESS 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK (15 | (0 << 4))

void clear_screen(void);
void print_char(char c, int row, int col, unsigned char attr);
void print_string(const char *str);
void print_string_color(const char *str, unsigned char color);
void set_cursor(int row, int col);
void scroll(void);

#endif
