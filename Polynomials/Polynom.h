#ifndef POLINOMSCALCULATOR_POLINOM_H
#define POLINOMSCALCULATOR_POLINOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "MonomArray.h"
extern int yyerror(const char*);

#define PossibleMonomsPowerCount 100
/* ------------------------------------------------------- */
typedef struct Polynom {
    struct MonomArray table[PossibleMonomsPowerCount];
} Polynom;
/* ---------------- Push monom to polynom ---------------- */
void pushMonom(Polynom* polynom, struct Monom* monom) {
    if(polynom == NULL || monom == NULL)
        yyerror("Attempt to null pointer dereference");
    unsigned totalPower = getMonomTotalPower(monom);
    if(totalPower < PossibleMonomsPowerCount)
        pushMonomToArray(&polynom->table[totalPower], monom);
    else
        yyerror("Total degree of the monomial exceeds the limit");
    *monom = createEmptyMonom();
};
/* ------------------ Polynoms creation ------------------ */
Polynom createEmptyPolynom() {
    Polynom result;
    for(unsigned i = 0; i < PossibleMonomsPowerCount; ++i)
        result.table[i] = createEmptyMonomArray();
    return result;
};
Polynom fromMonom(struct Monom* monom) {
    if(monom == NULL)
        yyerror("Attempt to null pointer dereference");
    Polynom result = createEmptyPolynom();
    pushMonom(&result, monom); *monom = createEmptyMonom();
    return result;
}
/* -------------------- Print polynom -------------------- */
void printPolynomFormatted(const Polynom* polynom) {
    if(polynom == NULL)
        yyerror("Attempt to null pointer dereference");
    printf("\t\t{ ");
    for(unsigned i = PossibleMonomsPowerCount - 1; i > 0; --i)
        printMonomArray(&polynom->table[i]);
    printMonomArray(&polynom->table[0]);
    printf(" }.\n");
}
void printPolynom(const Polynom* polynom) {
    if(polynom == NULL)
        yyerror("Attempt to null pointer dereference");
    unsigned long zero = 0;
    for(unsigned i = PossibleMonomsPowerCount - 1; i > 0; --i)
        zero += printMonomArray(&polynom->table[i]);
    zero += printMonomArray(&polynom->table[0]);
    if(zero == 0)
        printf("0");
}
/* ----------------- Polynomial operations --------------- */
Polynom addPolynom(const Polynom* left, const Polynom* right) {
    if(left == NULL || right == NULL)
        yyerror("Attemption to null pointer dereference");
    Polynom result = *left;
    for(unsigned i = 0; i < PossibleMonomsPowerCount; ++i)
        addMonomArrays(&result.table[i], &right->table[i]);
    return result;
};
Polynom subPolynom(const Polynom* left, const Polynom* right) {
    if(left == NULL || right == NULL)
        yyerror("Attemption to null pointer dereference");
    Polynom result = *left;
    for(unsigned i = 0; i < PossibleMonomsPowerCount; ++i)
        subMonomArrays(&result.table[i], &right->table[i]);
    return result;
};
void appendPolynom(Polynom* dest, const Polynom* source) {
    if(dest == NULL || source == NULL)
        yyerror("Attemption to null pointer dereference");
    for(unsigned i = 0; i < PossibleMonomsPowerCount; ++i)
        addMonomArrays(&dest->table[i], &source->table[i]);
}
/* --------------- Polynomial multiplication ------------- */
Polynom multiplyOnMonom(const Polynom* polynom, const struct Monom* monom) {
    if(polynom == NULL || monom == NULL)
        yyerror("Attempt to null pointer dereference");

    const unsigned monomPower = getMonomTotalPower(monom);
    Polynom temp = createEmptyPolynom();
    for(unsigned i = 0; i < PossibleMonomsPowerCount - monomPower; ++i) {
        struct MonomArray array = multiplyMonomArray(&polynom->table[i], monom);
        temp.table[i + monomPower] = array;
    }
    for(unsigned i = PossibleMonomsPowerCount - monomPower; i < PossibleMonomsPowerCount; ++i) {
        if(!isMonomArrayEmpty(&polynom->table[i]))
            yyerror("Multiplication of polynoms excedes maximum power limit");
    }
    return temp;
}
Polynom multArrayOnPolynom(const Polynom* polynom, const struct MonomArray* array) {
    if(polynom == NULL || array == NULL)
        yyerror("Attemption to null pointer dereference");
    Polynom result = createEmptyPolynom();
    for(unsigned i = 0; i < array->length; ++i) {
        Polynom temp = multiplyOnMonom(polynom, &array->buffer[i]);
        appendPolynom(&result, &temp);
    }
    return result;
}
Polynom multPolynom(const Polynom* left, const Polynom* right) {
    if(left == NULL || right == NULL)
        yyerror("Attemption to null pointer dereference");
    Polynom result = createEmptyPolynom();
    for(unsigned i = 0; i < PossibleMonomsPowerCount; ++i) {
        Polynom temp = multArrayOnPolynom(right, &left->table[i]);
        appendPolynom(&result, &temp);
//        result = addPolynom(&result, &temp);
    }
    return result;
};
/* -------------- Turn polynom into negative ------------- */
void negatePolynom(Polynom* polynom) {
    if(polynom == NULL)
        yyerror("Attemption to null poiter dereference");
    for(unsigned i = 0; i < PossibleMonomsPowerCount; ++i)
        negateMonomArray(&polynom->table[i]);
}
short isPolynomNumber(const Polynom* polynom) {
    for(unsigned i = 1; i < PossibleMonomsPowerCount; ++i)
        if(!isMonomArrayEmpty(&polynom->table[i]))
            return 0;
    return 1;
}
unsigned polynomToNumber(const Polynom* polynom) {
    if(polynom->table[0].buffer[0].number < 0)
        yyerror("Exponentiation to the negative power is not allowed");
    return polynom->table[0].buffer[0].number;
}
void polynomPowerUp(Polynom* polynom, unsigned power) {
    if(power == 0) {
        Monom m = fromNumber(1);
        *polynom = fromMonom(&m);
    }
    Polynom temp = *polynom;
    for(unsigned i = 1; i < power; ++i)
        temp = multPolynom(&temp, polynom);
    *polynom = temp;
}
#endif //POLINOMSCALCULATOR_POLINOM_H
