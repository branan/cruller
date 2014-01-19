%{
#include "parser.h"
%}

%%
[0-9]+               { yylval.number = atoi(yytext); return NUMBER; }
\(                   { return LPAREN; }
\)                   { return RPAREN; }
[^(), \t\n]+         { yylval.string = strdup(yytext); return IDENTIFIER; }
[, \t\n] /* ignore whitespace (including commas) */
%%
