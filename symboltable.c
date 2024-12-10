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
Scope_delete(Scope *scope)
{
	Scope *p;
	Scope *tdel;
	int i;
	Symbol *sym;
	Symbol *symdel;

	for (p = scope->subtables; p != NULL; p = p->next)
		Scope_delete(p);
	// free all the buckets
	for (i = 0; i < scope->symbols->nbucket; i++) {
		if (scope->symbols->buckets[i]->head != NULL) {
			sym = scope->symbols->buckets[i]->head;
			while (sym != NULL) {
				symdel = sym;
				sym = sym->next;
				free(symdel);
			}
		}
	}
	free(scope->symbols->buckets);
	free(scope->symbols);
	// loop throught parent table sub tables to delete t
	if (scope->parenttable != NULL) {
		if (scope->parenttable->subtables == scope) { // t is the only sub table
			tdel = scope;
			scope->parenttable->subtables = scope->next;
			free(tdel);
		} else {
			for (p = scope->parenttable->subtables; p->next != NULL; p = p->next) {
				if (p->next == scope) {
					tdel = p->next;
					p->next = p->next->next;
					free(tdel);
				}
			}
		}
	} else
		free(scope);
}

Symbol *
Scope_getsymbol(Scope *scope, char *name)
{
	unsigned int hash;
	Symbol *sym;

	hash = fnv1a(name, strlen(name)) % scope->symbols->nbucket;
	for (sym = scope->symbols->buckets[hash]->head; sym != NULL; sym = sym->next) {
		if (!strcmp(sym->name, name))
			return sym;
	}
	if (scope->parenttable != NULL)
		return Scope_getsymbol(scope->parenttable, name);
	return NULL;
}

Scope *
Scope_insertsubtable(Scope *scope, Scope *sub)
{
	if (scope->subtables == NULL)
		scope->subtables = sub;
	else 
		scope->lastsubtable->next = sub;
	scope->lastsubtable = sub;
	sub->parenttable = scope;
	return sub;
}

Symbol *
Scope_insertsymbol(Scope *scope, Symbol *sym)
{
	unsigned int hash;
	Symbol *p;

	// symbols are stored in a hash table
	hash = fnv1a(sym->name, strlen(sym->name)) % scope->symbols->nbucket;
	if (scope->symbols->buckets[hash]->head == NULL) {
		scope->symbols->buckets[hash]->head = sym;
	} else {
		// check for already existing symbol
		for (p = scope->symbols->buckets[hash]->head; p != NULL; p = p->next) {
			if (!strcmp(p->name, sym->name)) {
				return NULL;
			}
		}
		scope->symbols->buckets[hash]->tail->next = sym;
	}
	scope->symbols->buckets[hash]->tail = sym;
	scope->symbols->buckets[hash]->tail = NULL;
	scope->symbols->len++;
	return sym;
}

Scope *
Scope_new(char *name)
{
	Scope *scope;
	int i;

	if ((scope = malloc(sizeof(Scope))) == NULL)
		return NULL;
	scope->subtables = NULL;
	scope->parenttable = NULL;
	scope->lastsubtable = NULL;
	scope->next = NULL;
	scope->name = name;
	// init symbol hash table
	if ((scope->symbols = malloc(sizeof(HashTable))) == NULL) {
		free(scope);
		return NULL;
	}
	if ((scope->symbols->buckets = malloc(sizeof(struct bucket *) * HASH_TABLE_SIZE)) == NULL) {
		free(scope->symbols);
		free(scope);
		return NULL;
	}
	scope->symbols->nbucket = HASH_TABLE_SIZE;
	for (i = 0; i < scope->symbols->nbucket; i++) {
		if ((scope->symbols->buckets[i] = malloc(sizeof(struct bucket))) == NULL) {
			free(scope->symbols->buckets);
			free(scope->symbols);
			free(scope);
			return NULL;
		}
		scope->symbols->buckets[i]->head = NULL;
		scope->symbols->buckets[i]->tail = NULL;
	}
	scope->symbols->len = 0;
	return scope;
}

Symbol *
Scope_newfunc(Scope *scope, Symbol *func)
{
	if (Scope_insertsymbol(scope, func) == NULL)
		return NULL;
	if ((func->aux->func->scope = Scope_new(func->name)) == NULL)
		return NULL;
	if (Scope_insertsubtable(scope, func->aux->func->scope) == NULL) {
		free(func);
		return NULL;
	}
	return func;
}

Symbol *
Scope_newvar(Scope *scope, Symbol *symvar)
{
	if (Scope_insertsymbol(scope, symvar) == NULL)
		return NULL;
	return symvar;
}

ScopeTree *
ScopeTree_new(Scope *scope)
{
	ScopeTree *st;

	if ((st = malloc(sizeof(SymbolTable))) == NULL)
		return NULL;
	st->root = scope;
	return st;
}

Symbol *
Symbol_new(void)
{
	Symbol *sym;

	if ((sym = malloc(sizeof(Symbol))) == NULL)
		return NULL;
	sym->next = NULL;
	if ((sym->aux = malloc(sizeof(Aux))) == NULL) {
		free(sym);
		return NULL;
	}
	return sym;
}



#ifndef RELEASE

void
Scope_print(Scope *scope)
{
	int i;
	int nosym;
	Symbol *sym;
	Scope *subtable;

	printf("\n--------------------------\n");
	printf("NAME: %s\n", scope->name);
	printf("SYMBOLS:\n");
	nosym = 1;
	for (i = 0; i < scope->symbols->nbucket; i++) {
		for (sym = scope->symbols->buckets[i]->head; sym != NULL; sym = sym->next) {
			printf("-> %s\t%d\t%d\n", sym->name, sym->kind, sym->type);
			nosym = 0;
		}
	}
	if (nosym)
		printf("-> NULL\n");
	printf("SUBSCOPES(depth 1):\n");
	if (scope->subtables == NULL)
		printf("-> NULL\n");
	for (subtable = scope->subtables; subtable != NULL; subtable = subtable->next) {
		printf("-> %s\n", subtable->name);
	}
	printf("--------------------------\n");
}

#endif
