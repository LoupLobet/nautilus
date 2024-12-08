CC=cc
LD=cc
LEX=lex
YACC=yacc

CFLAGS=-Wall -pedantic -std=c99
LDFLAGS= 

all: lang

scanner.c: scanner.l
	${LEX} $<
scanner.o: scanner.c
	${CC} ${CFLAGS} -c $<
main.o: main.c
	${CC} ${CFLAGS} -c $<
lang: main.o scanner.o
	${LD} $^ -o $@ ${LDFLAGS}

