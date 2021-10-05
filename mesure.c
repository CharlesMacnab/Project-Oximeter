/*
 * @name : mesure.c
 * @goal : calcul de Sop2 et pouls
 * @author : Emeric Guichet / Charles-Clément Gross CIR3
 * @notes :
 */

#include "mesure.h"
#include "fichiers.h"

oxy mesureTest(char* filename){
    oxy myOxy;
    int etat = 0;

    //Initialisation de notre structure
    mesureVariables* variable = (mesureVariables*)malloc(1*sizeof(mesureVariables));
    variable->maxACR = 0; //x(n-1) pour Max CàC ACR
    variable->maxACIR = 0; //x(n-1) pour Max CàC ACIR
    variable->minACR = 0; //x(n-1) pour Min CàC ACR
    variable->minACIR = 0; //x(n-1) pour Min CàC ACIR

    variable->periode = 0; //compteur pour la periode
    variable->nLastVal = 0; //stock la valeur precedente
    variable->etat = 0; //machine d'etat
    variable->pouls = 0;//accumule les valeurs du BPM pour rendre une moyenne

    //Initialisation du fichier .fir
    FILE* iirFichier = initFichier(filename);
    do{
        //Lecture du fichier .fir ligne par ligne par une variable Temporaire !!
        absorp tmpmyAbsorp = lireFichier(iirFichier, &etat);
        //Condition sur les valeurs
        if(!(tmpmyAbsorp.acr == 0 && tmpmyAbsorp.dcr == 0 && tmpmyAbsorp.acir == 0 && tmpmyAbsorp.dcir == 0)){
            //Filtrage
            mesure(&myOxy,tmpmyAbsorp, variable);
            /*printf("%d\n",myOxy.spo2);
            printf("%d\n",myOxy.pouls);*/

        }


    } while (etat != EOF);
    //Liberation de l'espace de stockage du malloc
    free(variable);
    //Fermeture du fichier
    finFichier(iirFichier);


    return myOxy;

}


void mesure(oxy* myoxy, absorp mesureAbsorp, mesureVariables* variables){
    int BPM;
    float nbPeriodePourCalcul = 5;
    float tempsUnePeriode = 0;
    float doublems = 0.002;// on prend des valeurs toutes les 2 ms

    switch(variables->etat){

        // Une courbe sinisoidale passe par 0 3 fois, donc tous les 3 passages on a une periode
        case 0:
            // On commence la premiere a 0
            if( ((variables->nLastVal)*(mesureAbsorp.acir)<0) ||  mesureAbsorp.acr == 0){
                variables->etat = 1;
            }
            break;
        case 1:
            variables->cmp++;
            if ( ((variables->nLastVal)*(mesureAbsorp.acir)<0) ||  mesureAbsorp.acr == 0){
                variables->etat = 2;
            }
            break;
        case 2:
            variables->cmp++;

            if ( ((variables->nLastVal)*(mesureAbsorp.acir)<0) ||  mesureAbsorp.acr == 0){
                // On revient a 1 car on a deja passe le premier 0
                variables->etat = 1;
                variables->periode++;
                // Formule de periode
                tempsUnePeriode = (variables->cmp*doublems);
                BPM = 60 / tempsUnePeriode;
                // Accumule les variables
                variables->pouls += BPM;

                // On renitialise le comteur a chaque fin de periode
                variables->cmp = 0;


                if(variables->periode >=nbPeriodePourCalcul){

                    //Valeur finale du pouls
                    myoxy->pouls = variables->pouls/variables->periode;


                    // On renitialise le comteur de periode
                    variables->periode = 0;

                    // Différence pour avoir la valeur crête à crête du ACR et ACIR
                    variables->ACRPique = (variables->maxACR) - (variables->minACR);
                    variables->ACIRPique = (variables->maxACIR) - (variables->minACIR);

                    // Formule du RSIR
                    variables->RSIR = (variables->ACRPique / mesureAbsorp.dcr) / (variables->ACIRPique / mesureAbsorp.dcir);

                    float coef = -25;
                    float ordonnee = 110;
                    float coef2 = -35;
                    float ordonnee2 = 121;
                    int totalSPO2;

                    //Determiner les variables de la figure 15 pour cette partie,

                        //coef = -25 et l'ordonnée à l'origine = 110 entre 0.4 et 1

                    if(variables->RSIR >= 0.4 && variables->RSIR < 1) {

                          totalSPO2  = variables->RSIR*coef + ordonnee;

                    }
                        //coef = -35 et l'ordonnée à l'origine = 121 entre 1 et 3.4

                    else {

                        totalSPO2 = variables->RSIR*coef2 + ordonnee2;

                    }
                    // valeur final du spo2
                    myoxy->spo2 = totalSPO2;
                    variables->pouls = 0;

                }
                // Renitialise les valeurs des cretes a la fin de la periode
                variables->maxACR=0;
                variables->maxACIR=0;
                variables->minACR=0;
                variables->minACIR=0;
            }

            break;

    }

    //---------Calcul Spo2-----------

    //Je regarde à chaque fois si la valeur est sup au max et inversement pour le min
    if(mesureAbsorp.acr>variables->maxACR){
        variables->maxACR=mesureAbsorp.acr;
    }
    if(mesureAbsorp.acir>variables->maxACIR){
        variables->maxACIR=mesureAbsorp.acir;
    }
    if(mesureAbsorp.acr<variables->minACR){
        variables->minACR=mesureAbsorp.acr;
    }
    if(mesureAbsorp.acir<variables->minACIR){
        variables->minACIR=mesureAbsorp.acir;
    }

    // on met à jour la valeur precedente
    variables->nLastVal = mesureAbsorp.acir;


}