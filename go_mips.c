#include <stdio.h>
#include "parser.h"
#include <limits.h>
#include <string.h>

int counter=0;
int counter2=0;

void prList(CodeList* l);
void prListMips(CodeList* l);
void prInstrMips(Instr* instr);
void prInstr(Instr* instr);
Pair* compExpr(Expr* expr);
CodeList* CompileCmd(Cmd* cmd);
CodeList* append(CodeList* l1, CodeList* l2);
char* newvar2();
char* newvar();

void eval(CmdList* cmd_list) {
  CodeList* code_f;
  while(cmd_list!=NULL){
    code_f=append(code_f,CompileCmd(cmd_list->cmd));
    cmd_list=cmd_list->next;
  }
  prListMips(code_f);
  //prList(code_f);
}

CodeList* CompileCmd(Cmd* cmd){
  if(cmd->kind == E_IF){
    Instr* instr1;
    Address* addr1;
    Address* addr2;
    Pair* p1;
    Pair* p2;
    CodeList* l=NULL;

    p1 = compExpr(cmd->attr.ifs.cond->attr.op_b.left);
    p2 = compExpr(cmd->attr.ifs.cond->attr.op_b.right);

    if(p1->list == NULL){
      Address* a1 = mkAddrVar(newvar());
      Address* a2;
      if(p1->name->addrkind==E_INTJ){
        a2=mkAddrInt(p1->name->constant.value);
      }
      else{
        a2=mkAddrVar(p1->name->constant.name);
      }
      Instr* instr5 = mkInstr(T_INT,a1,a2,NULL);
      CodeList* l3 = mkCList(instr5, NULL);
      l=append(l,l3);
      p1->name=a1;
    }
    if(p2->list == NULL){
      Address* a1 = mkAddrVar(newvar());
      Address* a2;
      if(p2->name->addrkind==E_INTJ){
        a2=mkAddrInt(p2->name->constant.value);
      }
      else{
        a2=mkAddrVar(p2->name->constant.name);
      }
      Instr* instr5 = mkInstr(T_INT,a1,a2,NULL);
      CodeList* l3 = mkCList(instr5, NULL);
      l=append(l,l3);
      p2->name=a1;
    }

    l=append(l,p1->list);
    l=append(l,p2->list);
    Address* addr3 = mkAddrVar(newvar2());
    int elses = 0;
    Address* addr4;

    if(cmd->attr.ifs.cond->attr.op_b.operator_b == LT)
      instr1 = mkInstr(E_LT,p1->name,p2->name,addr3);
    else if(cmd->attr.ifs.cond->attr.op_b.operator_b == BT)
      instr1 = mkInstr(E_BT,p1->name,p2->name,addr3);
    else if(cmd->attr.ifs.cond->attr.op_b.operator_b == LET)
      instr1 = mkInstr(E_LET,p1->name,p2->name,addr3);
    else if(cmd->attr.ifs.cond->attr.op_b.operator_b == EQ)
      instr1 = mkInstr(E_EQ,p1->name,p2->name,addr3);
    else if(cmd->attr.ifs.cond->attr.op_b.operator_b == DIF)
      instr1 = mkInstr(E_DIF,p1->name,p2->name,addr3);
    else
      instr1 = mkInstr(E_BET,p1->name,p2->name,addr3);

    CodeList* l2;
    l2=mkCList(instr1,NULL);
    l = append(l,l2);

    if(cmd->attr.ifs.elses != NULL)
      l=append(l,CompileCmd(cmd->attr.ifs.elses));

    addr4 = mkAddrVar(newvar2());
    Instr* go_to = mkInstr(C_goto, addr4, NULL, NULL);
    CodeList* go_to2 = mkCList(go_to, NULL);
    l=append(l,go_to2);

    Instr* label = mkInstr(C_lab, addr3, NULL, NULL);
    CodeList* t_label = mkCList(label, NULL);
    l=append(l,t_label);

    CmdList* cmd_list = cmd->attr.ifs.cmds;
    while(cmd_list!=NULL){
      l=append(l,CompileCmd(cmd_list->cmd));
      cmd_list=cmd_list->next;
    }

    Instr* label2 = mkInstr(C_lab, addr4, NULL, NULL);
    CodeList* t_label2 = mkCList(label2, NULL);
    l=append(l,t_label2);


    return l;
  }

  else if(cmd->kind == E_ELSE){
    CodeList* l=NULL;
    CmdList* cmd_list = cmd->attr.ifs.cmds;
    while(cmd_list!=NULL){
      l=append(l,CompileCmd(cmd_list->cmd));
      cmd_list=cmd_list->next;
    }
    return l;
  }

  else if(cmd->kind == E_FOR){
    Instr* instr1;
    Address* addr1;
    Address* addr2;
    Pair* p1;
    Pair* p2;
    CodeList* l=NULL;

    Address* addr4 = mkAddrVar(newvar2());
    Instr* label2 = mkInstr(C_lab, addr4, NULL, NULL);
    CodeList* t_label2 = mkCList(label2, NULL);
    l=append(l,t_label2);

    p1 = compExpr(cmd->attr.fors.cond->attr.op_b.left);
    p2 = compExpr(cmd->attr.fors.cond->attr.op_b.right);

    if(p1->list == NULL){
      Address* a1 = mkAddrVar(newvar());
      Address* a2;
      if(p1->name->addrkind==E_INTJ){
        a2=mkAddrInt(p1->name->constant.value);
      }
      else{
        a2=mkAddrVar(p1->name->constant.name);
      }
      Instr* instr5 = mkInstr(T_INT,a1,a2,NULL);
      CodeList* l3 = mkCList(instr5, NULL);
      l=append(l,l3);
      p1->name=a1;
    }
    if(p2->list == NULL){
      Address* a1 = mkAddrVar(newvar());
      Address* a2;
      if(p2->name->addrkind==E_INTJ){
        a2=mkAddrInt(p2->name->constant.value);
      }
      else{
        a2=mkAddrVar(p2->name->constant.name);
      }
      Instr* instr5 = mkInstr(T_INT,a1,a2,NULL);
      CodeList* l3 = mkCList(instr5, NULL);
      l=append(l,l3);
      p2->name=a1;
    }

    l=append(l,p1->list);
    l=append(l,p2->list);

    Address* addr3 = mkAddrVar(newvar2());
    int elses = 0;

    if(cmd->attr.fors.cond->attr.op_b.operator_b == LT)
      instr1 = mkInstr(E_BET,p1->name,p2->name,addr3);
    else if(cmd->attr.fors.cond->attr.op_b.operator_b == BT)
      instr1 = mkInstr(E_LET,p1->name,p2->name,addr3);
    else if(cmd->attr.fors.cond->attr.op_b.operator_b == LET)
      instr1 = mkInstr(E_BT,p1->name,p2->name,addr3);
    else if(cmd->attr.fors.cond->attr.op_b.operator_b == EQ)
      instr1 = mkInstr(E_DIF,p1->name,p2->name,addr3);
    else if(cmd->attr.fors.cond->attr.op_b.operator_b == DIF)
      instr1 = mkInstr(E_EQ,p1->name,p2->name,addr3);
    else
      instr1 = mkInstr(E_LT,p1->name,p2->name,addr3);

    CodeList* l2;
    l2=mkCList(instr1,NULL);
    l = append(l,l2);

    CmdList* cmd_list = cmd->attr.fors.cmds;
    while(cmd_list!=NULL){
      l=append(l,CompileCmd(cmd_list->cmd));
      cmd_list=cmd_list->next;
    }

    Instr* go_to = mkInstr(C_goto, addr4, NULL, NULL);
    CodeList* go_to2 = mkCList(go_to, NULL);
    l=append(l,go_to2);

    Instr* label3 = mkInstr(C_lab, addr3, NULL, NULL);
    CodeList* t_label3 = mkCList(label3, NULL);
    l=append(l,t_label3);

    return l;
  }

  else if(cmd->kind == E_FOR2){
    Instr* instr1;
    Address* addr1;
    Address* addr2;
    Pair* p1;
    Pair* p2;
    CodeList* l=NULL;

    CodeList* list_decl = CompileCmd(cmd->attr.fors2.cmd);
    l=append(l, list_decl);

    Address* addr4 = mkAddrVar(newvar2());
    Instr* label2 = mkInstr(C_lab, addr4, NULL, NULL);
    CodeList* t_label2 = mkCList(label2, NULL);
    l=append(l,t_label2);

    p1 = compExpr(cmd->attr.fors2.cond->attr.op_b.left);
    p2 = compExpr(cmd->attr.fors2.cond->attr.op_b.right);

    if(p1->list == NULL){
      Address* a1 = mkAddrVar(newvar());
      Address* a2;
      if(p1->name->addrkind==E_INTJ){
        a2=mkAddrInt(p1->name->constant.value);
      }
      else{
        a2=mkAddrVar(p1->name->constant.name);
      }
      Instr* instr5 = mkInstr(T_INT,a1,a2,NULL);
      CodeList* l3 = mkCList(instr5, NULL);
      l=append(l,l3);
      p1->name=a1;
    }
    if(p2->list == NULL){
      Address* a1 = mkAddrVar(newvar());
      Address* a2;
      if(p2->name->addrkind==E_INTJ){
        a2=mkAddrInt(p2->name->constant.value);
      }
      else{
        a2=mkAddrVar(p2->name->constant.name);
      }
      Instr* instr5 = mkInstr(T_INT,a1,a2,NULL);
      CodeList* l3 = mkCList(instr5, NULL);
      l=append(l,l3);
      p2->name=a1;
    }

    l=append(l,p1->list);
    l=append(l,p2->list);

    Address* addr3 = mkAddrVar(newvar2());
    int elses = 0;

    if(cmd->attr.fors2.cond->attr.op_b.operator_b == LT)
      instr1 = mkInstr(E_BET,p1->name,p2->name,addr3);
    else if(cmd->attr.fors2.cond->attr.op_b.operator_b == BT)
      instr1 = mkInstr(E_LET,p1->name,p2->name,addr3);
    else if(cmd->attr.fors2.cond->attr.op_b.operator_b == LET)
      instr1 = mkInstr(E_BT,p1->name,p2->name,addr3);
    else if(cmd->attr.fors2.cond->attr.op_b.operator_b == EQ)
      instr1 = mkInstr(E_DIF,p1->name,p2->name,addr3);
    else if(cmd->attr.fors2.cond->attr.op_b.operator_b == DIF)
      instr1 = mkInstr(E_EQ,p1->name,p2->name,addr3);
    else
      instr1 = mkInstr(E_LT,p1->name,p2->name,addr3);

    CodeList* l2;
    l2=mkCList(instr1,NULL);
    l = append(l,l2);

    CmdList* cmd_list = cmd->attr.fors2.cmds;
    while(cmd_list!=NULL){
      l=append(l,CompileCmd(cmd_list->cmd));
      cmd_list=cmd_list->next;
    }

    CodeList* list_incr = CompileCmd(cmd->attr.fors2.incr);
    l=append(l, list_incr);

    Instr* go_to = mkInstr(C_goto, addr4, NULL, NULL);
    CodeList* go_to2 = mkCList(go_to, NULL);
    l=append(l,go_to2);

    Instr* label3 = mkInstr(C_lab, addr3, NULL, NULL);
    CodeList* t_label3 = mkCList(label3, NULL);
    l=append(l,t_label3);

    return l;

  }

  else if (cmd->kind == E_ATR){
    if(cmd->attr.atrib.expr->kind == E_INTEGER){
      Address* a1 = mkAddrVar(cmd->attr.atrib.var);
      Address* a2 = mkAddrInt(cmd->attr.atrib.expr->attr.value);
      Instr* instr1 = mkInstr(T_INT,a1,a2,NULL);
      return mkCList(instr1, NULL);
    }
    else if (cmd->attr.atrib.expr->kind == E_VAR){
      Address* a1 = mkAddrVar(cmd->attr.atrib.var);
      Address* a2 = mkAddrVar(cmd->attr.atrib.expr->attr.var);
      Instr* instr1 = mkInstr(T_INT,a1,a2,NULL);
      return mkCList(instr1, NULL);
    }
    else{
      Address* a1 = mkAddrVar(cmd->attr.atrib.var);
      CodeList* list1 = (compExpr(cmd->attr.atrib.expr)->list);
      CodeList* list2 = list1;
      while(list2->next != NULL){list2=list2->next;}
      Address* a2 = list2->code->addr1;
      Instr* instr1 = mkInstr(T_INT,a1,a2,NULL);
      return append(list1, mkCList(instr1,NULL));
    }
  }
  else if (cmd->kind == E_PRI){
    Instr* instr = mkInstr(C_PRI, mkAddrVar(cmd->attr.str), NULL, NULL);
    return mkCList(instr, NULL);
  }
  else if (cmd->kind == E_SCAN){
    Instr* instr = mkInstr(C_SCAN, mkAddrVar(cmd->attr.scans.var), NULL, NULL);
    return mkCList(instr, NULL);
  }
}

