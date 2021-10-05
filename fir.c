/*
 * @name : fir.c
 * @goal : filtrage passe-bas
 * @author : Emeric Guichet / Charles-Clément Gross CIR3
 * @notes : OK sur site tests
 *
 */
#include "fir.h"


//<----------------------------- PARTIE LISTE CHAINEE ----------------------------------->

// Creation
struct Cell* createCell(float val){
    struct Cell *result = (struct Cell*)malloc(sizeof(struct Cell));
    result->value = val;
    result->next = NULL;
    return result;
}
struct List* createList(){
    struct List* list = (struct List*)malloc(sizeof(struct List));
    list->head = NULL;
    list->size = 0;
    return list;
}

//Ajout
void addItem(struct List* l, float value, unsigned int position, int* valid) {
    Cell *tmp_ptr;//contient la sauvegarde, c'est elle qui recupere l'adresse de la prochaine struct
    Cell *new_ptr;
    Cell  *ptr = l->head;
    //Verification si la position n'est pas errone
    if (position > l->size) {
        *valid = 0;
    }else{
        if (position == 0) { // Si position  == 0 on tente d'ajouter notre item en fonction des conditions
            if ((new_ptr = createCell(value)) == NULL) {
                *valid = 0;
            }else{
                if (ptr == NULL) {
                    l->head = new_ptr;
                    l->size++;
                    *valid = 1;
                } else {
                    tmp_ptr = l->head;
                    l->head = new_ptr;
                    l->head->next = tmp_ptr;
                    l->size++;
                    *valid = 1;
                }
            }
        }else {
            while ((position > 1) && ptr != NULL) {//On cherche où placer la nouvelle struct
                ptr = ptr->next;
                position--; // Le debut d'une liste chainne est la fin d'une file
            }
            if (ptr == NULL) {
                *valid = 0;
            }else{
                if ((new_ptr = createCell(value)) == NULL) {
                    *valid = 0;
                }else{
                    //on change les valeurs avec un SWAP avec des variables temporaires
                    tmp_ptr = ptr->next;
                    ptr->next = new_ptr;
                    ptr = ptr->next;
                    ptr->next = tmp_ptr;
                    l->size++; //Refresh la taille d'une liste
                    *valid = 1;
                }
            }
        }
    }
}

//Supression
void deleteItem(struct List* l, unsigned int position, int* valid){
    struct Cell* ptr = l->head;
    struct Cell* q = NULL;

    if(l->head == NULL) {
        *valid = 0;
    }else{
        if (position == 0) {
            l->head = l->head ->next;
            free(ptr);
            *valid = 1;
        } else {
            while ((position > 1) && ptr != NULL) {//On cherche ou placer la nouvelle struct
                ptr = ptr->next;
                position--; // Le debut d'une liste chainne est la fin d'une file
            }
            if(ptr == NULL)
            {
                *valid = 0;
            }else {
                q = ptr->next;
                ptr->next = ptr->next->next;
                free(q); //Utilisation d'un malloc donc liberation lors d'une supression
                *valid = 1;
            }
        }
        l->size = getNbItems(l); // Refresh la taille d'une liste
    }
}

unsigned int getNbItems(struct List* l){
    unsigned int i = 0;
    Cell* tmp;
    if(l->head != NULL){
        tmp  = l->head;
        // On compte le nombre de cellules jusqu'a la valeure null
        while (tmp -> next != NULL) {
            i++;
            tmp = tmp -> next;
        }
        i++;
    }
    return i;
}

float getItem(struct List* l, unsigned int position, int* valid){
    float valuePosition = 0;
    unsigned int i = 0;
    Cell* ptr;

    if(l->head == NULL){
        *valid = 0;
        return 0;
    }
    ptr = l->head;

    //On s'arrete à la position de l'Item
    for(i = 0;i<=position;i++){
        if(ptr ==NULL)
        {
            *valid = 0;
            return -1;
        }else{
            valuePosition = ptr->value;
            ptr = ptr->next;
        }
    }
    *valid = 1;
    return valuePosition;
}


//<----------------------------- PARTIE QUEUE ------------------------------------------->

struct Queue* emptyQueue(){
    //On ajoute une liste vide dans notre file
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->liste = createList();
    return q;
}

