#include "affichage.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>  
#include "lecture.h"
// Commande gcc : 
// rm launch.out affichage.o labyrinthe.o lecture.o 
// ls -l *.o *.out && 
// gcc -c labyrinthe.c && gcc -c affichage.c && gcc affichage.o labyrinthe.o -o launch.out -lX11 && ./launch.out
// gcc -c labyrinthe.c && gcc -c affichage.c && gcc -c lecture.c && gcc affichage.o labyrinthe.o lecture.o -o launch.out -lX11 && ./launch.out

int coord_x_depart;
int coord_y_depart;
int coord_x_arrivee;
int coord_y_arrivee;

void coord_case_voisine(int coord_x_actu, int coord_y_actu, int direction_voulue, int *coord_x_voisine, int *coord_y_voisine);
int phaseExpansion(int tab_representative_labyrinthe[TAILLE_X][TAILLE_Y], int coord_x_depart, int coord_y_depart, int coord_x_arrivee, int coord_y_arrivee);
int phaseRemontee(int lab[TAILLE_X][TAILLE_Y], int coord_x_depart, int coord_y_depart, int coord_x_arrivee, int coord_y_arrivee);
void nettoyerLabyrinthe(int lab[TAILLE_X][TAILLE_Y]);

int main(void) {

    int tab_representative_labyrinthe[TAILLE_X][TAILLE_Y];

    printf("Caca test print\n");

    //pré-charger le labyrinthe depuis le fichier
    if (!build_labyrinthe("laby2.txt", tab_representative_labyrinthe)) {
        printf("Error chargement fichier\n");
        return 1;
    }

    //Initialisation affichage 
    if (!initAffichage()) {
        printf("Error initialisation de l'affichage\n");
        return 1;
    }

    // Coordonnées de départ et d'arrivée qu'on archive
    int couple_cordonne_depart[2] = {coord_x_depart, coord_y_depart};
    int couple_cordonne_arrivee[2] = {coord_x_arrivee, coord_y_arrivee};

    // Phase d'expansion
    bool chemin_existe = phaseExpansion(tab_representative_labyrinthe, coord_x_depart, coord_y_depart, coord_x_arrivee, coord_y_arrivee);

    if (chemin_existe) {
        printf("Chemin trouvé !!! B)\n");
    } else {
        printf("Pas de chemin possible entre départ et arrivée\n");
    }

    // Phase de remontée
    bool remontée_ok = phaseRemontee(tab_representative_labyrinthe, coord_x_depart, coord_y_depart, coord_x_arrivee, coord_y_arrivee);
    if (!remontée_ok) {
        printf("Erreur lors de la remontée du chemin\n");
    } else {
        printf("Chemin correctement tracé dans le labyrinthe\n");
    }

    // Nettoie le labyrinthe
    nettoyerLabyrinthe(tab_representative_labyrinthe);

    // Afficher le labyrinthe (avec le chemin marqué)
    afficheLabyrinthe(tab_representative_labyrinthe, (int[2]){coord_x_depart, coord_y_depart},(int[2]){coord_x_arrivee, coord_y_arrivee}, 0);

    return 0;
}

// Renvoie les coordonnées de la case voisine à partir d'une case (x, y) et d'une direction d
// d = 0 : Nord, 1 : Est, 2 : Sud, 3 : Ouest
 void coord_case_voisine(int coord_x_actu, int coord_y_actu, int direction_voulue, int *coord_x_voisine, int *coord_y_voisine)
{
    if (direction_voulue == 0) 
    {  
        // Nort
        *coord_x_voisine = coord_x_actu;
        *coord_y_voisine = coord_y_actu - 1;

    } else if (direction_voulue == 1) 
    {     
        // Est
        *coord_x_voisine = coord_x_actu + 1;
        *coord_y_voisine = coord_y_actu;

    } else if (direction_voulue == 2) 
    {    
        // Sud       
        *coord_x_voisine = coord_x_actu;
        *coord_y_voisine = coord_y_actu + 1;

    } else if (direction_voulue == 3) 
    {    
        // Ouest
        *coord_x_voisine= coord_x_actu - 1;
        *coord_y_voisine= coord_y_actu;

    } else 
    {
        // Au cas ou, mauvaise détection
        *coord_x_voisine = coord_x_actu;
        *coord_y_voisine = coord_y_actu;
    }
}  
/*
  Question 5 :

  Si on cherche à utiliser la fonction `coord_case_voisine` pour une case
  située sur le bord du tableau (coordonnées x=0, x=TAILLE_X-1, y=0, y=TAILLE_Y-1), 
  certaines directions pointeraient **hors du tableau** :

  Cela provoquerait un accès mémoire invalide si on essayait de lire/écrire
  dans le tableau avec ces coordonnées.

  Pour se prémunir de ce problème, on considère que toutes les cases de la périphérie du labyrinthe sont bloquées,
  et donc on n'appellera pas la fonction coord_case_voisine depuis une case
  située sur la bordure.
*/

