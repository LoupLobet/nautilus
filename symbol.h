#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#define FNV_OFFSET_BASIS 2166136261 // 32-bit
#define FNV_PRIME 16777619          // 32-bit
#define HASH_TABLE_SIZE 16

// Symbol kinds
#define K_FUNC 0
#define K_VAR 1

// Symbol types
#define T_INT 0
#define T_FLOAT 1

typedef void * Aux;
typedef struct auxFunction AuxFunction;
typedef struct auxVariable AuxVariable;
typedef struct scope Scope;
typedef struct scopeTree ScopeTree;
typedef struct symbol Symbol;
typedef struct symbolTable SymbolTable;

/*
 * The Aux field allows Symbol to contain an additional type-specific
 * data structure that may contain anything needed by the overlying symbol.
 * e.g.: A function symbol (K_FUNC) Aux would be used as AuxFunction (by casting)
 * which would contain the function arguments and scope.
 *
 * Do not forget to cast aux to the expected type when using it.
 */

struct auxFunction {
	Scope *scope;
};

struct auxVariable {
};

struct scope {
	char *name;
	SymbolTable *symbols; // hash table of symbols
	Scope *parenttable;   // parent scope table
	Scope *subtables;     // sub tables linked list
	Scope *lastsubtable;  // last sub tables in the linked list
	Scope *next;          // next ScopeTable at the same block level
};

struct scopeTree {
	Scope *root;
};

struct symbol {
	char *name;
	int kind;
	int type;
	Aux aux;

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
