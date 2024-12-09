#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#define FNV_OFFSET_BASIS 2166136261 // 32-bit
#define FNV_PRIME 16777619          // 32-bit
#define HASH_TABLE_SIZE 16

#define KIND_FUNC 0
#define KIND_VAR 1

#define TYPE_INT 0
#define TYPE_FLOAT 1

// TODO: test everything

typedef struct Symbol {
	int kind;
	int type;
	char *name;
	struct Symbol *next;
} Symbol;

struct bucket {
	Symbol *head;
	Symbol *tail;
};

typedef struct {
	struct bucket **buckets;
	int nbucket;
	int len;
} HashTable;

typedef struct ScopeTable {
	HashTable *symbols;

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
Symbol	*ScopeTable_getsymbol(ScopeTable *, char *);
ScopeTable	*ScopeTable_insertsubtable(ScopeTable *, ScopeTable *);
Symbol	*ScopeTable_insertsymbol(ScopeTable *, Symbol *);
ScopeTable	*ScopeTable_new(void);
Symbol	*Symbol_new(void);
SymbolTable	*SymbolTable_new(void);


#endif
