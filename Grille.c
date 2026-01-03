#include "bataille_naval_h.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>   // pour _kbhit() et _getch()


void initiergrille(caseGrille grille[Taille_Grille][Taille_Grille]){
   for (int i=0; i<Taille_Grille;i++){
    for (int j=0; j<Taille_Grille;j++)grille[i][j]=VIDE;
   }
}

void afficherGrille(caseGrille grille[Taille_Grille][Taille_Grille], int montrerBateaux) {
    printf("\n    ");
    for (int j = 0; j < Taille_Grille; j++) {
        printf(" %d ", j + 1);
    }
    printf("\n");

    for (int i = 0; i < Taille_Grille; i++) {
        printf("  %c ", 'A' + i);

        for (int j = 0; j < Taille_Grille; j++) {
            switch(grille[i][j]) {
                case VIDE:   printf(" ~ "); break;
                case BATEAU: printf(montrerBateaux ? " B " : " ~ "); break;
                case TOUCHE: printf(" X "); break;
                case RATE:   printf(" O "); break;
                default:
                    if (grille[i][j] == 'C') printf(" C ");
                else printf(" ~ ");
        break;
}
        }
        printf("\n");
    }
    printf("\n");
}

void afficherdeuxgrille(joueur*joueur){
    printf("\n--------------%s------------\n",joueur->nom);
    printf("Votre Grille :");
    afficherGrille(joueur->grille,1);
    printf("Vos Attaques: ");
    afficherGrille(joueur->grilleAttaque,0);
}

void Initialisation_Nom(joueur *joueur1, joueur *joueur2) {
    if (joueur1 == NULL) {
        printf("ERREUR CRITIQUE: joueur1 est NULL!\n");
        return;
    }
    if (joueur2 == NULL) {
        printf("ERREUR CRITIQUE: joueur2 est NULL!\n");
        return;
    }

    printf("=== INITIALISATION NOM DES JOUEURS ===\n\n");

    printf("Entrez le nom de joueur 1: ");
    fflush(stdout);
    scanf("%49s", joueur1->nom);

    printf("Entrez le nom de joueur 2: ");
    fflush(stdout);
    scanf("%49s", joueur2->nom);
}

int estPlacementValide(joueur *joueur, int taille, int ligne, int colonne, int orientation) {
    // Vérifier les limites
    if (ligne < 0 || ligne >= Taille_Grille || colonne < 0 || colonne >= Taille_Grille) {
        return 0;
    }

    if (orientation == 0) { // Horizontal
        if (colonne + taille > Taille_Grille) return 0;
        for (int i = 0; i < taille; i++) {
            if (joueur->grille[ligne][colonne + i] != VIDE) {
                return 0;
            }
        }
    } else { // Vertical
        if (ligne + taille > Taille_Grille) return 0;
        for (int i = 0; i < taille; i++) {
            if (joueur->grille[ligne + i][colonne] != VIDE) {
                return 0;
            }
        }
    }

    // Vérifier qu'il n'y a pas de bateau adjacent
    int debutL = (ligne > 0) ? ligne - 1 : 0;
    int finL = (orientation == 1) ? ligne + taille : ligne + 1;
    int debutC = (colonne > 0) ? colonne - 1 : 0;
    int finC = (orientation == 0) ? colonne + taille : colonne + 1;

    if (finL >= Taille_Grille) finL = Taille_Grille - 1;
    if (finC >= Taille_Grille) finC = Taille_Grille - 1;

    for (int i = debutL; i <= finL; i++) {
        for (int j = debutC; j <= finC; j++) {
            if (joueur->grille[i][j] == BATEAU) {
                return 0;
            }
        }
    }

    return 1;
}

int placerBateau(joueur *joueur, int indexBateau, int taille, int ligne, int colonne, int orientation) {
    if (!estPlacementValide(joueur, taille, ligne, colonne, orientation)) {
        return 0;
    }

    // Mettre à jour la grille
    if (orientation == 0) { // Horizontal
        for (int i = 0; i < taille; i++) {
            joueur->grille[ligne][colonne + i] = BATEAU;
        }
    } else { // Vertical
        for (int i = 0; i < taille; i++) {
            joueur->grille[ligne + i][colonne] = BATEAU;
        }
    }

    // Initialiser la structure du bateau
    joueur->Bateaux[indexBateau].taille = taille;
    joueur->Bateaux[indexBateau].x = ligne;
    joueur->Bateaux[indexBateau].y = colonne;
    joueur->Bateaux[indexBateau].orientation = orientation;
    joueur->Bateaux[indexBateau].coule = 0;

    // Initialiser le tableau des parties touchées
    for (int i = 0; i < taille; i++) {
        joueur->Bateaux[indexBateau].partiesTouchees[i] = 0;
    }

    return 1; // CORRECTION : Ajouter le return manquant
}

