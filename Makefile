CC=cc
LD=cc
LEX=lex
YACC=yacc

CFLAGS=-Wall -pedantic -std=c99
LDFLAGS=

all: clean naut

scanner.c: scanner.l
	${LEX} $<
scanner.o: scanner.c
	${CC} ${CFLAGS} -c $<
symbol.o: symbol.c
	${CC} ${CFLAGS} -c $<
main.o: main.c
	${CC} ${CFLAGS} -c $<
naut: main.o scanner.o symbol.o
	${LD} $^ -o $@ ${LDFLAGS}

clean:
	rm -rf *.o scanner.c
