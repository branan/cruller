%{
#include <stdio.h>
#include <vector>
#include "cruller.hpp"

List* program;
 
extern int yylex();
void yyerror(const char *str) {
        fprintf(stderr,"error: %s\n",str);
}
 
extern "C" int yywrap() {
        return 1;
}
%}

%token LPAREN RPAREN

%union {
  Object* node;
  List* list;
  int number;
  char *string;
}

%token <number> NUMBER
%token <string> IDENTIFIER

%type <node> sexpr
%type <node> list
%type <list> members
%type <node> atom

%start program

%%
program: members            { program = $1 }
    ;
sexpr: atom                 { $$ = $1; }
    | list                  { $$ = $1; }
    ;
list: LPAREN members RPAREN { $$ = $2; }
    ;
members:                    { $$ = 0; }
    | sexpr members         { $$ = new List($1, $2); }
    ;
atom: IDENTIFIER            { $$ = new Identifier($1); }
    | NUMBER                { $$ = new Number($1); }
    ;
%%