void initialiserBateauxManuel(joueur *joueur) {
    int tailles[Nbr_Bateau] = {5, 4, 3, 3, 2};
    char *noms[Nbr_Bateau] = {
        "Porte-avion (5 cases)",
        "Croiseur (4 cases)",
        "Contre-torpilleur (3 cases)",
        "Sous-marin (3 cases)",
        "Torpilleur (2 cases)"
    };

    printf("\n=== Placement des bateaux pour %s ===\n", joueur->nom);
    printf("Orientation : H = Horizontal, V = Vertical\n");
    printf("Coordonnees : ex. A1, B5, etc.\n\n");

    for (int i = 0; i < Nbr_Bateau; i++) {
        int place = 0;

        while (!place) {
            printf("Placez votre %s\n", noms[i]);
            afficherGrille(joueur->grille, 1);

            char coord[10];
            char orientation;
            int ligne, colonne;

            // Saisie des coordonnées
            printf("Entrez les coordonnees de debut (ex. A1): ");
            scanf("%s", coord);

            // Convertir la lettre
            ligne = toupper(coord[0]) - 'A';

            // Convertir les chiffres
            colonne = 0;
            int j = 1;
            while (coord[j] != '\0' && isdigit(coord[j])) {
                colonne = colonne * 10 + (coord[j] - '0');
                j++;
            }
            colonne--; // Convertir de 1-indexé à 0-indexé

            // Saisie de l'orientation
            char orientationChar;
            //char orientation;

            printf("Orientation (H/V): ");
            scanf(" %c", &orientationChar);
            orientationChar = toupper(orientationChar);
            orientation = (orientationChar == 'H') ? Horizontal : Vertical;

            // Essayer de placer le bateau
            place = placerBateau(joueur, i, tailles[i], ligne, colonne, orientation);

            if (!place) {
                printf("Placement invalide ! Reessayez.\n\n");
            }
        }

        printf("Bateau place avec succes !\n\n");
    }

    printf("Tous les bateaux ont ete places avec succes !\n");
}

void initialiserBateauxAuto(joueur *joueur) {
    int tailles[Nbr_Bateau] = {5, 4, 3, 3, 2};

    for (int i = 0; i < Nbr_Bateau; i++) {
        int place = 0;
        int tentatives = 0;

        while (!place && tentatives < 100) {
            int ligne = rand() % Taille_Grille;
            int colonne = rand() % Taille_Grille;
            int orientation = rand() % 2;

            place = placerBateau(joueur, i, tailles[i], ligne, colonne, orientation);
            tentatives++;
        }

        if (!place) {
            printf("Erreur : impossible de placer le bateau de taille %d\n", tailles[i]);
        }
    }
}

void initialiserBateauxJoueur(joueur *joueur, int avecSaisie) {
    // Initialiser le compteur de bateaux coulés
    joueur->nbBateauxCoules = 0;

    // Initialiser toutes les cases de la grille à VIDE
    for (int i = 0; i < Taille_Grille; i++) {
        for (int j = 0; j < Taille_Grille; j++) {
            joueur->grille[i][j] = VIDE;
            joueur->grilleAttaque[i][j] = VIDE;
        }
    }

    if (avecSaisie) {
        initialiserBateauxManuel(joueur);
    } else {
        initialiserBateauxAuto(joueur);
    }
}

void initialisation_Partie(joueur *joueur1, joueur *joueur2) {
    printf("=== INITIALISATION BATAILLE NAVALE ===\n\n");

    // Joueur 1 - placement manuel
    printf("Joueur 1 - Placement des bateaux :\n");
    initialiserBateauxJoueur(joueur1, 1);

    printf("\nAppuyez sur Entree pour passer au joueur 2...");
    getchar(); // Pour consommer le retour chariot restant de la dernière saisie
    getchar(); // Pour attendre l'appui sur Entrée

    // Joueur 2 - placement automatique  ( mauelle 0--> 1)
    printf("\nJoueur 2 - Placement automatique des bateaux...\n");
    initialiserBateauxJoueur(joueur2, 1);

    // Afficher les grilles (décommenter pour tester)
    //printf("\n=== Grille de %s ===\n", joueur1->nom);
    //afficherdeuxgrille(joueur1);
}

