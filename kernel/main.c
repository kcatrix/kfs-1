#include "screen.h"
#include "keyboard.h"

// Fonction de délai simple
static void delay(void) {
    for (volatile int i = 0; i < 100000; i++);
}

void kmain() {
    // Initialiser les écrans virtuels
    init_screens();
    enable_cursor(0, 15);
    keyboard_init(); // Initialiser le clavier
    
    // Écran 1
    switch_screen(0);
    clear_screen();
    print_string_color("===  Welcome ===\n\n", LIGHT_CYAN);
    print_string_color("=== SCREEN 1 ===\n\n", LIGHT_CYAN);
    print_string_color("42\n\n", GREEN);
    print_string_color("Press F1-F4 to switch screens\n", YELLOW);
    print_string_color("Type to test keyboard on each screen:\n\n", WHITE);
    
    // Écran 2
    switch_screen(1);
    clear_screen();
    print_string_color("=== SCREEN 2 ===\n\n", LIGHT_GREEN);
    print_string_color("Welcome to the second screen!\n", WHITE);
    print_string_color("This is a separate buffer.\n\n", LIGHT_GRAY);
    
    // Écran 3
    switch_screen(2);
    clear_screen();
    print_string_color("=== SCREEN 3 ===\n\n", YELLOW);
    print_string_color("Testing scroll on screen 3:\n", WHITE);
    for (int i = 0; i < 20; i++) {
        kprintf_color(CYAN, "Line %d\n", i + 1);
    }
    
    // Écran 4
    switch_screen(3);
    clear_screen();
    print_string_color("=== SCREEN 4 ===\n\n", LIGHT_MAGENTA);
    print_string_color("Color test screen:\n\n", WHITE);
    print_string_color("RED ", RED);
    print_string_color("GREEN ", GREEN);
    print_string_color("BLUE ", BLUE);
    print_string_color("YELLOW\n", YELLOW);
    
    // Revenir à l'écran 1
    switch_screen(0);
    
    // Boucle principale - lecture clavier
    char c;
    char last_c = 0; // Pour éviter les répétitions
    unsigned char colors[] = {LIGHT_CYAN, LIGHT_GREEN, YELLOW, LIGHT_MAGENTA};
    
    while (1) {
        c = keyboard_read_char();
        if (c && c != last_c) {
            // Afficher le caractère sur l'écran actuel
            print_char(c, -1, -1, colors[get_current_screen()]);
            last_c = c;
        } else if (!c) {
            last_c = 0; // Réinitialiser quand aucune touche n'est pressée
        }
        
        delay(); // Ralentir la boucle pour éviter les lectures multiples
    }
}