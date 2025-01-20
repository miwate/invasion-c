#include "../prot/tourelle.h"
#include <stdio.h>
#include <stdlib.h>

/* Créer une nouvelle tourelle*/
Tourelle* creerTourelle(int ligne, int position, char type, int prix) {
    Tourelle* nouvelleTourelle = malloc(sizeof(Tourelle));
    if (nouvelleTourelle == NULL) {
        printf("Erreur : Impossible de créer la tourelle.\n");
        return NULL;
    }

    nouvelleTourelle->pointsDeVie = 10; // Exemple de valeur par défaut
    nouvelleTourelle->ligne = ligne;
    nouvelleTourelle->position = position;
    nouvelleTourelle->prix = prix;
    nouvelleTourelle->type = type;
    nouvelleTourelle->next = NULL;

    return nouvelleTourelle;
}
