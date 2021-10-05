#ifndef MESURE_H
#define MESURE_H

#include "define.h"
#include <string.h>

typedef struct{


    //Calcule Pouls
    int periode;
    float cmp;
    float nLastVal;


    //Calcul SPO2
    float maxACR;
    float maxACIR;
    float minACR;
    float minACIR;
    float ACRPique;
    float ACIRPique;
    float RSIR;
    int etat;
    int pouls;

}mesureVariables;




oxy mesureTest(char* filename);

void mesure(oxy* myoxy, absorp mesureAbsorp, mesureVariables* variables); // Fonction de mesure du pouls et du spo2

#endif
	