CodeList* append(CodeList* l1, CodeList* l2){
  CodeList* p = NULL;
  if(l1==NULL) return l2;
  for(p=l1; p->next!=NULL; p=p->next);
  p->next=l2;
  return l1;
}

char* newvar(){
  char* t = "t";
  counter++;
  if(counter > 9){
    counter=1;
  }
  char str[99];
  sprintf(str, "%d", counter);
  char * str3 = (char *) malloc(1 + strlen(t)+ strlen(str) );
  strcpy(str3, t);
  strcat(str3, str);
  return str3;
}

char* newvar2(){
  char* t = "L";
  counter2++;
  char str[99];
  sprintf(str, "%d", counter2);
  char * str3 = (char *) malloc(1 + strlen(t)+ strlen(str) );
  strcpy(str3, t);
  strcat(str3, str);
  return str3;
}

Pair* compExpr(Expr* expr){
  if (expr->kind == E_INTEGER) {
    Pair* p1 = mkPair(mkAddrInt(expr->attr.value),NULL);
    return p1;
  }
  else if (expr->kind == E_VAR) {
    Pair* p1 = mkPair(mkAddrVar(expr->attr.var),NULL);
    return p1;
  }
  else if (expr->kind == E_OPERATION) {
    Pair* p1;
    Pair* p2;
    CodeList* l;
    Address* t_addr;
    Instr* instr1;
    switch (expr->attr.op.operator) {
      case PLUS:
        p1 = compExpr(expr->attr.op_b.left);
        p2 = compExpr(expr->attr.op_b.right);
        l = append(p1->list,p2->list);
        t_addr = mkAddrVar(newvar());
        instr1 = mkInstr(E_ADD,t_addr,p1->name,p2->name);
        return mkPair(t_addr,append(l,mkCList(instr1,NULL)));

      case MINUS:
        p1 = compExpr(expr->attr.op_b.left);
        p2 = compExpr(expr->attr.op_b.right);
        l = append(p1->list,p2->list);
        t_addr = mkAddrVar(newvar());
        instr1 = mkInstr(E_SUB,t_addr,p1->name,p2->name);
        return mkPair(t_addr,append(l,mkCList(instr1,NULL)));

      case MULT:
        p1 = compExpr(expr->attr.op_b.left);
        p2 = compExpr(expr->attr.op_b.right);
        l = append(p1->list,p2->list);
        t_addr = mkAddrVar(newvar());
        instr1 = mkInstr(E_MUL,t_addr,p1->name,p2->name);
        return mkPair(t_addr,append(l,mkCList(instr1,NULL)));

      case DIV:
        p1 = compExpr(expr->attr.op_b.left);
        p2 = compExpr(expr->attr.op_b.right);
        l = append(p1->list,p2->list);
        t_addr = mkAddrVar(newvar());
        instr1 = mkInstr(E_DIV,t_addr,p1->name,p2->name);
        return mkPair(t_addr,append(l,mkCList(instr1,NULL)));
      default: yyerror("Unknown operator!");
    }
  }
}

