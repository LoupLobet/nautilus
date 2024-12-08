#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#define FNV_OFFSET_BASIS 2166136261 // 32-bit
#define FNV_PRIME 16777619          // 32-bit
#define HASH_TABLE_SIZE 16

// TODO: test everything

typedef struct Symbol {
	int kind;
	int type;
	char *name;
	struct Symbol *next;
} Symbol;

typedef struct ScopeTable {
	Symbol **symbols; // symbol hash table

	struct ScopeTable *parenttable; // parent scope table
	struct ScopeTable *subtables; // sub tables linked list
	struct ScopeTable *lastsubtable; // last sub tables in the linked list
	
	struct ScopeTable *next; // next ScopeTable at the same block level
} ScopeTable;

typedef struct {
	ScopeTable *scopetables;
} SymbolTable;


unsigned int	 fnv1a(char *, long);
void	 ScopeTable_delete(ScopeTable *);
ScopeTable	*ScopeTable_insertsubtable(ScopeTable *, ScopeTable *);
Symbol	*ScopeTable_insertsymbol(ScopeTable *, Symbol *);
ScopeTable	*ScopeTable_new(void);
Symbol	*Symbol_new(void);
Symbol	*SymbolTable_getsymbol(ScopeTable *, char *);
SymbolTable	*SymbolTable_new(void);


#endif