// Renvoie true ssi un chemin existe, false sinon
int phaseExpansion(int tab_representative_labyrinthe[TAILLE_X][TAILLE_Y], int coord_x_depart, int coord_y_depart, int coord_x_arrivee, int coord_y_arrivee)
{
    int distance_parcourue = 1; // distance initiale
    int nouvelles_cases_marquees = 1;

    tab_representative_labyrinthe[coord_x_depart][coord_y_depart] = distance_parcourue;

    while (tab_representative_labyrinthe[coord_x_arrivee][coord_y_arrivee] == 0 && nouvelles_cases_marquees) {
        nouvelles_cases_marquees = 0;

        // Parcours de toutes les cases (ignore les bordures)
        for (int y = 1; y < TAILLE_Y - 1; y++) {
            for (int x = 1; x < TAILLE_X - 1; x++) {

                if (tab_representative_labyrinthe[x][y] == distance_parcourue) {
                    // Vérifie les 4 voisins
                    for (int d = 0; d < 4; d++) {
                        int nx, ny;
                        coord_case_voisine(x, y, d, &nx, &ny);

                        if (tab_representative_labyrinthe[nx][ny] == 0) { // case libre non visitée
                            tab_representative_labyrinthe[nx][ny] = distance_parcourue + 1;
                            nouvelles_cases_marquees = 1;
                        }
                    }
                }
            }
        }

        distance_parcourue++;
    }

    // Renvoie true si l'arrivée a été atteinte, false sinon
    return tab_representative_labyrinthe[coord_x_arrivee][coord_y_arrivee] > 0;
}

// Renvoie true si le chemin a été correctement remonté, false sinon
int phaseRemontee(int lab[TAILLE_X][TAILLE_Y], int coord_x_depart, int coord_y_depart, int coord_x_arrivee, int coord_y_arrivee) 
{
    int x_courant = coord_x_arrivee;
    int y_courant = coord_y_arrivee;
    int distance_actuelle = lab[x_courant][y_courant];

    if (distance_actuelle <= 0) {
        // L'arrivée n'a pas été atteinte pendant l'expansion
        return false;
    }

    // Marque la case d'arrivée comme faisant partie du chemin
    lab[x_courant][y_courant] = -2;

    while (!(x_courant == coord_x_depart && y_courant == coord_y_depart)) {
        bool voisin_trouve = false;

        // Vérifie les 4 voisins
        for (int d = 0; d < 4; d++) {
            int nx, ny;
            coord_case_voisine(x_courant, y_courant, d, &nx, &ny);

            // Si voisin a la distance précédente
            if (lab[nx][ny] == distance_actuelle - 1) {
                // Marque le voisin comme faisant partie du chemin
                lab[nx][ny] = -2;

                // Déplace la case courante
                x_courant = nx;
                y_courant = ny;
                distance_actuelle--;
                voisin_trouve = true;
                break;
            }
        }

        if (!voisin_trouve) {
            // Si aucun voisin à distance r-1 n'a été trouvé, la remontée échoue
            return false;
        }
    }

    return true;
}

// Nettoie le labyrinthe : toutes les cases non bloquées (-1) et qui ne font pas partie du chemin (-2) sont remises à 0
void nettoyerLabyrinthe(int lab[TAILLE_X][TAILLE_Y]) {
    for (int y = 0; y < TAILLE_Y; y++) {
        for (int x = 0; x < TAILLE_X; x++) {
            if (lab[x][y] > 0) {
                // Case visitée pendant l'expansion mais pas sur le chemin final
                lab[x][y] = 0;
            }
        }
    }
}
