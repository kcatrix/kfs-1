#include "screen.h"

void kmain() {
    // Clear screen et activer curseur visible
    clear_screen();
    // enable_cursor(0, 15);

    // // Affichage simple obligatoire
    // print_string_color("42\n\n", GREEN);

    // // Test du scroll et couleurs
    // print_string_color("Test du scroll avec différentes couleurs :\n", YELLOW);

    // for (int i = 0; i < 35; i++) {
    //     unsigned char color = (i % 2 == 0) ? CYAN : LIGHT_MAGENTA;
    //     kprintf_color(color, "Ligne %d : Hello World !\n", i + 1);
    // }


    // // Test kprintf avec %d et %s
    // kprintf_color(WHITE_ON_BLACK, "\n%s : %d lignes affichées avec scroll\n", "Résumé", 35);

    // // Boucle infinie pour figer l'affichage
    // while (1) {}

    char c;
    print_string_color("Tapez quelque chose : ", YELLOW);
    while (1) {
        c = keyboard_read_char();
        if (c)
            print_char(c, -1, -1, GREEN);
    }

}
