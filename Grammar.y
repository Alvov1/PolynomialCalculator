%{
	#include <stdio.h>
        #include <stdlib.h>

	#include "Database.h"
	#include "Polynom.h"
	#include "Dictionary.h"
	#include "IdentifierList.h"

        extern int yylex();
        struct Monom CurrentMonom;
	struct Base CurrentBase;
	struct Factor CurrentFactor;
	extern FILE* yyin;

	unsigned linecount = 1;
	unsigned rowcount = 1;
%}

%union {
	unsigned number;
	char name[21];
	char variable;
}

%token<number> NUMBER 
%token<variable> VARIABLE
%token IDENTIFIER

%type<number> Number
%type<number> Power
%type<variable> Variable
%type<number> Monom
%type<number> Polynom
%type<number> Identifier
%type<number> Expression

%left '+' '-'
%left '*'
%right '^'
%right '(' ')'
%right '='

%%

SourceFile :
	| SourceFile Statement
	;

Statement :
	Assignment
	| Printing
	;

Printing :
	'?' Identifier				{ struct Polynom* value = getByName(getIdentifier($2));
							if(value == NULL) yyerror("Unknown identifier"); else { printPolynom(value); printf("\n"); }}
	| '>' Identifier			{ struct Polynom* value = getByName(getIdentifier($2));
							if(value == NULL) yyerror("Unknown identifier"); else printPolynom(value); }
	| Identifier '?' 			{ struct Polynom* value = getByName(getIdentifier($1));
							if(value == NULL) yyerror("Unknown identifier"); else { printPolynom(value); printf("\n"); }}
	| '?' 					{ yyerror("Uncomplete comand"); }
	;

Assignment :
	Identifier '=' Expression		{ addIdentifier(getIdentifier($1), getByIndex($3)); clearPolynom($3);
							printf("%s = ", getIdentifier($1)); printPolynom(getByName(getIdentifier($1))); printf("\n"); }
	| Identifier '=' 			{ yyerror("Uncomplete comand"); }
	;

Expression :
	Polynom					{ $$ = $1; }

	| Identifier 				{ struct Polynom* polynom = getByName(getIdentifier($1));
							if(polynom == NULL) yyerror("Unknown identifier");
							$$ = pushPolynom(polynom); }

	| Identifier '+' Identifier		{ Polynom* left = getByName(getIdentifier($1)); Polynom* right = getByName(getIdentifier($3));
	 						if(left == NULL) yyerror("Unknown identifier on the left of the expression");
	 						if(right == NULL) yyerror("Unknown identifier on the right of the expression");
	 						Polynom result = addPolynom(left, right);
	 						$$ = pushPolynom(&result); }

	| Identifier '-' Identifier		{ Polynom* left = getByName(getIdentifier($1)); Polynom* right = getByName(getIdentifier($3));
							if(left == NULL) yyerror("Unknown identifier on the left ot the expression");
							if(right == NULL) yyerror("Unknown idenfier on the right of the expression");
							Polynom result = subPolynom(left, right);
							$$ = pushPolynom(&result); }

	| Identifier '*' Identifier		{ Polynom* left = getByName(getIdentifier($1)); Polynom* right = getByName(getIdentifier($3));
							if(left == NULL) yyerror("Unknown identifier on the left ot the expression");
							if(right == NULL) yyerror("Unknown idenfier on the right of the expression");
							Polynom result = multPolynom(left, right);
							$$ = pushPolynom(&result); }

	| Identifier '^' Identifier 		{ Polynom* right = getByName(getIdentifier($3));
							if(right == NULL) yyerror("Unknown identifier on the right of the expression");
							if(!isPolynomNumber(right)) yyerror("Exponentiation of polynoms is not allowed");

							Polynom* left = getByName(getIdentifier($1));
							if(left == NULL) yyerror("Unknown identifier on the left of the expression");

							Polynom result = *left;
							polynomPowerUp(&result, polynomToNumber(right));
							$$ = pushPolynom(&result); }
	;

Identifier : IDENTIFIER				{ $$ = pushIdentifier(&yylval.name[0]); };

Polynom :
	Monom					{ struct Polynom temp = fromMonom(&CurrentMonom); $$ = pushPolynom(&temp); CurrentMonom = createEmptyMonom(); }

	| Polynom '+' Polynom			{ struct Polynom r = addPolynom(getByIndex($1), getByIndex($3)); $$ = pushPolynom(&r);
						clearPolynom($1); clearPolynom($3); }

	| Polynom '-' Polynom 			{ struct Polynom r = subPolynom(getByIndex($1), getByIndex($3));  $$ = pushPolynom(&r);
							clearPolynom($1); clearPolynom($3); }

	| Polynom '*' Polynom			{ struct Polynom r = multPolynom(getByIndex($1), getByIndex($3)); $$ = pushPolynom(&r);
							clearPolynom($1); clearPolynom($3); }

	| Polynom '(' Polynom ')'		{ struct Polynom r = multPolynom(getByIndex($1), getByIndex($3)); $$ = pushPolynom(&r);
                                        		clearPolynom($1); clearPolynom($3); }

	| '(' Polynom ')'			{ $$ = $2; }

	| '+' Polynom 				{ $$ = $2; }

	| '-' Polynom 				{ $$ = $2; negatePolynom(getByIndex($2)); }
	| Polynom '^' Polynom			{ $$ = $1; if(!isPolynomNumber(getByIndex($3))) yyerror("Exponentiation of polynoms is not allowed");
							polynomPowerUp(getByIndex($1), polynomToNumber(getByIndex($3))); clearPolynom($3); }

	| '(' ')' 				{ yyerror("Using empty brackets"); }
	;

Monom :
	Number					{ CurrentMonom = fromNumber($1); 			 }
	| Base					{ CurrentMonom = fromBase(&CurrentBase); 		 }
	| Number Base				{ CurrentMonom = createMonom($1, &CurrentBase);		 }
	;
	
Base :
	Factor					{ insertFactor(&CurrentBase, &CurrentFactor); }
	| Base Factor				{ insertFactor(&CurrentBase, &CurrentFactor); }
	;

Factor :
	Variable Power				{ CurrentFactor = createFactor($1, $2); }
	;
	
Variable :
	VARIABLE				{ $$ = yylval.variable; }
	;
	
Power :
						{ $$ = 1; }
	| '^' Polynom				{ if(!isPolynomNumber(getByIndex($2)))
							yyerror("Exponentiation of polynoms is not allowed");
                                                  	$$ = polynomToNumber(getByIndex($2)); clearPolynom($2); }
	;

Number :
	NUMBER					{ $$ = yylval.number; }
	| '(' Number ')'			{ $$ = $2; }
	;
%%

void prepare() {
        clearDatabase();

        extern struct Dictionary Dictionary;
        Dictionary.position = 0;
        extern struct IdentifierList IdentifierList;
        IdentifierList.position = 0;

        CurrentBase = createEmptyBase();
        CurrentMonom = createEmptyMonom();
}

int yyerror(const char* s) {
	printf("Error: %s. [Line %d: Row %d].\n", s, linecount, rowcount);
	fclose(yyin);
	exit(1);
}

void yyinfo(const char* s) {
	printf("Warning: %s. [Line %d: Row %d].\n", s, linecount, rowcount);
}

int yywrap() {
	return 1;
}