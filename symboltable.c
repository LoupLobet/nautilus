#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symboltable.h"

unsigned int
fnv1a(char *s, long n)
{
	unsigned int digest;
	int i;

	digest = FNV_OFFSET_BASIS;
	for (i = 0; i < n; i++) {
		digest = digest ^ s[i];
		digest = digest * FNV_PRIME;
	}
	return digest;
}

void
ScopeTable_delete(ScopeTable *t)
{
	ScopeTable *p;

	for (p = t->subtables; p != NULL; p = p->next)
		ScopeTable_delete(p);
	free(t);
}

ScopeTable *
ScopeTable_insertsubtable(ScopeTable *t, ScopeTable *sub)
{
	if (t->subtables == NULL) {
		t->subtables = sub;
	}
	t->lastsubtable = sub;
	sub->parenttable = t;
	return sub;
}

Symbol *
ScopeTable_insertsymbol(ScopeTable *t, Symbol *sym)
{
	unsigned int hash;
	Symbol *p;

	// symbols are stored in a hash table
	hash = fnv1a(sym->name, strlen(sym->name)) % HASH_TABLE_SIZE;
	if (t->symbols[hash] == NULL)
		t->symbols[hash] = sym;
	else {
		for (p = t->symbols[hash]; p->next != NULL; p = p->next) {
			// check for already existing symbol
			if (!strcmp(p->name, sym->name))
				return NULL;
		}
		p->next = sym;
	}
	return sym;
}

ScopeTable *
ScopeTable_new(void)
{
	ScopeTable *t;

	if ((t = malloc(sizeof(ScopeTable))) == NULL)
		return NULL;
	if ((t->symbols = malloc(sizeof(Symbol *) * HASH_TABLE_SIZE)) == NULL) {
		free(t);
		return NULL;
	}
	t->subtables = NULL;
	t->parenttable = NULL;
	t->lastsubtable = NULL;
	t->next = NULL;
	return t;
}

Symbol *
Symbol_new(void)
{
	Symbol *sym;

	if ((sym = malloc(sizeof(Symbol))) == NULL)
		return NULL;
	sym->next = NULL;
	return sym;
}

Symbol *
ScopeTable_getsymbol(ScopeTable *t, char *name)
{
	unsigned int hash;
	Symbol *sym;

	hash = fnv1a(name, strlen(name)) % HASH_TABLE_SIZE;
	for (sym = t->symbols[hash]; sym != NULL; sym = sym->next) {
		if (!strcmp(sym->name, name))
			return sym;
	}
	if (t->parenttable != NULL)
		return ScopeTable_getsymbol(t->parenttable, name);
	return NULL;
}

SymbolTable *
SymbolTable_new(void)
{
	SymbolTable *st;

	if ((st = malloc(sizeof(SymbolTable))) == NULL)
		return NULL;
	if ((st->scopetables = ScopeTable_new()) == NULL) {
		free(st);
		return NULL;
	}
	return st;
}

