#ifndef POLINOMSCALCULATOR_MONOM_H
#define POLINOMSCALCULATOR_MONOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "MonomBase.h"

extern int yyerror(const char*);

typedef struct Monom {
    long long number;
    struct Base base;
} Monom;
/* ------------------- Monoms creation ------------------- */
Monom createMonom(unsigned number, struct Base* base) {
    if(base == NULL)
        yyerror("Attempt to null pointer dereference");
    Monom result;
    result.number = number;
    result.base = *base; *base = createEmptyBase();
    return result;
};
Monom createEmptyMonom() {
    Monom result;
    result.number = 0;
    result.base = createEmptyBase();
    return result;
};
Monom fromNumber(unsigned number){
    Monom result;
    result.number = number;
    result.base = createEmptyBase();
    return result;
};
Monom fromBase(struct Base* base) {
    if(base == NULL)
        yyerror("Attempt to null pointer dereference");
    Monom result;
    result.number = 1;
    result.base = *base; *base = createEmptyBase();
    return result;
};
/* ------------------- Monoms printing ------------------- */
void printMonomFormatted(const Monom* monom) {
    if(monom == NULL)
        yyerror("Attempt to null pointer dereference");
    printf("\t\t{ ");
    if(monom->number > 0) {
        printf("+%lld", monom->number);    
        printBase(&monom->base);
    }
    if(monom->number < 0) {
        printf("%lld", monom->number);    
        printBase(&monom->base);
    }
    printf(" }.\n");
}
unsigned printMonom(const Monom* monom) {
    if(monom == NULL)
        yyerror("Attempt to null pointer dereference");
    unsigned result = 0;
    if(monom->number > 0) {
        printf("+%lld", monom->number);    
        printBase(&monom->base);
        result++;
    }
    if(monom->number < 0) {
        printf("%lld", monom->number);    
        printBase(&monom->base);
        result++;
    }
    if(result != 0)
        printf(" ");
    return result;
}
/* ------------------- Get total power ------------------- */
unsigned getMonomTotalPower(const Monom* monom) {
    if(monom == NULL)
        yyerror("Attempt to null pointer dereference");
    return getBaseTotalPower(&monom->base);
}
short isMonomNumber(const Monom* monom) {
    if(monom == NULL)
        yyerror("Attempt to null pointer dereference");
    return isBaseEmpty(&monom->base);
}
/* ------------------------------------------------------- */
Monom multiplyMonoms(const Monom* first, const Monom* second) {
    if(first == NULL || second == NULL)
        yyerror("Attempt to null pointer dereference");
    Monom result = createEmptyMonom();
    result.number = first->number * second->number;
    result.base = uniteBases(&first->base, &second->base);
    return result;
}
void powerUp(Monom* monom, unsigned power) {
    if(monom == NULL)
        yyerror("Attemption to null pointer dereference");
    monom->number = (unsigned) pow(monom->number, power);
    multiplyBase(&monom->base, power);
}

#endif //POLINOMSCALCULATOR_MONOM_H
