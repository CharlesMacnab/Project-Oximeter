/*
 * @name : integration.c
 * @goal : filtrage passe-bas
 * @author : Emeric Guichet / Charles-Clément Gross CIR3
 * @notes : OK sur site tests
 *
 */

#include "integration.h"


void integrationTest(char* filename)
{
    int etat = 0;

    //Initialisation de notre structure FIR
    FIR_variables* variablesFIR = initialisationFIR();

    //Initialisation de notre structure IIR
    absorp myAbsorp;
    test* variablesIir = (test*)malloc(1 * sizeof(test));
    variablesIir->startLed = 0; // x(n) pour ACR
    variablesIir->ledMinusOne = 0; // x(n-1) pour ACR
    variablesIir->ledMinusOneBis = 0; // y(n-1) pour ACR
    variablesIir->startLedI = 0; // x(n) pour ACIR
    variablesIir->ledIMinusOne = 0; // x(n-1) pour ACIR
    variablesIir->ledIMinusOneBis = 0; // y(n-1) pour ACIR

    //Initialisation de notre structure mesure
    oxy myOxy;
    mesureVariables* variablesMesure = (mesureVariables*)malloc(1 * sizeof(mesureVariables));
    variablesMesure->maxACR = 0; //x(n-1) pour Max CàC ACR
    variablesMesure->maxACIR = 0; //x(n-1) pour Max CàC ACIR
    variablesMesure->minACR = 0; //x(n-1) pour Min CàC ACR
    variablesMesure->minACIR = 0; //x(n-1) pour Min CàC ACIR

    variablesMesure->periode = 0; //compteur pour la periode
    variablesMesure->nLastVal = 0; //stock la valeur precedente
    variablesMesure->etat = 0; //machine d'etat
    variablesMesure->pouls = 0;//accumule les valeurs du BPM pour rendre une moyenne

    //Initialisation du fichier
    FILE* fichier = initFichier(filename);
    do{
        //Lecture du fichier .fir ligne par ligne par une variablesIir Temporaire !!
        absorp tmpmyAbsorp = lecture(fichier, &etat);
        //Condition sur les valeurs
        if(!(tmpmyAbsorp.acr == 0 && tmpmyAbsorp.dcr == 0 && tmpmyAbsorp.acir == 0 && tmpmyAbsorp.dcir == 0)){

            if (etat == EOF){

            }
            else {
                //Filtrage FIR
                variablesFIR->absorp1 = tmpmyAbsorp;
                variablesFIR->absorp1 = FIR(&tmpmyAbsorp, variablesFIR->ACR, variablesFIR->ACIR, variablesFIR->FIR_TAPS);

                //Filtrage IIR
                iir(&variablesFIR->absorp1, variablesIir);
                myAbsorp = variablesFIR->absorp1;

                //Mesure
                mesure(&myOxy, myAbsorp, variablesMesure);

                //Affichage
                affichage(myOxy);
            }

        }


    } while (etat != EOF);

    //Pour vider nos queue a la fin de la lecture du fichier pour ne pas perdre de memoire
    while (isQueueEmpty(variablesFIR->ACR) == 0){
        dequeue(variablesFIR->ACR, &variablesFIR->etatVal);
    }
    while (isQueueEmpty(variablesFIR->ACIR) == 0){
        dequeue(variablesFIR->ACIR, &variablesFIR->etatVal);
    }

    //Liberation de l'espace de stockage du malloc
    free(variablesFIR);
    free(variablesIir);
    free(variablesMesure);
    //Fermeture du fichier
    finFichier(fichier);



}