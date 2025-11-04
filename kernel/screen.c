#include "screen.h"

#define VIDEO_MEMORY 0xb8000
#define WHITE_ON_BLACK 0x0F

void print_string(char *str) {
    unsigned char *vidmem = (unsigned char *)VIDEO_MEMORY;
    int i = 0;
    while (str[i] != '\0') {
        vidmem[i * 2] = str[i];
        vidmem[i * 2 + 1] = WHITE_ON_BLACK;
        i++;
    }
}

void clear_screen() {
    unsigned char *vidmem = (unsigned char *)VIDEO_MEMORY;
    for (int i = 0; i < 80 * 25 * 2; i++)
        vidmem[i] = 0;
}