void Partie_Entiere() {
    // Initialiser le générateur aléatoire
    srand(time(NULL));

    joueur *joueur1 = malloc(sizeof(joueur));
    joueur *joueur2 = malloc(sizeof(joueur));

    if (!joueur1 || !joueur2) {
        printf("Erreur d'allocation mémoire!\n");
        return;
    }

    Initialisation_Nom(joueur1, joueur2);
    initialisation_Partie(joueur1, joueur2);

    // Initialiser les statistiques de temps
    joueur1->tempsJoueur = 0;
    joueur2->tempsJoueur = 0;
    // Début de la partie (pour le calcul du temps total)
    clock_t debutPartie = clock();

        // Phase attaque
    joueur *attaquant = joueur1;
    joueur *defenseur = joueur2;

    while (defenseur->nbBateauxCoules < Nbr_Bateau) {
        clock_t debutTour = clock();

        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        afficherDeuxGrillesAttaque(joueur1, joueur2);
        printf("Tour de %s\n\n", attaquant->nom);

        // Attaque du joueur
        attaque(attaquant, defenseur);

        // Calcul du temps du joueur
        clock_t finTour = clock();
        attaquant->tempsJoueur += (double)(finTour - debutTour) / CLOCKS_PER_SEC;

        // Si fin de partie
        if (defenseur->nbBateauxCoules == Nbr_Bateau) break;

        printf("\nAppuyez sur Entrée pour continuer...");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        getchar();

        // Changement de joueur
        joueur *tmp = attaquant;
        attaquant = defenseur;
        defenseur = tmp;
    }

    // Temps total de la partie
    clock_t finPartie = clock();
    double tempsTotal = (double)(finPartie - debutPartie) / CLOCKS_PER_SEC;

    // Menu de fin
    Menu_fin(joueur1, joueur2, tempsTotal);

    free(joueur1);
    free(joueur2);
}

void Partie_Simple() {
    srand(time(NULL));

    // Créer les joueurs
    joueur *joueur1 = malloc(sizeof(joueur));
    joueur *joueur2 = malloc(sizeof(joueur));

    if (!joueur1 || !joueur2) {
        printf("Erreur d'allocation mémoire!\n");
        return;
    }

    // Initialiser les noms
    Initialisation_Nom(joueur1, joueur2);

    // Initialiser les grilles et les bateaux automatiquement
    for (int i = 0; i < Taille_Grille; i++) {
        for (int j = 0; j < Taille_Grille; j++) {
            joueur1->grille[i][j] = VIDE;
            joueur1->grilleAttaque[i][j] = VIDE;
            joueur2->grille[i][j] = VIDE;
            joueur2->grilleAttaque[i][j] = VIDE;
        }
    }

    initialiserBateauxJoueur(joueur1, 0); // 0 = placement automatique
    initialiserBateauxJoueur(joueur2, 0); // 0 = placement automatique

    // Initialiser les compteurs et temps
    joueur1->nbBateauxCoules = 0;
    joueur2->nbBateauxCoules = 0;
    joueur1->tirsTotal = joueur1->tirsTouches = joueur1->tirsRates = 0;
    joueur2->tirsTotal = joueur2->tirsTouches = joueur2->tirsRates = 0;
    joueur1->tempsDeJeu = joueur2->tempsDeJeu = 0;

    // Boucle d'attaque jusqu'à ce qu'un joueur ait tous ses bateaux coulés
    phaseAttaque(joueur1, joueur2);

    // Libérer la mémoire
    free(joueur1);
    free(joueur2);
}


// attaque

int lireCoordonnees(char *coord, int timeout) {
    clock_t start = clock();
    int i = 0;
    char c;

    printf("Entrez une coordonnee (ex: B7) : ");
    fflush(stdout);

    while ((clock() - start) * 1.0 / CLOCKS_PER_SEC < timeout) {
        if (_kbhit()) {
            c = _getch();
            if (c == '\r') { // Entrée
                coord[i] = '\0';
                printf("\n");
                return 1; // succès
            } else {
                coord[i++] = c;
                printf("%c", c); // afficher ce que tape le joueur
            }
        }
    }
    printf("\nTemps écoulé !\n");
    return 0; // timeout
}

