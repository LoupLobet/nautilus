#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"

// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
#define FNV_OFFSET_BASIS 2166136261 // 32-bit
#define FNV_PRIME 16777619          // 32-bit
#define HASH_TABLE_SIZE 16

unsigned int	 fnv1a(char *, long);

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

Symbol *
Scope_addsymbol(Scope *scope, Symbol *sym)
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
	// if the symbol is a function, link the symbol scope with
	// the upper scope.
	if (sym->kind == K_FUNC) {
		if (Scope_addchild(scope, AUX_FUNC(sym)->scope) == NULL)
			return NULL;
	}
	return sym;
}

void
Scope_delchild(Scope *parent, Scope *scope)
{
	// Scope_delchild should not be used directly on non-anonymous scopes
	// because it doesn't delete the associated scope symbol in parent
	// scope.
	Scope *p, *child, *last;
	Symbol *sym, *symdel;
	int i;

	// delete sub scopes
	for (p = scope->childs; p != NULL; p = p->next)
		Scope_delchild(scope, p);
	// remove the scope entry from the parent child list
	if (parent != NULL) {
		last = NULL;
		for (child = parent->childs; child != NULL; child = child->next) {
			if (child == scope) {
				if (last == NULL)
					parent->childs = child->next;
				else
					last->next = child->next;
			}
			last = child;
		}
	}
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
	free(scope->name);
	free(scope);
}

int
Scope_delsymbol(Scope *scope, char *name)
{
	unsigned int hash;
	Symbol *arg, *sym, *last, *next;
	Scope *scopedel;

	hash = fnv1a(name, strlen(name)) % scope->symbols->nbucket;
	last = NULL;
	if (scope->symbols->buckets[hash]->head == NULL)
		return 1;
	for (sym = scope->symbols->buckets[hash]->head; sym != NULL; sym = sym->next) {
		// symbol exists in the scope delete it
		if (!strcmp(sym->name, name)) { 
			if (last == NULL)
				scope->symbols->buckets[hash]->head = sym->next;
			else
				last->next = sym->next;
			if (sym->kind == K_FUNC) {
				scopedel = AUX_FUNC(sym)->scope;
				// if the symbol is a function, delete its scope and its
				// variables / subscopes.
				Scope_delchild(scope, scopedel);
				// delete function arguments (allocated)
				next = NULL;
				arg = AUX_FUNC(sym)->args;
				while (arg != NULL) {
					next = arg->next;
					free(arg);
					arg = next;
				}
			}
			free(sym->aux);
			free(sym->name);
			free(sym);
		}
		last = sym;
	}
	return 0;
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
	if (scope->parent != NULL)
		return Scope_getsymbol(scope->parent, name);
	return NULL;
}

Scope *
Scope_addchild(Scope *scope, Scope *child)
{
	if (scope->childs == NULL)
		scope->childs = child;
	else {
		child->next = scope->childs;
		scope->childs = child;
	}
	child->parent = scope;
	return child;
}

Scope *
Scope_new(char *name)
{
	Scope *scope;
	int i;

	if ((scope = malloc(sizeof(Scope))) == NULL)
		return NULL;
	scope->childs = NULL;
	scope->parent = NULL;
	scope->next = NULL;
	if ((scope->name = strdup(name)) == NULL) {
		free(scope);
		return NULL;
	}
	// init symbol hash table
	if ((scope->symbols = malloc(sizeof(SymbolTable))) == NULL) {
		free(scope->name);
		free(scope);
		return NULL;
	}
	if ((scope->symbols->buckets = malloc(sizeof(struct bucket *) * HASH_TABLE_SIZE)) == NULL) {
		free(scope->symbols);
		free(scope->name);
		free(scope);
		return NULL;
	}
	scope->symbols->nbucket = HASH_TABLE_SIZE;
	for (i = 0; i < scope->symbols->nbucket; i++) {
		if ((scope->symbols->buckets[i] = malloc(sizeof(struct bucket))) == NULL) {
			free(scope->symbols->buckets);
			free(scope->symbols);
			free(scope->name);
			free(scope);
			return NULL;
		}
		scope->symbols->buckets[i]->head = NULL;
		scope->symbols->buckets[i]->tail = NULL;
	}
	scope->symbols->len = 0;
	return scope;
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
Symbol_newfunction(char *name, int type, Symbol *args, int narg)
{
	Scope *scope;
	Symbol *sym;
	AuxFunc *aux;

	if ((sym = malloc(sizeof(Symbol))) == NULL)
		return NULL;
	if ((sym->name = strdup(name)) == NULL) {
		free(sym);
		return NULL;
	}
	sym->kind = K_FUNC;
	if ((aux = malloc(sizeof(AuxFunc))) == NULL) {
		free(sym->name);
		free(sym);
		return NULL;
	}
	aux->type = type;
	aux->args = args;
	aux->narg = narg;
	sym->aux = aux;
	// create function scope
	if ((scope = Scope_new(name)) == NULL) {
		free(aux);
		free(sym->name);
		free(sym);
		return NULL;
	}
	aux->scope = scope;
	sym->next = NULL;
	return sym;
}

Symbol *
Symbol_newvariable(char *name, int type)
{
	Symbol *sym;
	AuxVar *aux;

	if ((sym = malloc(sizeof(Symbol))) == NULL)
		return NULL;
	if ((sym->name = strdup(sym->name = name)) == NULL) {
		free(sym);
		return NULL;
	}
	sym->kind = K_VAR;
	if ((aux = malloc(sizeof(AuxFunc))) == NULL) {
		free(sym->name);
		free(sym);
		return NULL;
	}
	aux->type = type;
	sym->aux = aux;
	sym->next = NULL;
	return sym;
}

#ifndef RELEASE

void
Scope_print(Scope *scope)
{
	int i;
	int nosym;
	Symbol *sym, *arg;
	Scope *subtable;

	printf("--------------------------\n");
	printf("NAME: %s\n", scope->name);
	printf("SYMBOLS:\n");
	nosym = 1;
	for (i = 0; i < scope->symbols->nbucket; i++) {
		for (sym = scope->symbols->buckets[i]->head; sym != NULL; sym = sym->next) {
			switch (sym->kind){
			case K_VAR:
				printf("-> %s\t%s:\n", sym->name, "K_VAR");
				printf("\ttype: %d\n", AUX_VAR(sym)->type);
				break;
			case K_FUNC:
				printf("-> %s\t%s:\n", sym->name, "K_FUNC");
				printf("\ttype: %d\n", AUX_VAR(sym)->type);
				printf("\targs(%d): ", AUX_FUNC(sym)->narg);
				for (arg = AUX_FUNC(sym)->args; arg != NULL; arg = arg->next) { 
					printf("%s, ", arg->name);
				}
				printf("\n");
				break;
			}
			nosym = 0;
		}
	}
	if (nosym)
		printf("-> NULL\n");
	printf("SUBSCOPES(depth 1):\n");
	if (scope->childs == NULL)
		printf("-> NULL\n");
	for (subtable = scope->childs; subtable != NULL; subtable = subtable->next) {
		printf("-> %s\n", subtable->name);
	}
	printf("--------------------------\n");
}

#endif
