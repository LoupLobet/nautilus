#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"

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
		if (Scope_addchild(scope, ((AuxFunction *)sym->aux)->scope) == NULL)
			return NULL;
	}
	return sym;
}

void
Scope_del(Scope *scope)
{
	Scope *p, *scopedel, *last;
	Symbol *sym, *symdel;
	int i;

	for (p = scope->childs; p != NULL; p = p->next)
		Scope_del(p);
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
	if (scope->parent != NULL) {
		if (scope->parent->childs == scope) { // t is the only sub table
			scopedel = scope;
			scope->parent->childs = scope->next;
			free(scopedel);
		} else {
			last = NULL;
			for (p = scope->parent->childs; p != NULL; p = p->next) {
				if (p == scope) {
					if (last == NULL)
						scope->parent->childs = p->next;
					else
						last->next = p->next;
					free(p);
				}
				last = p;
			}
		}
	} else 
		free(scope);
}

int
Scope_delsymbol(Scope *scope, char *name)
{
	unsigned int hash;
	Symbol *sym, *last;

	hash = fnv1a(name, strlen(name)) % scope->symbols->nbucket;
	last = NULL;
	if (scope->symbols->buckets[hash]->head == NULL)
		return 1;
	for (sym = scope->symbols->buckets[hash]->head; sym != NULL; sym = sym->next) {
		if (!strcmp(sym->name, name)) {
			// symbol exists in the scope delete it
			switch (sym->kind) {
			case K_VAR:
				if (last == NULL)
					scope->symbols->buckets[hash]->head = sym->next;
				else
					last->next = sym->next;
				free(sym);
				break;
			case K_FUNC:
				Scope_del(((AuxFunction *)sym->aux)->scope);
				if (last == NULL)
					scope->symbols->buckets[hash]->head = sym->next;
				else
					last->next = sym->next;
				free(sym);
				break;
			default:
				// if this happen the problem is between chair and monitor :)
				fprintf(stderr, "unexpected error: ivalid symbol kind: %d\n", sym->kind);
				exit(1);
			}
			return 0;
		}
		last = sym;
	}
	return 1;
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
	if (scope->childs == NULL) {
		scope->childs = child;
		scope->lastchild = child;
	} else 
		scope->lastchild->next = child;
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
	scope->lastchild = NULL;
	scope->next = NULL;
	scope->name = name;
	// init symbol hash table
	if ((scope->symbols = malloc(sizeof(SymbolTable))) == NULL) {
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
	AuxFunction *aux;

	if ((sym = malloc(sizeof(Symbol))) == NULL)
		return NULL;
	sym->name = name;
	sym->kind = K_FUNC;
	if ((aux = malloc(sizeof(AuxFunction))) == NULL) {
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
	AuxVariable *aux;

	if ((sym = malloc(sizeof(Symbol))) == NULL)
		return NULL;
	sym->name = name;
	sym->kind = K_VAR;
	if ((aux = malloc(sizeof(AuxFunction))) == NULL) {
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
				printf("\ttype: %d\n", ((AuxVariable *)sym->aux)->type);
				break;
			case K_FUNC:
				printf("-> %s\t%s:\n", sym->name, "K_FUNC");
				printf("\ttype: %d\n", ((AuxFunction *)sym->aux)->type);
				printf("\targs(%d): ", ((AuxFunction *)sym->aux)->narg);
				for (arg = ((AuxFunction *)sym->aux)->args; arg != NULL; arg = arg->next) { 
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
