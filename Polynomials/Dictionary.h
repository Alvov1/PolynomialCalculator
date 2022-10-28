#ifndef POLINOMSCALCULATOR_DICTIONARY_H
#define POLINOMSCALCULATOR_DICTIONARY_H

#include "Polynom.h"

extern int yyerror(const char*);
extern void yyinfo(const char*);

#define IdentifierLength 21
typedef struct DictEntry {
    char name[IdentifierLength];
    struct Polynom* polynom;
} DictEntry;
DictEntry createEmptyEntry() {
    DictEntry result;
    memset(&result.name[0], 0, IdentifierLength * sizeof(char));
    result.polynom = NULL;
    return result;
}

#define DictionarySize 60
struct Dictionary {
    unsigned position;
    struct DictEntry table[DictionarySize];
} Dictionary;

unsigned addIdentifier(const char* name, struct Polynom* polynom) {
    if(name == NULL || polynom == NULL)
        yyerror("Attemption to null pointer dereference");
    if(strlen(name) >= IdentifierLength - 1)
        yyerror("Identifier is too large");
    if(Dictionary.position >= DictionarySize)
        yyerror("Language dictionary owerflow");

    DictEntry* current = &Dictionary.table[Dictionary.position];

    for(unsigned i = 0; i < Dictionary.position; ++i)
        if(strcmp(name, &Dictionary.table[i].name[0]) == 0) {
            yyinfo("Identifier redefined");
            current = &Dictionary.table[i];
        }

    
    *current = createEmptyEntry();
    strcpy(current->name, name);
    current->polynom = (struct Polynom*) malloc(sizeof(struct Polynom));
    *current->polynom = *polynom;    
    
    return Dictionary.position++;
}

Polynom* getByName(const char* name) {
    if(name == NULL)
        yyerror("Attemption to null pointer dereference");
    for(unsigned i = 0; i < Dictionary.position; ++i)
        if(strcmp(name, &Dictionary.table[i].name[0]) == 0)
            return Dictionary.table[i].polynom;
    return NULL;
}

#endif //POLINOMSCALCULATOR_DICTIONARY_H
