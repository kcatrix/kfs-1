#include "keyboard.h"
#include "ports.h"

// Table simplifiée scancode -> ASCII
static char scancode_table[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`',0,
    '\\','z','x','c','v','b','n','m',',','.','/',0, '*',0, ' ',
    // reste à zéro
};


char keyboard_read_char(void) {
    unsigned char scancode = inb(KEYBOARD_DATA_PORT);

    // On ne garde que les pressions (0–127)
    if (scancode < 128) {
        char c = scancode_table[scancode];
        if (c) {
            // Attendre que la touche soit relâchée
            while (inb(KEYBOARD_DATA_PORT) < 128);
            return c;
        }
    }

    return 0;
}


void keyboard_init(void) {
    // Ici on pourrait activer IRQ1 via le PIC (non inclus pour KFS_1 simple)
}
