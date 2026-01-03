#include "bataille_naval_h.h"
#include <stdio.h>
#include <stdlib.h>

int Menu(){
int choix ;
while (1) {
printf("            Bienvenue dans le jeu de la baille navale \n\n");
printf("tapez 0 si vous souhaitez Quitter le jeu\n");
printf("tapez 1 si vous souhaitez jouer une Partie Simple joueur contre joueur\n");
printf("tapez 2 si vous souhaitez jouer une Partie Complete joueur contre joueur\n");
printf("tapez 3 si vous souhaitez changer les parmetres de couleurs\n");
printf("tapez 4 si vous souhaitez regarder les regles\n");
    scanf("%d", &choix);
    if(choix ==0){return 0;}   // Quitter le jeu
    else if(choix ==1){system("cls");Partie_Simple();}       // Partie simple
    else if(choix ==2){system("cls");Partie_Entiere();}      // Partie complète
    else if(choix ==3){system("cls");Setting_Color();}   // changer les couleurs
    else if (choix==4){system ("cls");regles();}         // Afficher les règles
    else {}
   // Menu();
}

return 0;
}




//TM menu fin de jeu
void Menu_fin(joueur *joueur1, joueur *joueur2, double tempsTotal) {
    int choix;

    while (1) {
        printf("\n=== MENU DE FIN DE PARTIE ===\n");
        printf("1. Rejouer une nouvelle partie\n");
        printf("2. Voir les statistiques de la partie actuelle\n");
        printf("3. Retourner au menu principal\n");
        printf("4. Quitter le jeu\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                #endif
                printf("\n=== Nouvelle Partie ===\n\n");
                Partie_Entiere();  // Lancer une nouvelle partie
                return;

            case 2:
                #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                #endif
                printf("\n=== Statistiques de la Partie ===\n");
                afficherStatistiques(joueur1, joueur2, tempsTotal); // Passer le temps total
                break;

            case 3:
                #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                #endif
                printf("\nRetour au menu principal...\n\n");
                Menu();  // Appel du menu principal
                return;

            case 4:
                printf("Merci d'avoir joué ! Au revoir.\n");
                exit(0);

            default:
                printf("Choix invalide ! Veuillez réessayer.\n");
        }
    }
}
