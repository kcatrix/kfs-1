#include "keyboard.h"
#include "ports.h"
#include "screen.h"

// Scancodes des touches F1-F4
#define SCANCODE_F1 0x3B
#define SCANCODE_F2 0x3C
#define SCANCODE_F3 0x3D
#define SCANCODE_F4 0x3E

// Table simplifiée scancode -> ASCII
static char scancode_table[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`',0,
    '\\','z','x','c','v','b','n','m',',','.','/',0, '*',0, ' ',
};

char keyboard_read_char(void) {
    unsigned char scancode = inb(KEYBOARD_DATA_PORT);

    // Gérer les touches F1-F4 pour changer d'écran
    if (scancode == SCANCODE_F1) {
        switch_screen(0);
        return 0;
    } else if (scancode == SCANCODE_F2) {
        switch_screen(1);
        return 0;
    } else if (scancode == SCANCODE_F3) {
        switch_screen(2);
        return 0;
    } else if (scancode == SCANCODE_F4) {
        switch_screen(3);
        return 0;
    }

    // On ne garde que les pressions (0–127)
    if (scancode < 128) {
        char c = scancode_table[scancode];
        if (c) {
            return c;
        }
    }

    return 0;
}

void keyboard_init(void) {
    // Ici on pourrait activer IRQ1 via le PIC
}