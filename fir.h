#ifndef FIR_H
#define FIR_H

#include "define.h"
#include "fichiers.h"

//Strucuture contenant les variables de la fonction FIR et pour son initialisation

typedef struct FIR_variables FIR_variables;
struct FIR_variables{
    float FIR_TAPS[51];

    absorp absorp1;
    absorp abs_tmp;
    int etatVal;
    struct Queue* ACR;
    struct Queue* ACIR;

};

//<--------------------------------------------------------------------------------->


//List chainee : cellule + liste

typedef struct Cell Cell;
struct Cell{
    float value;
    struct Cell* next;
};

typedef struct List List;
struct List {
    unsigned int size;
    struct Cell* head;
};

//<--------------------------------------------------------------------------------->

//Fonctions pour les cellules et les listes

struct Cell* createCell(float val); // Creation de cellule

struct List* createList(); // Creation de liste

void addItem(struct List* l, float value, unsigned int position, int* valid); //ajoute une value dans une liste à partir d'une cellule

void deleteItem(struct List* l, unsigned int position, int* valid); //supprime une value (donc une cellule) de la liste

unsigned int getNbItems(struct List* l); //Avoir la taille de la liste (nombre de cellules)

float getItem(struct List* l, unsigned int position, int* valid); //Recuperer une valeur souhaité d'une cellule dans la liste


//<--------------------------------------------------------------------------------->

//Partie Queue : une file

typedef struct Queue Queue;
 struct Queue{
    struct List* liste;
};


struct Queue* emptyQueue(); // creation d'une file vide

void enqueue(Queue* q,float item, int* valid); // ajoute un element à la fin de la file

float dequeue(Queue* q, int* valid);// retire et renvoie element de tete

unsigned int queueSize(Queue* q);//renvoie le nombre d'element

int isQueueEmpty(Queue* q); // renvoie true si la file est vide, false sinon

float printValeurQueue(Queue* q, int position);//renvoie la n ieme valeur sans la supprimer --> juste un scrool

//<--------------------------------------------------------------------------------->

//Partie FIR

absorp firTest(char* filename);
FIR_variables* initialisationFIR(); // Initialiser les variables pour le filtrage
absorp FIR(absorp* myabsorpFIR, struct Queue* ACR, struct Queue* ACIR, float FIR_TAPS[]); // Fonction de filtrage FIR

#endif