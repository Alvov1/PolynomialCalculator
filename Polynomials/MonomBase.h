#ifndef POLINOMSCALCULATOR_MONOMBASE_H
#define POLINOMSCALCULATOR_MONOMBASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern int yyerror(const char*);

#define PossibleVariablesCount 26
/* ------------------- Factors creation ------------------ */
typedef struct Factor {
    char variable;
    unsigned power;
} Factor;
Factor createFactor(char variable, unsigned power) {
    if(variable < 'a' || variable > 'z')
        yyerror("Unsupported variable inside factor");
    Factor result;
    result.variable = variable;
    result.power = power;
    return result;
}



/* ------------------- Base creation --------------------- */
typedef struct Base {
    unsigned factors[PossibleVariablesCount];
} Base;
Base createEmptyBase() {
    Base result;
    for(unsigned i = 0; i < PossibleVariablesCount; ++i)
        result.factors[i] = 0;
    return result;
}
/* -------------- Insert factor into base ---------------- */
void insertFactor(Base* base, struct Factor* factor) {
    if(base == NULL || factor == NULL)
        yyerror("Attempt to null pointer dereference");
    base->factors[factor->variable - 'a'] += factor->power;
}
/* ------------------------- Tests ----------------------- */
short areBasesEquals(const Base* first, const Base* second) {
    if(first == NULL || second == NULL)
        yyerror("Attempt to null pointer dereference");
    for(unsigned i = 0; i < PossibleVariablesCount; ++i)
        if(first->factors[i] != second->factors[i])
            return 0;
    return 1;
}
short isBaseEmpty(const Base* base) {
    if(base == NULL)
        yyerror("Attempt to null pointer dereference");
    for(unsigned i = 0; i < PossibleVariablesCount; ++i)
        if(base->factors[i] != 0)
            return 0;
    return 1;
}
/* --------------------- Base printing ------------------- */
void printBase(const Base* base) {
    if(base == NULL)
        yyerror("Attempt to null pointer dereference");
    for(unsigned i = 0; i < PossibleVariablesCount; ++i)
        if(base->factors[i] != 0) // If the power of the current factor is not zero
            printf("%c^%d", 'a' + i, base->factors[i]);
}
/* --------------- Count base total power ---------------- */
unsigned getBaseTotalPower(const Base* base) {
    if(base == NULL)
        yyerror("Attempt to null pointer dereference");
    unsigned sum = 0;
    for(unsigned i = 0; i < PossibleVariablesCount; ++i)
        sum += base->factors[i];
    return sum;
}
/* ------------------------------------------------------- */
Base uniteBases(const Base* first, const Base* second) {
    if(first == NULL || second == NULL)
        yyerror("Attemption to null pointer dereference");
    Base result = createEmptyBase();
    for(unsigned i = 0; i < PossibleVariablesCount; ++i)
        result.factors[i] = first->factors[i] + second->factors[i];
    return result;
}
void multiplyBase(Base* base, unsigned number) {
    if(base == NULL)
        yyerror("Attemption to null pointer dereference");
    for(unsigned i = 0; i < PossibleVariablesCount; ++i)
        base->factors[i] *= number;
}
#endif //POLINOMSCALCULATOR_MONOMBASE_H