void enqueue(Queue* q, float item, int* valid){
    if(q == NULL){
        *valid = 0;
    }else{
        //On appelle addItem pour ajouter une valeur dans la file
        addItem(q->liste,item, 0,valid);
        *valid = 1;
    }

}

float dequeue(Queue* q, int* valid){
    unsigned int taille = 0;
    float valueHead = 0;
    int i = 0;

    //Verification qu'on retire un element ou pas
    if(isQueueEmpty(q)== 1)
    {
        *valid = 0;
        return 0;
    }else {
        //Supprimer un element d'une file correspond a enlever le dernier element
        //Donc il nous faut la taille de la file pour se deplacer jusqu'a la derniere cellule
        taille = queueSize(q);
        valueHead = getItem(q->liste, taille - 1, valid);
        struct Cell *ptr = q->liste->head;
        if (taille <= 1) {
        }else {
            for (i = 0; i <= taille - 2; i++) {
                ptr = ptr->next;
            }
        }
        //Suppression de l'element par la fonction deleteItem
        deleteItem(q->liste, taille - 1, valid);
        ptr->next = NULL;
        *valid =1;
        return valueHead;
    }
}

unsigned int queueSize(Queue* q){
    //File commence a 0
    unsigned int valueSize = 0;
    if(q->liste != NULL){
        //Recuperation de la taille de notre file = taille de notre liste
        valueSize = getNbItems(q->liste);
    }
    return valueSize;
}

int isQueueEmpty(Queue* q){
    //Si il y a plus d'un element alors la file n'est pas vide
    unsigned int valueSize = 0;
    valueSize = getNbItems(q->liste);
    if(valueSize==0)
    {
        return 1;
    }else{
        return 0;
    }
}

float printValeurQueue(Queue* q, int position){
    int valid = 0;
    float value = 0;
    unsigned  int size = getNbItems(q->liste);
    int i =0;
    for(i = 0; i<size;i++) {
        value = getItem(q->liste, i, &valid);
        if( i == position)
        {
            return value;
        }
    }
    return 0;
}




//<----------------------------- PARTIE FIR -------------------------------------------->


FIR_variables* initialisationFIR(){
    FIR_variables* variablesFIR = (FIR_variables*)malloc(sizeof(FIR_variables));
    //Tableau des valeurs des coefficients de notre filtre de la formule qui permet de calculer le FIR
    float tab[51] ={
            1.4774946e-019,
            1.6465231e-004,
            3.8503956e-004,
            7.0848037e-004,
            1.1840522e-003,
            1.8598621e-003,
            2.7802151e-003,
            3.9828263e-003,
            5.4962252e-003,
            7.3374938e-003,
            9.5104679e-003,
            1.2004510e-002,
            1.4793934e-002,
            1.7838135e-002,
            2.1082435e-002,
            2.4459630e-002,
            2.7892178e-002,
            3.1294938e-002,
            3.4578348e-002,
            3.7651889e-002,
            4.0427695e-002,
            4.2824111e-002,
            4.4769071e-002,
            4.6203098e-002,
            4.7081811e-002,
            4.7377805e-002,
            4.7081811e-002,
            4.6203098e-002,
            4.4769071e-002,
            4.2824111e-002,
            4.0427695e-002,
            3.7651889e-002,
            3.4578348e-002,
            3.1294938e-002,
            2.7892178e-002,
            2.4459630e-002,
            2.1082435e-002,
            1.7838135e-002,
            1.4793934e-002,
            1.2004510e-002,
            9.5104679e-003,
            7.3374938e-003,
            5.4962252e-003,
            3.9828263e-003,
            2.7802151e-003,
            1.8598621e-003,
            1.1840522e-003,
            7.0848037e-004,
            3.8503956e-004,
            1.6465231e-004,
            1.4774946e-019
    };
    int i = 0;
    for(i = 0; i<50;i++){//on remplit le tableau des FIR_TAPS
        variablesFIR->FIR_TAPS[i] = tab[i];
    }
    // On initialise les 2 files
    variablesFIR->etatVal = 0;
    variablesFIR->ACR = emptyQueue();
    variablesFIR->ACIR =emptyQueue();

    return variablesFIR;
}

