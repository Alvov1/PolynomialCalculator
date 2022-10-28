%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #include "Parser.h"

    #define IdentifierLength 21

    extern int yylex();
    extern int yyerror();
    extern int yywrap();
    extern void yyinfo();
    extern int yyparse();
    extern void yyinfo();

    extern FILE* yyin;
   	
   	extern unsigned linecount;
   	extern unsigned rowcount;
    
    #define IdentifierLength 21
    extern int yyerror(const char*);
    extern void prepare();

    int main(int argc, const char** argv) {
        if(argc == 1) {
            printf("Please pass the name of the input file as comang line argument.\n");
            exit(1);
        }

        yyin = fopen(argv[1], "r");
        if(yyin == NULL) {
            printf("Error: Input file is not found.\n");
            exit(1);
        }
        prepare();
        yyparse();

        fclose(yyin);
    }

    void pushNumber();
    void pushVariable();
    void pushName();
%}
%%

"//".*                                  { }
"/"                                     { yyerror("Unterminated singleline comment"); }
[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]     { }
[/][*]                                  { yyerror("Unterminated multiline comment"); }

[0-9]+([eE][+-]?[0-9]+)?                { rowcount += strlen(yytext); pushNumber(); return NUMBER; }


[a-z]                                   { rowcount += strlen(yytext); pushVariable(); return VARIABLE; }
[A-Z_][a-zA-Z0-9_]+                     { if(strlen(yytext) >= IdentifierLength) yyerror("Identifier is too long");
                                            rowcount += strlen(yytext); pushName(); return IDENTIFIER; }
[a-z][A-Z0-9_]+                         { yyerror("Varibles starting with lowercase are unacceptable"); }

[\t]                                    { rowcount += 4; }
[ ]                                     { rowcount += 1; }
[\n]                                    { rowcount = 0;  linecount++; }
[\r]                                    { rowcount = 0; }
"?"                                     { rowcount += 1; return *yytext; }
"*"                                     { rowcount += 1; return *yytext; }
"-"                                     { rowcount += 1; return *yytext; }
"+"                                     { rowcount += 1; return *yytext; }
"="                                     { rowcount += 1; return *yytext; }
"("                                     { rowcount += 1; return *yytext; }
")"                                     { rowcount += 1; return *yytext; }
"^"                                     { rowcount += 1; return *yytext; }

.                                       { yyerror("Unacceptable character"); }
%%

/* Sets number into the yylval union. */
void pushNumber() {
    unsigned len = strlen(yytext);
    if(len > 10 || (len == 10 && strcmp(yytext, "4294967295")) > 0)        
        yyerror("Unsigned overflow");
    yylval.number = atoi(yytext);
}

/* Sets current variable into the yylval union. */
void pushVariable() {
    if(*yytext < 'a' || *yytext > 'z')
      yyerror("Unsupported variable inside monom");
    yylval.variable = *yytext;
}

/* Sets current identifier's name into the yylval union. */
void pushName() {
    unsigned len = strlen(yytext);
    if(len > IdentifierLength)
        yyerror("Too long variable name");
    strncpy((char*) yylval.name, yytext, len);
    memset((char*) &yylval.name[len], 0, sizeof(char) * (IdentifierLength - len));
}