void afficherDeuxGrillesAttaque(joueur *j1, joueur *j2) {
    printf("\n        Grille d'attaque %s           Grille d'attaque %s\n\n", j1->nom, j2->nom);

    // En-tête des colonnes
    printf("   ");
    for (int j = 0; j < Taille_Grille; j++)
        printf(" %d ", j + 1);
    printf("        ");
    for (int j = 0; j < Taille_Grille; j++)
        printf(" %d ", j + 1);
    printf("\n");

    for (int i = 0; i < Taille_Grille; i++) {
        // Grille du joueur 1
        printf("  %c ", 'A' + i);
        for (int j = 0; j < Taille_Grille; j++) {
            switch (j1->grilleAttaque[i][j]) {
                case VIDE:   printf(" ~ "); break;
                case TOUCHE: printf(" X "); break;
                case RATE:   printf(" O "); break;
                case 'C':   printf(" C "); break;
                default:     printf(" ~ "); break;
            }
        }

        // Espacement entre les grilles
        printf("     ");

        // Grille du joueur 2
        printf(" %c ", 'A' + i);
        for (int j = 0; j < Taille_Grille; j++) {
            switch (j2->grilleAttaque[i][j]) {
                case VIDE:   printf(" ~ "); break;
                case TOUCHE: printf(" X "); break;
                case RATE:   printf(" O "); break;
                case 'C':   printf(" C "); break;
                default:     printf(" ~ "); break;
            }
        }

        printf("\n");
    }
    printf("\n");
}

int trouverBateau(joueur *j, int ligne, int colonne) {
    for (int b = 0; b < Nbr_Bateau; b++) {
        bateau *bat = &j->Bateaux[b];

        for (int i = 0; i < bat->taille; i++) {
            int x = bat->x + (bat->orientation == Vertical ? i : 0);
            int y = bat->y + (bat->orientation == Horizontal ? i : 0);

            if (x == ligne && y == colonne)
                return b;
        }
    }
    return -1;
}

int bateauCoule(bateau *b) {
    for (int i = 0; i < b->taille; i++) {
        if (b->partiesTouchees[i] == 0)
            return 0;
    }
    return 1;
}

void attaque(joueur *attaquant, joueur *defenseur) {
    char coord[10];
    int ligne, colonne;

    while (1) {
        if (!lireCoordonnees(coord, 10)) {
        // Si le joueur n'a pas tapé à temps
        printf("%s a depasse le temps ! Tour passe.\n", attaquant->nom);
        return; // on passe au joueur suivant
}
        ligne = toupper(coord[0]) - 'A';
        colonne = atoi(&coord[1]) - 1;

        if (ligne < 0 || ligne >= Taille_Grille ||
            colonne < 0 || colonne >= Taille_Grille) {
            printf("Coordonnees invalides !\n");
            continue;
        }

        if (attaquant->grilleAttaque[ligne][colonne] != VIDE) {
            printf("Vous avez deja tire ici !\n");
            continue;
        }
        break;
    }

    // 1) Compter ce tir
    attaquant->tirsTotal++;

    /* === TOUCHE === */
    if (defenseur->grille[ligne][colonne] == BATEAU) {
        printf("TOUCHE !\n");

        defenseur->grille[ligne][colonne] = TOUCHE;
        attaquant->grilleAttaque[ligne][colonne] = TOUCHE;

        // Tir touché
        attaquant->tirsTouches++;

        int indexBateau = trouverBateau(defenseur, ligne, colonne);

        if (indexBateau != -1) {
            bateau *b = &defenseur->Bateaux[indexBateau];

            int indexPartie =
                (b->orientation == Horizontal) ? colonne - b->y : ligne - b->x;

            b->partiesTouchees[indexPartie] = 1;

            if (!b->coule && bateauCoule(b)) {
                b->coule = 1;
                defenseur->nbBateauxCoules++;
                printf(">>> BOOOOOM! BATEAU COULE !!! <<<\n");

                // Marquer toutes les cases du bateau comme 'C'
                marquerBateauCoule(attaquant, defenseur, b);
            }
        }
    }
    /* === RATE === */
    else {
        printf("RATE !\n");
        defenseur->grille[ligne][colonne] = RATE;
        attaquant->grilleAttaque[ligne][colonne] = RATE;

        // Tir raté
        attaquant->tirsRates++;
    }
}

