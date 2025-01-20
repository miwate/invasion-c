#include "prot/jeu.h"
#include "prot/etudiant.h"
#include "prot/tourelle.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    if (argc<2){
        printf("Aucun fichier donné.\n");
        return 1;
    }

    for (int i=1; i<argc; i++) {
        printf("%s\n", argv[i]);

        Jeu* jeu = malloc(sizeof(Jeu));
        if (jeu == NULL){
            printf("Impossible d'allouer l'espace mémoire du Jeu\n");
            return 1;
        }

        initJeu(jeu);
        loadFichier(jeu, argv[i]);
        previewVagues(jeu);
        free(jeu);

    }

    printf("Fin\n");
    return 0;
}


