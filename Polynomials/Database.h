#ifndef POLINOMSCALCULATOR_DATABASE_H
#define POLINOMSCALCULATOR_DATABASE_H

#include "Polynom.h"

extern int yyerror(const char*);

typedef struct DatabaseEntry {
    short used; // Boolean
    struct Polynom polynom;
} DatabaseEntry;

void createEntry(DatabaseEntry* entry, const Polynom* polynom) {
    if(entry == NULL || polynom == NULL)
        yyerror("Attemption to null pointer dereference");
    entry->used = 1;
    entry->polynom = *polynom;
}

void clearEntry(DatabaseEntry* entry) {
    if(entry == NULL)
        yyerror("Attemption to null pointer dereference");
    entry->used = 0;
    entry->polynom = createEmptyPolynom();
}

#define DatabaseSize 100
DatabaseEntry Database[DatabaseSize];

void clearDatabase() {
    for(unsigned i = 0; i < DatabaseSize; ++i) {
        Database[i].used = 0;
        Database[i].polynom = createEmptyPolynom();
    }
}

unsigned pushPolynom(const Polynom* polynom) {
    if(polynom == NULL)
        yyerror("Attemption to null pointer dereference");
    for(unsigned i = 0; i < DatabaseSize; ++i)
        if(Database[i].used == 0) {
            createEntry(&Database[i], polynom);
            return i;
        }
    return yyerror("Polynoms database overflowed");
}

void clearPolynom(unsigned index) {
    if(index > DatabaseSize)
        yyerror("Attemption to clear too large index inside the table");
    if(Database[index].used == 0)
        yyerror("Cell in the table is already cleared");
    clearEntry(&Database[index]);
}

Polynom* getByIndex(unsigned index) {
    if(index > DatabaseSize)
        yyerror("Polynom index in the table is incorrect");
    if(Database[index].used == 0)
        yyerror("Appeal to the empty cell in the table");
    return &Database[index].polynom;
}

#endif //POLINOMSCALCULATOR_DATABASE_H
