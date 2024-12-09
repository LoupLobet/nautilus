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
	int i;
	Symbol *sym;
	Symbol *tmp;

	for (p = t->subtables; p != NULL; p = p->next)
		ScopeTable_delete(p);
	for (i = 0; i < t->symbols->nbucket; i++) {
		// free all the buckets
		if (t->symbols->buckets[i]->head != NULL) {
			sym = t->symbols->buckets[i]->head;
			while (sym != NULL) {
				tmp = sym;
				sym = sym->next;
				free(tmp);
			}
		}
	}
	free(t);
}

Symbol *
ScopeTable_getsymbol(ScopeTable *t, char *name)
{
	unsigned int hash;
	Symbol *sym;

	hash = fnv1a(name, strlen(name)) % t->symbols->nbucket;
	for (sym = t->symbols->buckets[hash]->head; sym != NULL; sym = sym->next) {
		if (!strcmp(sym->name, name))
			return sym;
	}
	if (t->parenttable != NULL)
		return ScopeTable_getsymbol(t->parenttable, name);
	return NULL;
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
	hash = fnv1a(sym->name, strlen(sym->name)) % t->symbols->nbucket;
	printf("hash: %d\n", hash);
	printf("%p\n", t->symbols->buckets[hash]);
	if (t->symbols->buckets[hash]->tail == NULL) {
		t->symbols->buckets[hash]->head = sym;
	} else {
		puts("OK");
		// check for already existing symbol
		for (p = t->symbols->buckets[hash]->head; p->next != NULL; p = p->next) {
			if (!strcmp(p->name, sym->name))
				return NULL;
		}
	}
	t->symbols->buckets[hash]->tail = sym;
	t->symbols->len++;
	sym->next = NULL;
	return sym;
}

ScopeTable *
ScopeTable_new(void)
{
	ScopeTable *t;
	int i;

	if ((t = malloc(sizeof(ScopeTable))) == NULL)
		return NULL;
	t->subtables = NULL;
	t->parenttable = NULL;
	t->lastsubtable = NULL;
	t->next = NULL;
	// init symbol hash table
	if ((t->symbols = malloc(sizeof(HashTable))) == NULL) {
		free(t);
		return NULL;
	}
	if ((t->symbols->buckets = malloc(sizeof(struct bucket *) * HASH_TABLE_SIZE)) == NULL) {
		free(t->symbols);
		free(t);
		return NULL;
	}
	t->symbols->nbucket = HASH_TABLE_SIZE;
	for (i = 0; i < t->symbols->nbucket; i++) {
		if ((t->symbols->buckets[i] = malloc(sizeof(struct bucket))) == NULL) {
			free(t->symbols->buckets);
			free(t->symbols);
			free(t);
			return NULL;
		}
		t->symbols->buckets[i]->head = NULL;
		t->symbols->buckets[i]->tail = NULL;
	}
	t->symbols->len = 0;
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

