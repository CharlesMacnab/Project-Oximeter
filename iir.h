#ifndef IIR_H
#define IIR_H

#include "define.h"
#include "fichiers.h"

typedef struct {
    float startLed; // x(n)
    float ledMinusOne; // x(n-1)
    float ledMinusOneBis; // y(n-1)

    float startLedI; // x(n)
    float ledIMinusOne;// x(n-1)
    float ledIMinusOneBis; //y(n-1)
}test;

absorp iirTest(char* filename);
void iir(absorp* myAbsorp, test* variables); // Fonction de filtrage IIR

#endif

