
// AST definitions
#ifndef __ast_h__
#define __ast_h__

// AST for packages and imports
struct _Packs{
  enum {
    E_PACK,
    E_IMP
  } kind;
  union {
    char* var;  //package
    char* str;  //import
  } attr;
};

// AST for expressions
struct _Expr {
  enum {
    E_INTEGER,
    E_VAR,
    E_OPERATION,
    E_BOOL
  } kind;
  union {
    int value; // for integer values
    char* var;
    struct {
      int operator_b;
      struct _Expr* left;
      struct _Expr* right;
    }op_b;
    struct {
      int operator; // PLUS, MINUS, etc
      struct _Expr* left;
      struct _Expr* right;
    } op; // for binary expressions
  } attr;
};

typedef struct _Expr Expr; // Convenience typedef

typedef struct _Packs Pack;

typedef struct _Cmd Cmd;

typedef struct _PackList PackList;

typedef struct _CmdList CmdList;

typedef struct _CodeList CodeList;

typedef struct _Address Address;

typedef struct _Instr Instr;

typedef struct _Pair Pair;

typedef enum {
  E_ATRIB,
  E_ADD,
  E_SUB,
  E_MUL,
  E_DIV,
  E_EQ,
  E_DIF,
  E_LT,
  E_BT,
  E_LET,
  E_BET,
  E_EQ_S,
  C_PRI,
  C_SCAN,
  C_IF,
  T_INT,
  C_lab,
  C_goto
} Opkind;

typedef enum {
  E_INTJ,
  E_VARI
} Addrkind;

struct _Cmd { //TODO
  enum {
    E_IF,
    E_ELSE,
    E_FOR,
    E_FOR2,
    E_ATR,
    E_PRI,
    E_SCAN
  } kind;
  union {
    struct {        // If
      Expr* cond;
      CmdList* cmds;
      Cmd* elses;
    }ifs;
    struct {        // Atribs
      char* var;
      Expr* expr;
    }atrib;
    struct {
      Expr* cond;
      CmdList* cmds;     // While
    }fors;
    struct {
      Cmd* cmd;
      Expr* cond;
      Cmd* incr;
      CmdList* cmds;     // For
    }fors2;
    char* str;     // Print
    struct {
      char* str;
      char* var;
    }scans;       // Scan
  } attr;
};

struct _Instr {
  Opkind o;
  Address* addr1,* addr2,* addr3;
};

struct _Address {
  Addrkind addrkind;
  union {
    int value;
    char* name;
  } constant;
};

struct _PackList {
  Pack* pack;
  struct _PackList* next;
};

struct _CmdList {
  Cmd* cmd;
  struct _CmdList* next;
};

struct _CodeList {
  Instr* code;
  struct _CodeList* next;
};

struct _Pair{
  Address* name;
  struct _CodeList* list;
};

// Constructor functions (see implementation in ast.c)

Expr* ast_integer(int v);
Expr* ast_var(char* var);
Expr* ast_operation(int operator, Expr* left, Expr* right);
Expr* ast_bool(int operator, Expr* left, Expr* right);
Pack* pack_Package(char* var);
Pack* pack_Import(char* str);
Cmd* cmd_If(Expr* cond, CmdList* cmds, Cmd* elses);
Cmd* cmd_Else(CmdList* cmds);
Cmd* cmd_For(Expr* cond, CmdList* cmds);
Cmd* cmd_For2(Cmd* cmd, Expr* cond, Cmd* incr, CmdList* cmds);
Cmd* cmd_atrib(char* var, Expr* expr);
Cmd* cmd_print(char* str);
Cmd* cmd_scan(char* str, char* var);
PackList* mkPList(Pack* e, PackList* r);
CmdList* mkList(Cmd* e, CmdList* r);
Instr* mkInstr(Opkind o, Address* a1, Address* a2, Address* a3);
Address* mkAddrInt(int i);
Address* mkAddrVar(char* var);
CodeList* mkCList(Instr* e, CodeList* r);
Pair* mkPair(Address* c, CodeList* l);
void printvars();

#endif
