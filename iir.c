
/*
 * @name : iir.c
 * @goal : filtrage passe-haut
 * @author : Emeric Guichet / Charles-Clément Gross CIR3
 * @notes : OK sur site tests
 *
 */

#include "iir.h"


absorp iirTest(char* filename){
    absorp myAbsorp;
    int etat = 0;

    //Initialisation de notre structure
    test* variable = (test*)malloc(1*sizeof(test));
    variable->startLed = 0; // x(n) pour ACR
    variable->ledMinusOne = 0; // x(n-1) pour ACR
    variable->ledMinusOneBis = 0; // y(n-1) pour ACR
    variable->startLedI = 0; // x(n) pour ACIR
    variable->ledIMinusOne = 0; // x(n-1) pour ACIR
    variable->ledIMinusOneBis = 0; // y(n-1) pour ACIR

    //Initialisation du fichier .fir
    FILE* firFichier = initFichier(filename);
    do{
        //Lecture du fichier .fir ligne par ligne par une variable Temporaire !!
        absorp tmpmyAbsorp = lireFichier(firFichier, &etat);
        //Condition sur les valeurs
        if(!(tmpmyAbsorp.acr == 0 && tmpmyAbsorp.dcr == 0 && tmpmyAbsorp.acir == 0 && tmpmyAbsorp.dcir == 0)){
            //Filtrage
            iir(&tmpmyAbsorp, variable);
            //Attribution a myAbsorp
            myAbsorp = tmpmyAbsorp;
        }


    } while (etat != EOF);
    //Liberation de l'espace de stockage du malloc
    free(variable);
    //Fermeture du fichier
    finFichier(firFichier);


    return myAbsorp;

}

void iir(absorp* myAbsorp, test* variables){

    //Variable alpha sur l'enonce
    float alpha = 0.992;

    //Formule
    variables->startLed = myAbsorp->acr - variables->ledMinusOne + (alpha*variables->ledMinusOneBis);
    //On applique le x(n-1) en recuperant la valeur du acr pour le prochain appel de la fonction
    variables->ledMinusOne =  myAbsorp->acr;
    //Idem pour le signal de sortie
    variables->ledMinusOneBis =  variables->startLed;
    myAbsorp->acr = variables->startLed;

    //Idem, meme raisonnement qu'au dessus
    variables->startLedI = myAbsorp->acir - variables->ledIMinusOne + (alpha* variables->ledIMinusOneBis);
    variables->ledIMinusOne =  myAbsorp->acir;
    variables->ledIMinusOneBis =  variables->startLedI;
    myAbsorp->acir = variables->startLedI;
}

