#include "screen.h"
#include "ports.h"
#include "string.h"

static int cursor_row = 0;
static int cursor_col = 0;
static unsigned char *video_memory = (unsigned char *)VIDEO_ADDRESS;

// Gestion multi-écrans
static screen_t screens[NUM_SCREENS];
static int current_screen = 0;

static inline int valid_row(int r) { return r >= 0 && r < MAX_ROWS; }
static inline int valid_col(int c) { return c >= 0 && c < MAX_COLS; }

// --------------------
// Multi-screen management
// --------------------
void init_screens(void) {
    // Initialiser tous les buffers d'écrans
    for (int s = 0; s < NUM_SCREENS; s++) {
        screens[s].cursor_row = 0;
        screens[s].cursor_col = 0;
        
        // Remplir avec des espaces
        for (int i = 0; i < SCREEN_SIZE; i += 2) {
            screens[s].buffer[i] = ' ';
            screens[s].buffer[i + 1] = WHITE_ON_BLACK;
        }
    }
    
    // Afficher un message différent sur chaque écran
    current_screen = 0;
    print_string_color("=== Screen 1 ===\n", LIGHT_CYAN);
    
    current_screen = 1;
    print_string_color("=== Screen 2 ===\n", LIGHT_GREEN);
    
    current_screen = 2;
    print_string_color("=== Screen 3 ===\n", YELLOW);
    
    current_screen = 3;
    print_string_color("=== Screen 4 ===\n", LIGHT_MAGENTA);
    
    // Revenir à l'écran 1
    switch_screen(0);
}

void switch_screen(int screen_num) {
    if (screen_num < 0 || screen_num >= NUM_SCREENS)
        return;
    
    // Sauvegarder l'état de l'écran actuel
    screens[current_screen].cursor_row = cursor_row;
    screens[current_screen].cursor_col = cursor_col;
    
    // Copier la mémoire vidéo dans le buffer de l'écran actuel
    for (int i = 0; i < SCREEN_SIZE; i++) {
        screens[current_screen].buffer[i] = video_memory[i];
    }
    
    // Changer d'écran
    current_screen = screen_num;
    
    // Restaurer le buffer du nouvel écran
    for (int i = 0; i < SCREEN_SIZE; i++) {
        video_memory[i] = screens[current_screen].buffer[i];
    }
    
    // Restaurer la position du curseur
    cursor_row = screens[current_screen].cursor_row;
    cursor_col = screens[current_screen].cursor_col;
    set_cursor(cursor_row, cursor_col);
}

int get_current_screen(void) {
    return current_screen;
}

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
            // CORRECTION: Synchroniser le buffer
            screens[current_screen].buffer[dst] = video_memory[src];
            screens[current_screen].buffer[dst + 1] = video_memory[src + 1];
        }
    }
    // clear last row
    for (int col = 0; col < MAX_COLS; col++) {
        int off = ((MAX_ROWS - 1) * MAX_COLS + col) * 2;
        video_memory[off] = ' ';
        video_memory[off + 1] = WHITE_ON_BLACK;
        // CORRECTION: Synchroniser le buffer
        screens[current_screen].buffer[off] = ' ';
        screens[current_screen].buffer[off + 1] = WHITE_ON_BLACK;
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
        // CORRECTION CRITIQUE: Synchroniser immédiatement le buffer
        screens[current_screen].buffer[offset] = (unsigned char)c;
        screens[current_screen].buffer[offset + 1] = attr;
        
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
        // CORRECTION: Synchroniser le buffer
        screens[current_screen].buffer[i * 2] = ' ';
        screens[current_screen].buffer[i * 2 + 1] = WHITE_ON_BLACK;
    }
    cursor_row = 0;
    cursor_col = 0;
    set_cursor(0, 0);
}


// --------------------
// Simple kprintf
// --------------------

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

// printf classique (sans couleur)
void kprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buf[32];

    for (int i = 0; fmt[i]; i++) {
        if (fmt[i] == '%' && fmt[i+1]) {
            i++;
            if (fmt[i] == 'd') {
                itoa(va_arg(args, int), buf);
                print_string(buf);
            } else if (fmt[i] == 's') {
                print_string(va_arg(args, char *));
            } else {
                print_char(fmt[i], -1, -1, WHITE_ON_BLACK);
            }
        } else {
            print_char(fmt[i], -1, -1, WHITE_ON_BLACK);
        }
    }
    va_end(args);
}