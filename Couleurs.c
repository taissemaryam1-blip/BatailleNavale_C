#include <stdio.h>
#include <windows.h>

void Setting_Color() {
    int choix;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    printf("Choisissez une couleur :\n");
    printf("1 - Rouge\n");
    printf("2 - Vert\n");
    printf("3 - Bleu\n");
    printf("4 - Jaune\n");
    printf("5 - Violet\n");
    printf("6 - Cyan\n");
    printf("Votre choix : ");
    scanf("%d", &choix);

    switch (choix) {
        case 1: SetConsoleTextAttribute(hConsole, 12); break;
        case 2: SetConsoleTextAttribute(hConsole, 10); break;
        case 3: SetConsoleTextAttribute(hConsole, 9);  break;
        case 4: SetConsoleTextAttribute(hConsole, 14); break;
        case 5: SetConsoleTextAttribute(hConsole, 13); break;
        case 6: SetConsoleTextAttribute(hConsole, 11); break;
        default:
            printf("Choix invalide !\n");
    }

    system("cls");
}
