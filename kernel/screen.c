#include "screen.h"
#include "ports.h"

static int cursor_row = 0;
static int cursor_col = 0;
static unsigned char *video_memory = (unsigned char *)VIDEO_ADDRESS;

static inline int valid_row(int r) { return r >= 0 && r < MAX_ROWS; }
static inline int valid_col(int c) { return c >= 0 && c < MAX_COLS; }

void set_cursor(int row, int col) {
    if (row < 0) row = 0;
    if (col < 0) col = 0;
    if (row >= MAX_ROWS) row = MAX_ROWS - 1;
    if (col >= MAX_COLS) col = MAX_COLS - 1;

    unsigned short pos = (unsigned short)(row * MAX_COLS + col);
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
    cursor_row = row;
    cursor_col = col;
}

void scroll(void) {
    // décaler toutes les lignes vers le haut d'une ligne
    for (int row = 1; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {
            int src = (row * MAX_COLS + col) * 2;
            int dst = ((row - 1) * MAX_COLS + col) * 2;
            video_memory[dst] = video_memory[src];
            video_memory[dst + 1] = video_memory[src + 1];
        }
    }
    // vider la dernière ligne
    for (int col = 0; col < MAX_COLS; col++) {
        int off = ((MAX_ROWS - 1) * MAX_COLS + col) * 2;
        video_memory[off] = ' ';
        video_memory[off + 1] = WHITE_ON_BLACK;
    }
    // placer le curseur sur la dernière ligne, colonne 0
    cursor_row = MAX_ROWS - 1;
    cursor_col = 0;
    set_cursor(cursor_row, cursor_col);
}

void print_char(char c, int row, int col, unsigned char attr) {
    if (attr == 0) attr = WHITE_ON_BLACK;

    if (row >= 0 && col >= 0) {
        // position explicite
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

    // si on dépasse la dernière ligne, scroll une seule fois puis clamp
    if (cursor_row >= MAX_ROWS) {
        scroll();
    } else {
        set_cursor(cursor_row, cursor_col);
    }
}

void print_string(const char *str) {
    for (int i = 0; str[i] != '\0'; i++)
        print_char(str[i], -1, -1, WHITE_ON_BLACK);
}

void print_string_color(const char *str, unsigned char color) {
    for (int i = 0; str[i] != '\0'; i++)
        print_char(str[i], -1, -1, color);
}

void clear_screen(void) {
    for (int i = 0; i < MAX_ROWS * MAX_COLS; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = WHITE_ON_BLACK;
    }
    cursor_row = 0;
    cursor_col = 0;
    set_cursor(0, 0);
}
