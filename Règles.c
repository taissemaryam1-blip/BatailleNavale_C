#include <stdio.h>
#include <stdlib.h>

void regles() {
    system("cls");  // Efface l'écran

    printf("===================================== REGLES DE LA BATAILLE NAVALE ==================================\n\n");

    printf("1. Objectif : Couler tous les bateaux de l'adversaire avant qu'il ne coule les votres.\n\n");

    printf("---------------------------------------------------------------------------\n");
    printf("2. ------------------------------Plateaux :--------------------------------\n");
    printf("---------------------------------------------------------------------------\n");
    printf("   - Grille de 10x10 cases (lignes A-J, colonnes 1-10)\n");
    printf("   - Chaque joueur possede sa grille et sa grille d'attaque\n\n");

    printf("---------------------------------------------------------------------------\n");
    printf("3. ---------------------------Bateaux a placer :---------------------------\n");
    printf("---------------------------------------------------------------------------\n");
    printf("   - Porte-avions (5 cases)\n");
    printf("   - Croiseur (4 cases)\n");
    printf("   - Contre-torpilleur (3 cases)\n");
    printf("   - Sous-marin (3 cases)\n");
    printf("   - Torpilleur (2 cases)\n\n");

    printf("---------------------------------------------------------------------------\n");
    printf("4. -------------------------Placement des bateaux :------------------------\n");
    printf("---------------------------------------------------------------------------\n");
    printf("   - Horizontal ou vertical\n");
    printf("   - Pas en diagonale\n");
    printf("   - Les bateaux ne doivent pas se toucher\n\n");

    printf("---------------------------------------------------------------------------\n");
    printf("5. ------------------------Deroulement d'un tour :-------------------------\n");
    printf("---------------------------------------------------------------------------\n");
    printf("   - Un joueur annonce une coordonnee (ex. B7)\n");
    printf("   - L'adversaire repond : Touche, Rate ou Coule\n");
    printf("   - La grille d'attaque est mise a jour\n\n");

    printf("---------------------------------------------------------------------------\n");
    printf("6. ---------------------------Fin de la partie :---------------------------\n");
    printf("---------------------------------------------------------------------------\n");
    printf("   - Le jeu se termine quand tous les bateaux d'un joueur sont coules\n");
    printf("   - L'autre joueur est declare vainqueur\n\n");

    printf("Appuyez sur Entree pour revenir au menu...");
    getchar(); // Pour consommer le retour chariot
    getchar(); // Pour attendre l'appui sur Entrée
    system("cls");  // Retour au menu
}
