#ifndef POLINOMSCALCULATOR_MONOMARRAY_H
#define POLINOMSCALCULATOR_MONOMARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Monom.h"
extern int yyerror(const char*);

#define PossibleMonomsWithSameDegree 20
/* --------------------- Monom array --------------------- */
typedef struct MonomArray {
    struct Monom buffer[PossibleMonomsWithSameDegree];
    unsigned length;
} MonomArray;
/* ----------------- Monom array creation ---------------- */
MonomArray createEmptyMonomArray() {
    MonomArray result;

    result.length = 0;
    for(unsigned i = 0; i < PossibleMonomsWithSameDegree; ++i)
        result.buffer[i] = createEmptyMonom();

    return result;
}
/* ------------------ Push monom to array ---------------- */
void pushMonomToArray(MonomArray* array, const struct Monom* monom) {
    if(array == NULL || monom == NULL)
        yyerror("Attemption to null pointer dereference");
    
    /* First check if monom with the same powers already in the list */
    for(unsigned i = 0; i < array->length; ++i)
        if(areBasesEquals(&monom->base, &array->buffer[i].base)) {
            array->buffer[i].number += monom->number;
            return;
        }

    /* If not, then append monom to the list */
    if(array->length < PossibleMonomsWithSameDegree)
        array->buffer[array->length++] = *monom;
    else
        yyerror("The array of monomials is full");
}
void pushMonomToEmptyArray(MonomArray* array, const struct Monom* monom) {
    if(array == NULL || monom == NULL)
        yyerror("Attemption to null pointer dereference");
    
    if(array->length < PossibleMonomsWithSameDegree)
        array->buffer[array->length++] = *monom;
    else
        yyerror("The array of monomials is full");
}
/* ------------------ Print monom Array ------------------ */
unsigned printMonomArray(const MonomArray* monomArray) {
    if(monomArray == NULL)
        yyerror("Attemption to null pointer dereference");
    unsigned count = 0;
    for(unsigned i = 0; i < monomArray->length; ++i)
        count += printMonom(&monomArray->buffer[i]);
    return count;
}
/* ------------------------------------------------------- */
MonomArray multiplyMonomArray(const MonomArray* array, const struct Monom* monom) {
    if(array == NULL || monom == NULL)
        yyerror("Attemption to null pointer dereference");
    MonomArray result = createEmptyMonomArray();
    for(unsigned i = 0; i < array->length; ++i) {
        struct Monom current = multiplyMonoms(&array->buffer[i], monom);
        pushMonomToEmptyArray(&result, &current);
    }
    return result;
}
void addMonomArrays(MonomArray* dest, const MonomArray* source) {
    if(dest == NULL || source == NULL)
        yyerror("Attemption to null pointer dereference");
    for(unsigned i = 0; i < source->length; ++i)
        pushMonomToArray(dest, &source->buffer[i]);
}
void subMonomArrays(MonomArray* dest, const MonomArray* source) {
    if(dest == NULL || source == NULL)
        yyerror("Attemption to null pointer dereference");
    for(unsigned i = 0; i < source->length; ++i) {
        struct Monom temp = source->buffer[i];
        temp.number *= -1;
        pushMonomToArray(dest, &temp);
    }
}
/* ------------------------------------------------------- */
void negateMonomArray(MonomArray* array) {
    if(array == NULL)
        yyerror("Attemption to null pointer dereference");
    for(unsigned i = 0; i < array->length; ++i)
        array->buffer[i].number *= -1;
}

short isMonomArrayEmpty(const MonomArray* array) {
    if(array == NULL)
        yyerror("Attemption to null pointer dereference");
    if(array->length == 0) return 1;
    for(unsigned i = 0; i < array->length; ++i)
        if(array->buffer[i].number != 0)
            return 0;
    return 1;
}

#endif //POLINOMSCALCULATOR_MONOMARRAY_H