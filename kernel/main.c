#include "screen.h"

void kmain() {
    clear_screen();

    print_string_color("Test du scroll ↓↓↓\n\n", YELLOW);

    // On imprime 30 lignes pour dépasser la taille de l’écran
    for (int i = 0; i < 35; i++) {
        print_string_color("Ligne numéro ", CYAN);
        print_string("\n");
    }

    print_string_color("\nScroll terminé.\n", GREEN);

    // Boucle d’attente pour figer l’affichage
    while (1) {}
}