absorp firTest(char* filename){
    FIR_variables* variablesFIR = initialisationFIR();
    FILE* file = NULL;

    file = initFichier(filename);
    int etat = EOF;
    if(file!=NULL){

        do{
            variablesFIR->abs_tmp = lireFichier(file, &etat);
            if(etat == EOF){
                //on place absorp dans une variable temporaire pour safe notre derniere valeur
                //si on arrive a la fin du fichier
                //pour ne pas avoir de calcul pour 0
                printf("Derniere ligne");
            }else{
                variablesFIR-> absorp1 = variablesFIR->abs_tmp;
                /*printf("ACR: %f\n", variablesFIR->absorp1.acr);
                printf("DCR: %f\n", variablesFIR->absorp1.dcr);
                printf("ACIR: %f\n", variablesFIR->absorp1.acir);
                printf("DCIR: %f\n", variablesFIR->absorp1.dcir);*/
                variablesFIR->absorp1 = FIR(&variablesFIR->absorp1, variablesFIR->ACR, variablesFIR->ACIR, variablesFIR->FIR_TAPS);
                /*printf("-----------------\n");
                printf("ACR: %f\n", variablesFIR->absorp1.acr);
                printf("DCR: %f\n", variablesFIR->absorp1.dcr);
                printf("ACIR: %f\n", variablesFIR->absorp1.acir);
                printf("DCIR: %f\n", variablesFIR->absorp1.dcir);
                printf(":::::::::::::::::\n");*/
            }
        }while (etat !=EOF);
        fclose(file);
        //Pour vider nos queue a la fin de la lecture du fichier pour ne pas perdre de memoire

        while (isQueueEmpty(variablesFIR->ACR) == 0){
            dequeue(variablesFIR->ACR, &variablesFIR->etatVal);
        }
        while (isQueueEmpty(variablesFIR->ACIR) == 0){
            dequeue(variablesFIR->ACIR, &variablesFIR->etatVal);
        }
        //retourne le dernier absorp filtré

    }
    return variablesFIR->absorp1;

}

absorp FIR(absorp* myabsorpFIR, struct Queue* ACR, struct Queue* ACIR, float FIR_TAPS[]){
    //initialisation
    int i =0;
    int validACR = 0;
    float valRetourACR = 0;
    float x = 0;
    int tailleQueueACR = 0;


    int validACIR = 0;
    float valRetourACIR = 0;
    int tailleQueueACIR = 0;


    //Partie 1 - ACR
    //ajout d'une valeur de plus dans la file
    enqueue(ACR, myabsorpFIR->acr, &validACR);
    if(validACR == 1){

        //si nos files ne sont pas vides
        tailleQueueACR = queueSize(ACR);
        for(i=0; i<tailleQueueACR;i++){// On va de x[n] a x[0]
            //mais comme l'odre dans la file est inversé, on commence a 0
            x = printValeurQueue(ACR, i);
            valRetourACR +=  FIR_TAPS[i]* (float)x;
        }
        //Si on a plus de 50 valeurs dans la file, on supprime celle de tete
        if (queueSize(ACR) > 50){
            dequeue(ACR, &validACR);
        }
        //Puis on applique notre valeur d'ACR dans notre structure myAbsorp
        myabsorpFIR->acr = valRetourACR;
    }

    //Partie 2 - ACIR
    enqueue(ACIR, myabsorpFIR->acir, &validACIR);
    if(validACIR == 1){
        //si nos files ne sont pas vides

        tailleQueueACIR = queueSize(ACIR);
        for(i =0; i<tailleQueueACIR;i++){// On va de x[n] a x[0]
            //mais comme l'odre dans la file est inversé, on commence a 0
            x = printValeurQueue(ACIR, i);
            valRetourACIR +=  FIR_TAPS[i]*x;
        }

        //Si on a plus de 50 valeurs dans la file, on supprime celle de tete
        if (queueSize(ACIR) > 50){
            dequeue(ACIR, &validACIR);
        }
        //Puis on applique notre valeur d'ACIR dans notre structure myAbsorp
        myabsorpFIR->acir = valRetourACIR;
    }
    return *myabsorpFIR;
}