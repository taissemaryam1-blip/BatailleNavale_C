#ifndef BATAILLE_NAVALE_H
#define BATAILLE_NAVALE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* Constantes */
#define Taille_Grille 10
#define Nbr_Bateau 5

/* Types de cases */
typedef enum {
    VIDE,
    BATEAU,
    TOUCHE,
    RATE
} caseGrille;

/* Orientation des bateaux */
typedef enum {
    Horizontal,
    Vertical
} Orientation;

/* Structure bateau */
typedef struct {
    char nom[50];
    int taille;
    int x, y;
    int orientation;       // Horizontal / Vertical
    int touche;
    int coule;
    int partiesTouchees[5]; // max 5 pour le plus grand bateau
    double tempsJoueur;   // temps total que ce joueur a passé à jouer (en secondes)
} bateau;


/* Structure joueur */
typedef struct {
    char nom[50];
    caseGrille grille[Taille_Grille][Taille_Grille];
    caseGrille grilleAttaque[Taille_Grille][Taille_Grille];
    bateau Bateaux[Nbr_Bateau];
    int bateaurestant;
    int nbBateauxCoules;

    // Compteurs pour statistiques
    int tirsTotal;
    int tirsTouches;
    int tirsRates;
    double tempsDeJeu; // Temps passé par le joueur en secondes
    double tempsJoueur;   // temps total que ce joueur a passé à jouer (en secondes)
} joueur;

/* Fonctions */
void initiergrille(caseGrille grille[Taille_Grille][Taille_Grille]);
void afficherGrille(caseGrille grille[Taille_Grille][Taille_Grille], int montrerBateaux);
void afficherdeuxgrille(joueur* joueur);
void initier_bateau(bateau Bateau[Nbr_Bateau]);
void Partie_Entiere();
void Partie_Simple();
void Setting_Color();
void regles();
int lireCoordAvecTimeout(char *coord, int secondes);
void afficherDeuxGrillesAttaque(joueur *j1, joueur *j2);
int trouverBateau(joueur *j, int ligne, int colonne);
int bateauCoule(bateau *b);
void attaque(joueur *attaquant, joueur *defenseur);
void phaseAttaque(joueur *j1, joueur *j2);
void marquerBateauCoule(joueur *attaquant, joueur *defenseur, bateau *b);
void Menu_fin(joueur *joueur1, joueur *joueur2, double tempsTotal);
void afficherStatistiques(joueur *j1, joueur *j2, double tempsTotal);
int lireCoordonnees(char *coord, int timeout);

#endif
