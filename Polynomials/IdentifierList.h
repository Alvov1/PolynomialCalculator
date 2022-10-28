#ifndef POLINOMSCALCULATOR_IDENTIFIERLIST_H
#define POLINOMSCALCULATOR_IDENTIFIERLIST_H

#define IdentifiersLimit 100
#define IdentifierLength 21
struct IdentifierList {
    unsigned position;
    char list[IdentifiersLimit][IdentifierLength];
} IdentifierList;

unsigned pushIdentifier(const char* name) {
    if(name == NULL)
        yyerror("Attemption to null pointer dereference");
    if(IdentifierList.position >= IdentifiersLimit)
        yyerror("Identifiers list overflow");
    if(strlen(name) >= IdentifierLength - 1)
        yyerror("Identifier is too large");
    strcpy(&IdentifierList.list[IdentifierList.position][0], name);
    return IdentifierList.position++;
}
const char* getIdentifier(unsigned index) {
    if(index >= IdentifierList.position)
        yyerror("Identifier index is too large");
    return (const char*) &IdentifierList.list[index];
}

#endif //POLINOMSCALCULATOR_IDENTIFIERLIST_H
