#ifndef _LECTURE_H  
#define _LECTURE_H

extern int coord_x_depart;
extern int coord_y_depart;
extern int coord_x_arrivee;
extern int coord_y_arrivee;

/* pré-construire le labyrinthe en récupérant les données depuis un fichier */
int build_labyrinthe(const char *nom_fichier, int coordonnees_labyrinthe[TAILLE_X][TAILLE_Y]);

#endif