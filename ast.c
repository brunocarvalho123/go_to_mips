// AST constructor functions

#include <stdlib.h> // for malloc
#include <stdio.h>
#include "ast.h" // AST header
#include <string.h>
char* array_vars[100];
int var_pos = 0;

Expr* ast_integer(int v) {
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_INTEGER;
  node->attr.value = v;
  return node;
}

Expr* ast_var(char* var){
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_VAR;
  node->attr.var = var;
  return node;
}

Expr* ast_operation
(int operator, Expr* left, Expr* right) {
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_OPERATION;
  node->attr.op.operator = operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}

Expr* ast_bool
(int operator, Expr* left, Expr* right){
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_BOOL;
  node->attr.op_b.operator_b = operator;
  node->attr.op_b.left = left;
  node->attr.op_b.right = right;
  return node;
}

Pack* pack_Package(char* var){
  Pack* node = (Pack*) malloc(sizeof(Pack));
  node->kind = E_PACK;
  node->attr.var = var;
  return node;
}
Pack* pack_Import(char* str){
  Pack* node = (Pack*) malloc(sizeof(Pack));
  node->kind = E_IMP;
  node->attr.str = str;
  return node;
}

Cmd* cmd_If(Expr* cond, CmdList* cmds, Cmd* elses){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = E_IF;
  if(cond->kind == E_BOOL){
    node->attr.ifs.cond = cond;
    node->attr.ifs.cmds = cmds;
    node->attr.ifs.elses = elses;
  }
  else node->attr.ifs.cond = NULL;
  return node;
}

Cmd* cmd_Else(CmdList* cmds){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = E_ELSE;
  node->attr.ifs.cmds = cmds;
  return node;
}

Cmd* cmd_For(Expr* cond, CmdList* cmds){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = E_FOR;
  node->attr.fors.cond = cond;
  node->attr.fors.cmds = cmds;
  return node;
}

Cmd* cmd_For2(Cmd* cmd, Expr* cond, Cmd* incr, CmdList* cmds){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = E_FOR2;
  node->attr.fors2.cmd = cmd;
  node->attr.fors2.cond = cond;
  node->attr.fors2.incr = incr;
  node->attr.fors2.cmds = cmds;
  return node;
}

Cmd* cmd_atrib(char* var, Expr* expr){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = E_ATR;
  int h, hh=0;
  for(h=0; h<var_pos; h++){
    if(strcmp(array_vars[h],var)==0){
      hh=1;
      break;
    }
  }
  if(hh==0){
    array_vars[var_pos] = var;
    var_pos++;
  }
  node->attr.atrib.var = var;
  node->attr.atrib.expr = expr;
  return node;
}

Cmd* cmd_print(char* str){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = E_PRI;
  node->attr.str = str;
  return node;
}

Cmd* cmd_scan(char* str, char* var){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = E_SCAN;
  node->attr.scans.str = str;
  node->attr.scans.var = var;
  return node;
}

PackList* mkPList(Pack* e, PackList* r){
  PackList* n= (PackList*) malloc(sizeof(PackList));
  n->pack = e;
  n->next = r;
  return n;
}

CmdList* mkList(Cmd* e, CmdList* r){
  CmdList* n= (CmdList*) malloc(sizeof(CmdList));
  n->cmd = e;
  n->next = r;
  return n;
}

Instr* mkInstr(Opkind o, Address* a1, Address* a2, Address* a3){
  Instr* n= (Instr*) malloc(sizeof(Instr));
  n->o = o;
  n->addr1 = a1;
  n->addr2 = a2;
  n->addr3 = a3;
  return n;
}

Address* mkAddrInt(int i){
  Address* n= (Address*) malloc(sizeof(Address));
  n->addrkind = 0;
  n->constant.value = i;
  return n;
}

Address* mkAddrVar(char* var){
  Address* n= (Address*) malloc(sizeof(Address));
  n->addrkind = E_VARI;
  n->constant.name = var;
  return n;
}

CodeList* mkCList(Instr* e, CodeList* r){
  CodeList* n = (CodeList*) malloc(sizeof(CodeList));
  n->code=e;
  n->next=r;
  return n;
}

Pair* mkPair(Address* c, CodeList* l){
  Pair* n =(Pair*) malloc(sizeof(Pair));
  n->name=c;
  n->list=l;
  return n;
}

void printvars(){
  int h;
  printf(".data\n");
  for(h=0; h<var_pos; h++){
    printf("\t%s: .word\n", array_vars[h]);
  }
}
