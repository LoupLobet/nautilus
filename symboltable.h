#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#define FNV_OFFSET_BASIS 2166136261 // 32-bit
#define FNV_PRIME 16777619          // 32-bit
#define HASH_TABLE_SIZE 16

#define K_FUNC 0
#define K_VAR 1

#define T_INT 0
#define T_FLOAT 1


typedef struct function Function;
typedef struct variable Variable;
typedef struct scope Scope;
typedef struct scopeTree ScopeTree;
typedef struct symbol Symbol;
typedef struct symbolTable SymbolTable;
typedef union aux Aux;

union aux {
	Function *func;
	Variable *var;
};

struct function {
	Scope *scope;
};

struct variable {
};

struct scope {
	char *name;
	SymbolTable *symbols; // hash table of symbols
	Scope *parenttable; // parent scope table
	Scope *subtables; // sub tables linked list
	Scope *lastsubtable; // last sub tables in the linked list
	Scope *next; // next ScopeTable at the same block level
};

struct scopeTree {
	Scope *root;
};

struct symbol {
	int kind;
	int type;
	char *name;
	Aux *aux;
	Symbol *next;
};

struct symbolTable {
	struct bucket **buckets;
	int nbucket;
	int len;
};

struct bucket {
	Symbol *head;
	Symbol *tail;
};


unsigned int	 fnv1a(char *, long);
void	 Scope_delete(Scope *); // TODO:
Symbol	*Scope_getsymbol(Scope *, char *); // TODO: (to check)
Scope	*Scope_insertsubtable(Scope *, Scope *); // TODO: (to check)
Symbol	*Scope_insertsymbol(Scope *, Symbol *); // TODO: (to check)
Scope	*Scope_new(char *); // TODO:
Symbol	*Scope_newfunc(Scope *, Symbol *);
Symbol	*Scope_newvar(Scope *, Symbol *);
ScopeTree	*ScopeTree_new(Scope *);
Symbol	*Symbol_new(void);


#ifndef RELEASE
void	 Scope_print(Scope *);
#endif


#endif