void prInstr(Instr* instr){
  switch (instr->o) {
    case E_ADD:
      if(instr->addr1!=NULL && instr->addr1->addrkind == E_INTJ)
        printf("%d=", instr->addr1->constant.value);
      else if(instr->addr1!=NULL)
        printf("%s := ",instr->addr1->constant.name);
      if(instr->addr2!=NULL && instr->addr2->addrkind == E_INTJ)
        printf("%d", instr->addr2->constant.value);
      else if(instr->addr2!=NULL)
        printf("%s", instr->addr2->constant.name);
      printf("+");
      if(instr->addr3!=NULL && instr->addr3->addrkind == E_INTJ)
        printf("%d\n", instr->addr3->constant.value);
      else if(instr->addr3!=NULL && instr->addr3->addrkind == E_VARI)
        printf("%s\n", instr->addr3->constant.name);
      break;

    case E_SUB:
      if(instr->addr1!=NULL && instr->addr1->addrkind == E_INTJ)
        printf("%d=", instr->addr1->constant.value);
      else if(instr->addr1!=NULL)
        printf("%s := ",instr->addr1->constant.name);
      if(instr->addr2!=NULL && instr->addr2->addrkind == E_INTJ)
        printf("%d", instr->addr2->constant.value);
      else if(instr->addr2!=NULL)
        printf("%s", instr->addr2->constant.name);
      printf("-");
      if(instr->addr3!=NULL && instr->addr3->addrkind == E_INTJ)
        printf("%d\n", instr->addr3->constant.value);
      else if(instr->addr3!=NULL && instr->addr3->addrkind == E_VARI)
        printf("%s\n", instr->addr3->constant.name);
      break;

    case E_MUL:
      if(instr->addr1!=NULL && instr->addr1->addrkind == E_INTJ)
        printf("%d=", instr->addr1->constant.value);
      else if(instr->addr1!=NULL)
        printf("%s := ",instr->addr1->constant.name);
      if(instr->addr2!=NULL && instr->addr2->addrkind == E_INTJ)
        printf("%d", instr->addr2->constant.value);
      else if(instr->addr2!=NULL)
        printf("%s", instr->addr2->constant.name);
      printf("*");
      if(instr->addr3!=NULL && instr->addr3->addrkind == E_INTJ)
        printf("%d\n", instr->addr3->constant.value);
      else if(instr->addr3!=NULL && instr->addr3->addrkind == E_VARI)
        printf("%s\n", instr->addr3->constant.name);
      break;

    case E_DIV:
      if(instr->addr1!=NULL && instr->addr1->addrkind == E_INTJ)
        printf("%d=", instr->addr1->constant.value);
      else if(instr->addr1!=NULL)
        printf("%s := ",instr->addr1->constant.name);
      if(instr->addr2!=NULL && instr->addr2->addrkind == E_INTJ)
        printf("%d", instr->addr2->constant.value);
      else if(instr->addr2!=NULL)
        printf("%s", instr->addr2->constant.name);
      printf("/");
      if(instr->addr3!=NULL && instr->addr3->addrkind == E_INTJ)
        printf("%d\n", instr->addr3->constant.value);
      else if(instr->addr3!=NULL && instr->addr3->addrkind == E_VARI)
        printf("%s\n", instr->addr3->constant.name);
      break;

    case T_INT:
      if(instr->addr1!=NULL)
        printf("%s := ",instr->addr1->constant.name);
      if(instr->addr2!=NULL && instr->addr2->addrkind == E_INTJ)
        printf("%d\n", instr->addr2->constant.value);
      else if(instr->addr2!=NULL)
        printf("%s\n", instr->addr2->constant.name);
      break;

    case E_LT:
      if(instr->addr1!=NULL)
        printf("if %s < ",instr->addr1->constant.name);
      if(instr->addr2!=NULL && instr->addr2->addrkind == E_INTJ)
        printf("%d goto ", instr->addr2->constant.value);
      else if(instr->addr2!=NULL)
        printf("%s goto ", instr->addr2->constant.name);
      if(instr->addr3!=NULL && instr->addr2->addrkind == E_INTJ)
        printf("%d\n", instr->addr3->constant.value);
      else if(instr->addr3!=NULL)
        printf("%s\n", instr->addr3->constant.name);
      break;

    case E_BT:
      if(instr->addr1!=NULL)
        printf("if %s > ",instr->addr1->constant.name);
      if(instr->addr2!=NULL && instr->addr2->addrkind == E_INTJ)
        printf("%d goto ", instr->addr2->constant.value);
      else if(instr->addr2!=NULL)
        printf("%s goto ", instr->addr2->constant.name);
      if(instr->addr3!=NULL && instr->addr2->addrkind == E_INTJ)
        printf("%d\n", instr->addr3->constant.value);
      else if(instr->addr3!=NULL)
        printf("%s\n", instr->addr3->constant.name);
      break;

    case E_LET:
      if(instr->addr1!=NULL)
        printf("if %s <= ",instr->addr1->constant.name);
      if(instr->addr2!=NULL && instr->addr2->addrkind == E_INTJ)
        printf("%d goto ", instr->addr2->constant.value);
      else if(instr->addr2!=NULL)
        printf("%s goto ", instr->addr2->constant.name);
      if(instr->addr3!=NULL && instr->addr2->addrkind == E_INTJ)
        printf("%d\n", instr->addr3->constant.value);
      else if(instr->addr3!=NULL)
        printf("%s\n", instr->addr3->constant.name);
      break;

    case E_BET:
      if(instr->addr1!=NULL)
        printf("if %s >= ",instr->addr1->constant.name);
      if(instr->addr2!=NULL && instr->addr2->addrkind == E_INTJ)
        printf("%d goto ", instr->addr2->constant.value);
      else if(instr->addr2!=NULL)
        printf("%s goto ", instr->addr2->constant.name);
      if(instr->addr3!=NULL && instr->addr2->addrkind == E_INTJ)
        printf("%d\n", instr->addr3->constant.value);
      else if(instr->addr3!=NULL)
        printf("%s\n", instr->addr3->constant.name);
      break;

    case E_EQ:
      if(instr->addr1!=NULL)
        printf("if %s == ",instr->addr1->constant.name);
      if(instr->addr2!=NULL && instr->addr2->addrkind == E_INTJ)
        printf("%d goto ", instr->addr2->constant.value);
      else if(instr->addr2!=NULL)
        printf("%s goto ", instr->addr2->constant.name);
      if(instr->addr3!=NULL && instr->addr2->addrkind == E_INTJ)
        printf("%d\n", instr->addr3->constant.value);
      else if(instr->addr3!=NULL)
        printf("%s\n", instr->addr3->constant.name);
      break;

    case E_DIF:
      if(instr->addr1!=NULL)
        printf("if %s != ",instr->addr1->constant.name);
      if(instr->addr2!=NULL && instr->addr2->addrkind == E_INTJ)
        printf("%d goto ", instr->addr2->constant.value);
      else if(instr->addr2!=NULL)
        printf("%s goto ", instr->addr2->constant.name);
      if(instr->addr3!=NULL && instr->addr2->addrkind == E_INTJ)
        printf("%d\n", instr->addr3->constant.value);
      else if(instr->addr3!=NULL)
        printf("%s\n", instr->addr3->constant.name);
      break;

    case C_PRI:
      printf("print(%s)\n", instr->addr1->constant.name);
      break;

    case C_SCAN:
      printf("%s := scan()\n", instr->addr1->constant.name);
      break;

    case C_lab:
      printf("label ");
      printf("%s\n", instr->addr1->constant.name);
      break;

    case C_goto:
      printf("goto ");
      printf("%s\n", instr->addr1->constant.name);
      break;

    default: yyerror("Unknown operator!");
  }
}

