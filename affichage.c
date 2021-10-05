/*
 * @name : affichage.c
 * @goal : changer la valeur de pouls et de spo2 dans Data.txt
 * @author : Emeric Guichet / Charles-Clément Gross CIR3
 * @notes : ne pas remonter dans l'arboresence, laisser Data.txt dans le cmake-build-debug
 */
#include "affichage.h"

void affichage(oxy myOxy){

    //Initialisation
    FILE* data =  NULL;

    if( access( ".verrouData", F_OK ) != -1 )
    {
        // Fichier verrou existe donc il y a presence d'ecriture et lecture
    }else{
        // Fichier verrou n'existe pas, donc autorise la lecture et ecriture

        FILE* verrouData = NULL;
        verrouData = fopen(".verrouData", "w");
        fclose(verrouData);

        //---Ecriture---

        /*
         * ajout de la valeur de spo2 et pouls dans notre fichier Data.txt
         */
        data = fopen("Data.txt", "w+");
        //Ecriture du spo2
        fprintf(data,"%d", myOxy.spo2);
        fprintf(data,"\n");
        //Ecriture du pouls
        fprintf(data,"%d", myOxy.pouls);

        fclose(data);
        //---Suppression--- .verrouData à la fin de l'ecriture
        remove(".verrouData");
    }
}

