%{
// HEADERS
#include <stdlib.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%
[ \t]+ {  }
\/\/.*\n { yyline++; }
\n* { yyline++; }
\-?[0-9]+ {
   yylval.intValue = atoi(yytext);
   return INT; }
"func main()" { return MAIN; }
"package" { return PACK; }
"import" { return IMP; }
";" { return SC; }
"," { return COMMA; }
"&" { return COM_AND; }
"if" { return IF; }
"else" { return ELSE; }
"for" { return FOR; }
"var" { return V_A; }
":=" { return ATRIB; }
"print" { return PRINT; }
"fmt.Scanf" { return SCAN; }
[a-z][a-zA-Z0-9_]* {
     yylval.str = strdup(yytext);
     return VAR; }
"{" { return O_BRC; }
"}" { return C_BRC; }
"(" { return O_PAR; }
")" { return C_PAR; }
"++" { return INCR; }
"--" { return DECR; }
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return MULT; }
"/" { return DIV; }
"%" { return REST; }
"=" { return EQ_S; }
"==" { return EQ; }
"!=" { return DIF; }
"<" { return LT; }
">" { return BT; }
"<=" { return LET; }
">=" { return BET; }
\".*\" {
    yylval.str = strdup(yytext);
    return STR; }
.  { printf("Line %d:%s\n", yyline, yytext);
   yy_fatal_error("compile error");}
%%
