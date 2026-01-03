#include <stdio.h>
#include <math.h>
#include<stdlib.h>
typedef struct Client {
    int id ;
    float x ;
    float y ;
    int q ;
    struct Client*suivant;
};
typedef struct Client Client ;

struct Client * creerClient(int id, float x, float y, int q);
void afficher(Client * liste);
float distance(Client * c1 , Client * c2);
float distanceDepot(Client * c);
float distanceTotale(Client * liste);
Client * insertionClient(Client * liste, Client * c , int pos);
int meilleurePosition(Client * liste, Client * c);
Client * suprressionClient(Client * liste, int pos);
