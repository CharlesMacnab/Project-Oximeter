#include "lecture.h"




absorp lecture(FILE* file_pf, int* file_state){
    absorp myAbsorp;
    // On initialise acr, dcr, acir et dcir
    myAbsorp.acr = -1;
    myAbsorp.dcr = -1;
    myAbsorp.acir = -1;
    myAbsorp.dcir = -1;

    char chaine[30];

    if(file_pf != NULL){
        //Verification si on a bien un fichier valid

        if(!feof(file_pf)){
            //Verification si on n'est pas a la fin du fichier
            float a=0,b=0,c=0,d=0, erreurLigne = 1;
            while(erreurLigne){
                erreurLigne = 0;
                float valid = fscanf(file_pf,"%f,%f,%f,%f\n",&a,&b,&c,&d);// 4 valeurs avec les virgules puis \n et \r
                if(valid != 4){//On elemine les lignes vides
                    fgets(chaine, 30, file_pf);
                    erreurLigne = 1;
                }
                // Recuperer la position courante du fichier, un pointeur se place dans le fichier
                *file_state = ftell(file_pf);
            }
            //on centre nos valeurs d'acr et d'acir en faisant -2048 --> Ref ac_cap STM32
            myAbsorp.acr = a - 2048;
            myAbsorp.dcr = b;
            myAbsorp.acir = c - 2048;
            myAbsorp.dcir = d;

        }else{
            *file_state = EOF;
        }
    }else{//si on n'a pas bon fichier
        printf("Erreur fichier");
        *file_state = EOF;
    }
    return myAbsorp; // return EOF flag
}


