#include "../prot/jeu.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Aucun fichier donné.\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);
        Jeu* jeu = malloc(sizeof(Jeu));
        if (jeu == NULL) {
            printf("Erreur : Jeu\n");
            return 1;
        }
        initJeu(jeu);
        chargerFichier(jeu, argv[i]);

        prevualisationVagues(jeu);

        printf("Appuyez sur Entrée pour jouer...\n");
        getchar();

        renduActuelJeu(jeu);

        free(jeu);
    }
    return 0;
}
