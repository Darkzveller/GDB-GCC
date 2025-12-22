#include "affichage.h"
#include <stdio.h>
#include <stdlib.h>
#include "lecture.h"

/* pré-construire le labyrinthe en récupérant les données depuis un fichier */
int build_labyrinthe(const char *nom_fichier, int coordonnees_labyrinthe[TAILLE_X][TAILLE_Y]) {
    FILE *file = fopen(nom_fichier, "r");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", nom_fichier);
        return EXIT_FAILURE;// Renvoie un code erreur, comme quoi on a pas réussi a ouvrir le fichier
    }

    /* On récupére les deux pairs de coordonnées de départ et arrivée */
    fscanf(file, "%d %d %d %d", &coord_x_depart, &coord_y_depart, &coord_x_arrivee, &coord_y_arrivee);

    /* Copie les coordonnées du labyrinthe dans la tableau mise en argument */
    for (int y = 0; y < TAILLE_Y; y++) {
        for (int x = 0; x < TAILLE_X; x++) {
            fscanf(file, "%d", &coordonnees_labyrinthe[x][y]);
        }
    }

    fclose(file);
    return 1; 
}