void marquerBateauCoule(joueur *attaquant, joueur *defenseur, bateau *b) {
    for (int i = 0; i < b->taille; i++) {
        int x = b->x + (b->orientation == Vertical ? i : 0);
        int y = b->y + (b->orientation == Horizontal ? i : 0);
        attaquant->grilleAttaque[x][y] = 'C';
    }
}

//void phaseAttaque(joueur *j1, joueur *j2) {
//    joueur *attaquant = j1;
//    joueur *defenseur = j2;
//
//    while (defenseur->nbBateauxCoules < Nbr_Bateau) {
//        #ifdef _WIN32
//            system("cls");
//        #else
//            system("clear");
//        #endif
//
//        afficherDeuxGrillesAttaque(j1, j2);
//        printf("Tour de %s\n\n", attaquant->nom);
//        attaque(attaquant, defenseur);
//
//        // Si après l'attaque tous les bateaux de défenseur sont coulés, on termine
//        if (defenseur->nbBateauxCoules == Nbr_Bateau) {
//            printf("\nTous les bateaux de %s ont ete coules !\n", defenseur->nom);
//            break; // fin de la boucle
//        }
//
//        printf("\nAppuyez sur Entree pour continuer...");
//        int c;
//        while ((c = getchar()) != '\n' && c != EOF);
//        getchar();
//
//        // Changer de joueur seulement si la partie n'est pas finie
//        joueur *tmp = attaquant;
//        attaquant = defenseur;
//        defenseur = tmp;
//    }
//
//    // Une fois tous les bateaux coulés
//    Menu_fin(j1, j2);
//}

void phaseAttaque(joueur *j1, joueur *j2) {
    joueur *attaquant = j1;
    joueur *defenseur = j2;

    clock_t debutPartie = clock(); // début du chrono global

    while (defenseur->nbBateauxCoules < Nbr_Bateau) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        afficherDeuxGrillesAttaque(j1, j2);
        printf("Tour de %s\n\n", attaquant->nom);

        clock_t debutTour = clock();  // début du tour du joueur
        attaque(attaquant, defenseur);
        clock_t finTour = clock();    // fin du tour

        // Ajouter le temps passé par le joueur
        attaquant->tempsJoueur += (double)(finTour - debutTour) / CLOCKS_PER_SEC;

        // Vérifier fin de partie
        if (defenseur->nbBateauxCoules == Nbr_Bateau) {
            printf("\nTous les bateaux de %s ont ete coules !\n", defenseur->nom);
            break;
        }

        printf("\nAppuyez sur Entree pour continuer...");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        getchar();

        // Changer de joueur
        joueur *tmp = attaquant;
        attaquant = defenseur;
        defenseur = tmp;
    }

    // Temps total de la partie
    clock_t finPartie = clock();
    double tempsTotal = (double)(finPartie - debutPartie) / CLOCKS_PER_SEC;

    afficherStatistiques(j1, j2, tempsTotal);  // passer le temps total
    Menu_fin(j1, j2, tempsTotal);
}
void afficherStatistiques(joueur *j1, joueur *j2, double tempsTotal) {
    joueur* joueurs[2] = {j1, j2};

    printf("\n=== Temps total de la partie : %.2f secondes ===\n", tempsTotal);

    for (int i = 0; i < 2; i++) {
        joueur *j = joueurs[i];
        printf("\n=== Statistiques de %s ===\n", j->nom);
        printf("Temps de jeu de %s : %.2f secondes\n", j->nom, j->tempsJoueur);
        printf("Tirs total : %d\n", j->tirsTotal);
        printf("Tirs touches : %d (%.2f%%)\n", j->tirsTouches,
               j->tirsTotal ? (j->tirsTouches * 100.0 / j->tirsTotal) : 0);
        printf("Tirs rates : %d (%.2f%%)\n", j->tirsRates,
               j->tirsTotal ? (j->tirsRates * 100.0 / j->tirsTotal) : 0);
        printf("Bateaux coules de l'adversaire : %d/%d\n", j->nbBateauxCoules, Nbr_Bateau);
    }
}

