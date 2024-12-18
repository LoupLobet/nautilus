#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

// Symbol kinds
enum Kind {
	K_FUNC,
	K_VAR,
};

// Symbol types
enum Type {
	T_INT,
	T_FLOAT,
	T_STRING,
};

#define AUX_FUNC(sym) ((AuxFunc *)(sym->aux))
#define AUX_VAR(sym) ((AuxVar *)(sym->aux))

typedef void * Aux;
typedef struct auxFunc AuxFunc;
typedef struct auxVar AuxVar;
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

struct auxFunc{
	Scope *scope;
	int type;
	int narg;
	Symbol *args;
};

struct auxVar{
	int type;
};

struct scope {
	char *name;
	SymbolTable *symbols;
	Scope *parent;
	Scope *childs;
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

Scope	*Scope_addchild(Scope *, Scope *);
Symbol	*Scope_addsymbol(Scope *, Symbol *);
void	 Scope_delchild(Scope *, Scope *);
int	 Scope_delsymbol(Scope *, char *);
Symbol	*Scope_getsymbol(Scope *, char *);
Scope	*Scope_new(char *);
ScopeTree	*ScopeTree_new(Scope *);
Symbol	*Symbol_newfunction(char *, int, Symbol *, int);
Symbol	*Symbol_newvariable(char *, int);


#ifndef RELEASE
void	 Scope_print(Scope *);
#endif

#endif
