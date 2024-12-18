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
	int type;
	int narg;
	Symbol *args;
};

struct auxVariable {
	int type;
};

struct scope {
	char *name;
	SymbolTable *symbols;
	Scope *parent;
	Scope *childs;
	// NOTE: lastchild can be something better, juste qof pointer to
	// the list tail, but a bit dirty, might be better.
	Scope *lastchild;
	Scope *next;
};

struct scopeTree {
	Scope *root;
};

struct symbol {
	char *name;
	int kind;
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
Symbol	*Scope_addsymbol(Scope *, Symbol *);
void	 Scope_del(Scope *, Scope *);
int	 Scope_delsymbol(Scope *, char *);
Symbol	*Scope_getsymbol(Scope *, char *);
Scope	*Scope_addchild(Scope *, Scope *);
Scope	*Scope_new(char *);
ScopeTree	*ScopeTree_new(Scope *);
Symbol	*Symbol_newfunction(char *, int, Symbol *, int);
Symbol	*Symbol_newvariable(char *, int);


#ifndef RELEASE
void	 Scope_print(Scope *);
#endif


#endif
