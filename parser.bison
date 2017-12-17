// Tokens
%token INT IF ELSE O_BRC C_BRC MAIN VAR V_A PRINT STR
%token ATRIB SCAN FOR INCR DECR SC PACK IMP COMMA COM_AND
// Operator associativity & precedence
%left EQ DIF LT BT LET BET EQ_S
%left O_PAR C_PAR
%left PLUS MINUS
%left MULT DIV REST

// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.
%union {
  int intValue;
  Expr* exprValue;
  Pack* pcK;
  PackList* packList;
  Cmd* cmD;
  CmdList* cmdList;
  char *str;
}

%type <pcK> pck
%type <packList> pck_list
%type <intValue> INT
%type <exprValue> expr
%type <exprValue> cond
%type <str> VAR
%type <str> STR
%type <cmD> cmd
%type <cmD> for_cmd
%type <cmdList> cmd_list

// Use "%code requires" to make declarations go
// into both parser.c and parser.h
%code requires {
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
CmdList* root;
PackList* r_packs;
}

%%
program: pck_list MAIN O_BRC cmd_list C_BRC { r_packs= $1; root = $4; }

pck_list: /* empty */ { $$=NULL; }
  | pck pck_list { $$=mkPList($1,$2); }
  ;

pck: PACK VAR {
    $$ = pack_Package($2);
  }
  | IMP STR {
    $$ = pack_Import($2);
  }
  ;

cmd_list: /* empty */ { $$=NULL; }
  | cmd cmd_list { $$=mkList($1,$2); }
  ;

cmd: IF cond O_BRC cmd_list C_BRC{
    $$ = cmd_If($2, $4, NULL);
  }
  | IF cond O_BRC cmd_list C_BRC ELSE O_BRC cmd_list C_BRC{
    $$ = cmd_If($2, $4, cmd_Else($8));
  }
  | FOR cond O_BRC cmd_list C_BRC {
    $$ = cmd_For($2, $4);
  }
  | FOR cmd cond SC for_cmd O_BRC cmd_list C_BRC {
    $$ = cmd_For2($2, $3, $5, $7);
  }
  | V_A VAR EQ_S expr SC{
    $$ = cmd_atrib($2, $4);
  }
  | VAR EQ_S expr SC{
    $$ = cmd_atrib($1, $3);
  }
  | VAR INCR SC{
    $$ = cmd_atrib($1, ast_operation(PLUS, ast_var($1), ast_integer(1)));
  }
  | VAR DECR SC{
    $$ = cmd_atrib($1, ast_operation(MINUS, ast_var($1), ast_integer(1)));
  }
  | VAR ATRIB expr SC{
    $$ = cmd_atrib($1, $3);
  }
  | PRINT O_PAR STR C_PAR SC{
    $$ = cmd_print($3);
  }
  | PRINT O_PAR VAR C_PAR SC{
    $$ = cmd_print($3);
  }
  | SCAN O_PAR STR COMMA COM_AND VAR C_PAR SC{
    $$ = cmd_scan($3,$6);
  }

for_cmd: V_A VAR EQ_S expr{
    $$ = cmd_atrib($2, $4);
  }
  | VAR EQ_S expr{
    $$ = cmd_atrib($1, $3);
  }
  | VAR INCR{
    $$ = cmd_atrib($1, ast_operation(PLUS, ast_var($1), ast_integer(1)));
  }
  | VAR DECR{
    $$ = cmd_atrib($1, ast_operation(MINUS, ast_var($1), ast_integer(1)));
  }
  | VAR ATRIB expr{
    $$ = cmd_atrib($1, $3);
  }
  | PRINT O_PAR STR C_PAR{
    $$ = cmd_print($3);
  }
  | PRINT O_PAR VAR C_PAR{
    $$ = cmd_print($3);
  }
  | SCAN O_PAR STR COMMA COM_AND VAR C_PAR SC{
    $$ = cmd_scan($3,$6);
  }

expr:
  VAR {
    $$ = ast_var($1);
  }
  |
  INT {
    $$ = ast_integer($1);
  }
  |
  expr PLUS expr {
    $$ = ast_operation(PLUS, $1, $3);
  }
  |
  expr MINUS expr {
    $$ = ast_operation(MINUS, $1, $3);
  }
  |
  expr MULT expr {
    $$ = ast_operation(MULT, $1, $3);
  }
  |
  expr DIV expr {
    $$ = ast_operation(DIV, $1, $3);
  }
  |
  expr REST expr {
    $$ = ast_operation(REST, $1, $3);
  }

cond:
  expr EQ expr {
    $$ = ast_bool(EQ, $1, $3);
  }
  |
  expr DIF expr {
    $$ = ast_bool(DIF, $1, $3);
  }
  |
  expr LT expr {
    $$ = ast_bool(LT, $1, $3);
  }
  |
  expr BT expr {
    $$ = ast_bool(BT, $1, $3);
  }
  |
  expr LET expr {
    $$ = ast_bool(LET, $1, $3);
  }
  |
  expr BET expr {
    $$ = ast_bool(BET, $1, $3);
  }
  ;
%%

void yyerror(const char* err) {
  printf("Line %d: %s - '%s'\n", yyline, err, yytext  );
}