void prList(CodeList* l){
  while(l!=NULL){
    Instr* instr = l->code;
    prInstr(instr);
    l = l->next;
  }
}

void prInstrMips(Instr* instr){
  if(instr->o == E_ADD || instr->o == E_SUB || instr->o == E_MUL || instr->o == E_DIV){
    char* oper;
    switch (instr->o) {
      case E_ADD: oper="add"; break;
      case E_SUB: oper="sub"; break;
      case E_MUL: oper="mul"; break;
      case E_DIV: oper="div"; break;
    }
    if(instr->addr2->addrkind == E_INTJ && instr->addr3->addrkind == E_INTJ){
      printf("li $t0, %d\n", instr->addr2->constant.value);
      printf("\t%s $%s, $t0, %d\n", oper, instr->addr1->constant.name, instr->addr3->constant.value);
    }
    else if(instr->addr2->addrkind != E_INTJ && instr->addr3->addrkind == E_INTJ){
      if(instr->addr2->constant.name[0] != 't'){
        printf("lw $t0, %s\n", instr->addr2->constant.name);
        printf("\t%s $%s, $t0, %d\n", oper, instr->addr1->constant.name, instr->addr3->constant.value);
      }
      else {
        printf("%s $%s, $%s, %d\n", oper, instr->addr1->constant.name,
              instr->addr2->constant.name, instr->addr3->constant.value);
      }
    }
    else if(instr->addr2->addrkind == E_INTJ && instr->addr3->addrkind != E_INTJ){
      if(instr->addr3->constant.name[0] != 't'){
        printf("lw $t0, %s\n", instr->addr3->constant.name);
        printf("\tli $s1, %d\n", instr->addr2->constant.value);
        printf("\t%s $%s, $s1, $t0\n", oper, instr->addr1->constant.name);
      }
      else {
        printf("li $t0, %d\n", instr->addr2->constant.value);
        printf("\t%s $%s, $t0, $%s\n", oper, instr->addr1->constant.name, instr->addr3->constant.name);
      }
    }
    else{
      if(instr->addr2->constant.name[0] != 't' && instr->addr3->constant.name[0] != 't'){
        printf("lw $t0, %s\n", instr->addr2->constant.name);
        printf("\tlw $s1, %s\n", instr->addr3->constant.name);
        printf("\t%s $%s, $t0, $s1\n", oper, instr->addr1->constant.name);
      }
      else {
        if(instr->addr2->constant.name[0] != 't'){
          printf("lw $t0, %s\n", instr->addr2->constant.name);
          printf("\t%s $%s, $t0, %d\n", oper, instr->addr1->constant.name, instr->addr3->constant.value);
        }else if(instr->addr3->constant.name[0] != 't'){
          printf("lw $t0, %s\n", instr->addr3->constant.name);
          printf("\t%s $%s, $t0, %d\n", oper, instr->addr1->constant.name, instr->addr2->constant.value);
        }else printf("%s $%s, $%s, $%s\n", oper, instr->addr1->constant.name,
                instr->addr2->constant.name, instr->addr3->constant.name);
      }
    }
  }
  else if(instr->o == T_INT){
    if(instr->addr1!=NULL && instr->addr2!=NULL && instr->addr2->addrkind == E_INTJ){
      if(instr->addr1->constant.name[0] != 't'){
        printf("li $t0, %d\n", instr->addr2->constant.value);
        printf("\tsw $t0, %s\n", instr->addr1->constant.name);
      }
      else printf("li $%s, %d\n",instr->addr1->constant.name, instr->addr2->constant.value);
    }
    else if(instr->addr1!=NULL && instr->addr2!=NULL){
      if(instr->addr2->constant.name[0] == 't')
        printf("sw $%s, %s\n",instr->addr2->constant.name, instr->addr1->constant.name);
      else
        printf("lw $%s, %s\n",instr->addr1->constant.name, instr->addr2->constant.name);
    }
  }
  else if(instr->o == E_LT || instr->o == E_BT || instr->o == E_LET ||
            instr->o == E_BET || instr->o == E_DIF || instr->o == E_EQ){
    char* oper;
    switch (instr->o) {
      case E_LT: oper="blt"; break;
      case E_BT: oper="bgt"; break;
      case E_LET: oper="ble"; break;
      case E_BET: oper="bge"; break;
      case E_EQ: oper="beq"; break;
      case E_DIF: oper="bne"; break;
    }
    printf("%s $%s, $%s, %s\n", oper, instr->addr1->constant.name,
          instr->addr2->constant.name, instr->addr3->constant.name);
  }
  else if (instr->o == C_PRI) {
    printf("li $v0, 1\n");
    printf("\tlw $t0, %s\n", instr->addr1->constant.name);
    printf("\tadd $a0, $zero, $t0\n");
    printf("\tsyscall\n");
  }

  else if (instr->o == C_SCAN) {
    printf("addi $v0, $zero, 5\n");
    printf("\tsyscall\n");
    printf("\tadd $t0, $zero, $v0\n");
    printf("\tsw $t0, %s\n",instr->addr1->constant.name);
  }
  else if (instr->o == C_lab)
    printf("%s:\n", instr->addr1->constant.name);
  else if (instr->o == C_goto)
    printf("j %s\n", instr->addr1->constant.name);
}

void prListMips(CodeList* l){
  printvars();
  printf(".text\n");
  while(l!=NULL){
    Instr* instr = l->code;
    printf("\t");
    prInstrMips(instr);
    l = l->next;
  }
  printf("\tli $v0, 10\n");
  printf("\tsyscall\n");
}

int main(int argc, char** argv) {
  --argc; ++argv;
  if (argc != 0) {
    yyin = fopen(*argv, "r");
    if (!yyin) {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  }
  if (yyparse() == 0) {
      eval(root);
  }
  return 0;
}
